/*-------------------------------------------------------------------------
    
-------------------------------------------------------------------------*/
/*********************************************************
  Copyright (C), 2013-2016
  File name:	hw_ioctrl.c
  Author:   	
  Version:   	1.0
  Date: 		14.04.08
  Description:  
  History:                   
    1. Date:
       Author:
       Modification:
    2. ...
*********************************************************/
#include <stdio.h> 
#include <string.h> 
#include "hw_define.h"
#include "hw_serial.h"
#include "hw_bluetooth.h"
#include "log.h"
#if(LEVEL_DEBUG & LEVEL_DRIVER)
#define LOGD        hw_serial_printf
#else
#define LOGD        hw_serial_printf_null  
#endif

static const BT_CMD mBTCMD[] = 
{
    {BT_CMD_AT,         "AT",                 2,    0},
    {BT_CMD_NAME,       "AT+NAME",             7,    0},
    {BT_CMD_VERSION,    "AT+VERSION",          10,    0},
    {BT_CMD_DEFAULT,    "AT+DEFAULT",          10,    0},
    {BT_CMD_RESET,      "AT+RESET",            8,    0},
    {BT_CMD_PIN,        "AT+PIN",              6,    0},
    {BT_CMD_BAUD,       "AT+BAUD",             7,    0},
    {BT_CMD_UARTMODE,   "AT+UARTMODE",         11,    0},
    {BT_CMD_LADDR,      "AT+LADDR",            8,    0},
    {BT_CMD_STATE,      "AT+STATE",            8,    0},
    {BT_CMD_CMODE,      "AT+CMODE",            8,    0},
    {BT_CMD_LOWPOWER,   "AT+LOWPOWER",         11,    0},
    {BT_CMD_AUTOCONN,   "AT+AUTOCONN",         11,    0},
    {BT_CMD_ROLE,       "AT+ROLE",             7,    0},
    {BT_CMD_ENABLEIND,  "AT+ENABLEIND",        12,    0},
    {BT_CMD_CLEAR,      "AT+CLEAR",           8,     0},
    {BT_CMD_SUPERVERSION, "AT+SUPERVERSION",   15,    0}
};

static BT_CMD mInitCMD[] = 
{
    {BT_CMD_LADDR,        "",                   0,    0},
    {BT_CMD_PIN,          BT_DEFAULT_PIN,       6,    1}, 
    {BT_CMD_NAME,         BT_DEFAULT_NAME,     11,    1},
    {BT_CMD_STATE,        "",                   0,    0},    
};

#define BT_INITCMD_MAX  4

static int8_t mInitCount = -1;
static BT_FRAME mRecvBuf[BT_RECV_MAX];
static uint8_t mBTRecvIndex = 0;
static BT_FRAME mSendBuf[BT_SEND_MAX];
static int8_t mBTSendIndex = -1;
static uint8_t mBTState = 0;
static uint8_t mBTRecvACK = 0;
static uint8_t mBTNeedRecv = 0;
/*************************************************
  Function:		BTSwitchRecvBuf
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static void BTSwitchRecvBuf(void)
{
	mBTRecvIndex++;
	if(mBTRecvIndex >=  BT_RECV_MAX)
	{
		mBTRecvIndex = 0;
	}
}

/*************************************************
  Function:		BTGetRecvBufIndex
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static int BTGetRecvBufIndex(uint8_t type)
{
    uint8_t i;
    int ret = -1;
    for (i = 0 ; i < BT_RECV_MAX; i++)
    {
        if(type == BT_FRAME_FIND_FULL)
        {
            if(mRecvBuf[i].FrameState == BT_FRAME_STATE_FINISH)
            {
                ret = i;
                break;
            }   
        }
        else
        {
            if(mRecvBuf[i].FrameState == BT_FRAME_STATE_NONE)
            {
                ret = i;
                break;
            } 
        } 
    }

    return ret;
}

/*************************************************
  Function:		BTGetSendBufIndex
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static int BTGetSendBufIndex(uint8_t type)
{
    uint8_t i;
    int ret = -1;
    for (i = 0 ; i < BT_SEND_MAX; i++)
    {
        if(type == BT_FRAME_FIND_FULL)
        {
            if(mSendBuf[i].FrameState == BT_FRAME_STATE_SEND)
            {
                ret = i;
                break;
            }   
        }
        else
        {
            if(mSendBuf[i].FrameState == BT_FRAME_STATE_NONE)
            {
                ret = i;
                break;
            } 
        } 
    }

    return ret;
}

/*************************************************
  Function:		hw_bluetooth_uart_proc
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
int hw_bluetooth_uart_proc(uint8_t buf)
{
    int ret = -1;
    if(mRecvBuf[mBTRecvIndex].FrameState == BT_FRAME_STATE_NONE)
    {
        mRecvBuf[mBTRecvIndex].FrameState = BT_FRAME_STATE_RECV;

        if('+' == buf || 'O' == buf)
        {
            mRecvBuf[mBTRecvIndex].FrameTime = -1;
            mRecvBuf[mBTRecvIndex].FrameType = BT_FRAME_TYPE_CMD;
            mRecvBuf[mBTRecvIndex].FrameBuf[0] = buf;
            mRecvBuf[mBTRecvIndex].FrameLen = 1;
        } 
        else if(buf == 0xAA)
        {
            mRecvBuf[mBTRecvIndex].FrameTime = 0;
            mRecvBuf[mBTRecvIndex].FrameType = BT_FRAME_TYPE_DATA_HEAD;
            mRecvBuf[mBTRecvIndex].FrameBuf[0] = buf;
            mRecvBuf[mBTRecvIndex].FrameLen = 1;
            mBTNeedRecv = 5;
        }
        
    }
    else if(mRecvBuf[mBTRecvIndex].FrameState == BT_FRAME_STATE_RECV)
    {
        if(mRecvBuf[mBTRecvIndex].FrameType == BT_FRAME_TYPE_CMD)
        {
            mRecvBuf[mBTRecvIndex].FrameBuf[mRecvBuf[mBTRecvIndex].FrameLen++] = buf;
            mRecvBuf[mBTRecvIndex].FrameTime = 0;
            if(('\n' == buf)||(mRecvBuf[mBTRecvIndex].FrameLen >= BT_FRAME_MAX))
            {
               mRecvBuf[mBTRecvIndex].FrameState = BT_FRAME_STATE_FINISH;
               BTSwitchRecvBuf();
            }
        }
        else
        {
            mRecvBuf[mBTRecvIndex].FrameTime = 0;
            if (mRecvBuf[mBTRecvIndex].FrameType == BT_FRAME_TYPE_DATA_HEAD)
			{    
				mRecvBuf[mBTRecvIndex].FrameBuf[mRecvBuf[mBTRecvIndex].FrameLen] = buf;
				mBTNeedRecv--;
				if(mBTNeedRecv == 0)
				{
					mBTNeedRecv = mRecvBuf[mBTRecvIndex].FrameBuf[mRecvBuf[mBTRecvIndex].FrameLen];
					mRecvBuf[mBTRecvIndex].FrameType = BT_FRAME_TYPE_DATA_END;
				}	
				mRecvBuf[mBTRecvIndex].FrameLen++;	
			}
			else
			{
				mRecvBuf[mBTRecvIndex].FrameBuf[mRecvBuf[mBTRecvIndex].FrameLen] = buf;
				mRecvBuf[mBTRecvIndex].FrameLen++;	
				mBTNeedRecv--;		
				if(mBTNeedRecv == 0 || mRecvBuf[mBTRecvIndex].FrameLen >= BT_FRAME_MAX)
				{
				    mRecvBuf[mBTRecvIndex].FrameType = BT_FRAME_TYPE_DATA;
                    mRecvBuf[mBTRecvIndex].FrameState = BT_FRAME_STATE_FINISH;
                    BTSwitchRecvBuf();
				}
			}
        }
    
        
    }

    return ret;
}

/*************************************************
  Function:		hw_bluetooth_uart_end_proc
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
int hw_bluetooth_uart_end_proc(uint8_t buf)
{
    if(-1 != mBTSendIndex)
    {
        mSendBuf[mBTSendIndex].FrameState = BT_FRAME_STATE_NONE;
        mBTSendIndex = -1;
    }
    
    return 0;
}

/*************************************************
 Function:		hw_bluetooth_time_proc
 Descroption:	 
 Input: 
	1.Param
 Output: 
 Return: 	
 Other:  
*************************************************/
static int hw_bluetooth_time_proc(void* Param)
{
    if(mRecvBuf[mBTRecvIndex].FrameTime != -1)
    {
        mRecvBuf[mBTRecvIndex].FrameTime++;      
        if(mRecvBuf[mBTRecvIndex].FrameTime > 5)
        {
           mRecvBuf[mBTRecvIndex].FrameTime = -1;
           mRecvBuf[mBTRecvIndex].FrameState = BT_FRAME_STATE_FINISH;
           BTSwitchRecvBuf();
        }
    }
    
    return 0;
}

/*************************************************
 Function:		hw_bluetooth_sendcmd
 Descroption:	 
 Input: 
	1.CMD
	2.setCMD
	3.setBuf
	4.setBufLen
 Output: 
 Return: 	
 Other:  
*************************************************/
int hw_bluetooth_sendcmd(int CMD, uint8_t setCMD, uint8_t * setBuf, uint8_t setBufLen)
{
    int i, len = 0;
    int ret = -1;
    int Index = BTGetSendBufIndex(BT_FRAME_FIND_EMPTY);
   
    if(-1 != Index  && BT_FRAME_STATE_NONE == mSendBuf[Index].FrameState)
    {
        for (i = 0 ; i < BT_FRAME_MAX; i++)
        {
            mSendBuf[Index].FrameBuf[i] = 0;
        }

        if(setCMD)
        {
            for (i = 0 ; i < mBTCMD[CMD].CmdLen; i++)
            {
                mSendBuf[Index].FrameBuf[i] = mBTCMD[CMD].Cmd[i];
                len++;
            }
            for (i = 0 ; i < setBufLen; i++)
            {
                mSendBuf[Index].FrameBuf[len++] = setBuf[i];
            }
            
            mSendBuf[Index].FrameBuf[len++] = '\r';
            mSendBuf[Index].FrameBuf[len++] = '\n';
            mSendBuf[Index].FrameLen = len;
        }
        else
        {
            for (i = 0 ; i < mBTCMD[CMD].CmdLen; i++)
            {
                mSendBuf[Index].FrameBuf[i] = mBTCMD[CMD].Cmd[i];
            }
            
            mSendBuf[Index].FrameBuf[i] = '\r';
            mSendBuf[Index].FrameBuf[i+1] = '\n';
            mSendBuf[Index].FrameLen = mBTCMD[CMD].CmdLen+2;
        }
        mSendBuf[Index].FrameType = BT_FRAME_TYPE_CMD;
        mSendBuf[Index].FrameState = BT_FRAME_STATE_SEND;
        ret = 0;
    }
    return ret;
}

/*************************************************
  Function:		hw_bluetooth_cmd_dt
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_bluetooth_cmd_dt(uint8_t *buf, uint8_t len)
{
    uint8_t first = buf[0];
    uint8_t second = buf[1];
    switch (first)
    {
        case 'O':
            if('K' == second)
            {
            }
            break;

        case 'E':
            LOGD((const char*)buf); 
            break;
        
        case '+':
            switch(second)
            {
                case 'L':
                    if(buf[2] == 'A')
                    {
                        LOGD("BT ADDR: ");
                        LOGD((const char*)&buf[7]);    
                    }
                    break;
                    
                case 'E':
                    LOGD((const char*)buf); 
                    break;
            
                case 'A':
                    if(buf[2] == 'U' && buf[3] == 'T')
                    {
                        LOGD("BT NAME: ");
                        LOGD((const char*)&buf[6]);                            
                    }
                    else
                    {
                        LOGD((const char*)buf); 
                    }
                    break;
            
                case 'N':
                    LOGD("BT NAME ");
                    LOGD((const char*)&buf[6]);  
                    break;

                case 'P':
                    if (buf[2] == 'I' && buf[3] == 'N')
                    {
                        LOGD("BT PIN ");
                        LOGD((const char*)&buf[5]);
                    }
                    else if(buf[2] == 'A' && buf[3] == 'I')
                    {
                       mBTState = BT_PAIRABLE;
                       LOGD("BT PAIRABLE\r\n");  
                    }
                    break;

                case 'R':
                    if('E' == buf[2]  && buf[3] == 'A')
                    {
                        mBTState = BT_READY;
                        LOGD("BT READY\r\n");  
                    }
                    else if ('N' == buf[2] && 'A' == buf[3])
                    {
                        LOGD("BT REMOTE NAME ");  
                        LOGD((const char*)&buf[6]);    
                    }
                    else    
                    {
                        LOGD("BT ROLE ");
                        LOGD((const char*)&buf[6]);    
                    }
                    break;

                case 'C':
                    if(buf[8] == 'E')
                    {
                        mBTState = BT_CONNECTED;
                        LOGD("BT CONNECTED\r\n"); 
                    }
                    else if(buf[8] == 'I' && buf[9] == 'O')
                    {
                        mBTState = BT_READY;
                        LOGD("BT CONNECT Fail\r\n"); 
                    }
                    else if(buf[8] == 'I' && buf[9] == 'N')
                    {
                         mBTState = BT_CONNECTING;
                        LOGD("BT CONNECTING ");
                        LOGD((const char*)&buf[11]); 
                    }
                    break;

                case 'S':
                    switch(buf[7])
                    {
                        case '0':
                            mBTState = BT_INITIALIZING;
                            LOGD("BT INITIALIZING\r\n"); 
                            break;

                        case '1':
                            mBTState = BT_READY;
                            LOGD("BT READY\r\n"); 
                            break;
                        
                        case '2':
                            mBTState = BT_INQUIRING;
                            LOGD("BT INQUIRING\r\n"); 
                            break;
                        
                        case '3':
                            mBTState = BT_PAIRABLE;
                            LOGD("BT PAIRABLE\r\n"); 
                            break;    

                        case '4':
                            mBTState = BT_CONNECTING;
                            LOGD("BT CONNECTING\r\n"); 
                            break;
                        
                        case '5':
                            mBTState = BT_CONNECTED;
                            LOGD("BT CONNECTED\r\n"); 
                            break;    

                        default:
                            mBTState = BT_INITIALIZING;
                            LOGD("BT INITIALIZING\r\n"); 
                            break;
                    }
                    break;    


                 case 'D':
                    LOGD("BT %s",(const char*)&buf[6]); 
                    mBTState = BT_READY;
                    break;
            }
            break;

            default:
                break;
    }
}

/*************************************************
 Function:		hw_bluetooth_proc
 Descroption:	  
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
void hw_bluetooth_proc(void)
{
    uint8_t i;
    int index = BTGetRecvBufIndex(BT_FRAME_FIND_FULL);
    
    if(-1 != index && mRecvBuf[index].FrameState == BT_FRAME_STATE_FINISH)
    {             
        if(BT_FRAME_TYPE_CMD == mRecvBuf[index].FrameType)
        {
            mBTRecvACK = 0;
            hw_bluetooth_cmd_dt(mRecvBuf[index].FrameBuf,mRecvBuf[index].FrameLen);  
            for (i = 0 ; i < BT_FRAME_MAX; i++)
            {
                mRecvBuf[index].FrameBuf[i] = 0;
            }
            mRecvBuf[index].FrameTime = -1;
            mRecvBuf[index].FrameState = BT_FRAME_STATE_NONE;
        }
    }

    if(mInitCount < BT_INITCMD_MAX && -1 != mInitCount && -1 == mBTSendIndex && 0 == mBTRecvACK)
    {
        i = mInitCount;
        if(0 == hw_bluetooth_sendcmd(mInitCMD[i].sendCMD, mInitCMD[i].setCMD, mInitCMD[i].Cmd, mInitCMD[i].CmdLen))
        {
            mBTRecvACK = 1;
            mInitCount++;         
        }  
    }

    if(-1 == mBTSendIndex)
    {
        index = BTGetSendBufIndex(BT_FRAME_FIND_FULL);
        
        if(-1 != index && mSendBuf[index].FrameState == BT_FRAME_STATE_SEND)
        {
            mBTSendIndex = index;    
            mSendBuf[index].FrameState = BT_FRAME_STATE_SENDING;            
            hw_serial_send_bluetooth((uint32_t)mSendBuf[index].FrameBuf, mSendBuf[index].FrameLen);
        }
    }
}

/*************************************************
 Function:		hw_bluetooth_set_pin
 Descroption:	 
 Input: 
	1.* setBuf
	2.setBufLen
 Output: 
 Return: 	
 Other:  
*************************************************/
void hw_bluetooth_set_pin(uint8_t * setBuf, uint8_t setBufLen)
{
    hw_bluetooth_sendcmd(BT_CMD_PIN, 1, setBuf, setBufLen);
}

/*************************************************
 Function:		hw_bluetooth_set_name
 Descroption:	 
 Input: 
	1.* setBuf
	2.setBufLen
 Output: 
 Return: 	
 Other:  
*************************************************/
void hw_bluetooth_set_name(uint8_t * setBuf, uint8_t setBufLen)
{
    hw_bluetooth_sendcmd(BT_CMD_NAME, 1, setBuf, setBufLen);
}

/*************************************************
 Function:		hw_bluetooth_send_buf
 Descroption:	 
 Input: 
	1.* setBuf
	2.setBufLen
 Output: 
 Return: 	
 Other:  
*************************************************/
void hw_bluetooth_send_buf(uint8_t * setBuf, uint8_t setBufLen)
{
    uint8_t i;
    int Index = -1;

    if(mBTState != BT_CONNECTED)
    {
        LOGD("mBTState Error [%d]\r\n",mBTState);
        return;
    }

    Index = BTGetSendBufIndex(BT_FRAME_FIND_EMPTY); 
    if(-1 != Index  && BT_FRAME_STATE_NONE == mSendBuf[Index].FrameState)
    {
        if(setBufLen > BT_FRAME_MAX)
        {
            setBufLen = BT_FRAME_MAX;
        }
    
        for (i = 0 ; i < setBufLen; i++)
        {
            mSendBuf[Index].FrameBuf[i] = setBuf[i];
        }

        mSendBuf[Index].FrameLen = setBufLen;
        mSendBuf[Index].FrameType = BT_FRAME_TYPE_DATA;
        mSendBuf[Index].FrameState = BT_FRAME_STATE_SEND;
    }
}

/*************************************************
 Function:		hw_bluetooth_get_frame
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
PBT_FRAME hw_bluetooth_get_frame(void)
{
    int Index = BTGetRecvBufIndex(BT_FRAME_FIND_FULL);
    if(Index != -1 && mRecvBuf[Index].FrameType == BT_FRAME_TYPE_DATA)
    {
        mRecvBuf[Index].FrameIndex = Index;
        return &mRecvBuf[Index];
    }
    else
    {
        return NULL;
    }
}

/*************************************************
 Function:		hw_bluetooth_free_frame
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
void hw_bluetooth_free_frame(int Index)
{
    mRecvBuf[Index].FrameState = BT_FRAME_STATE_NONE;
    mRecvBuf[Index].FrameTime = -1;
    mRecvBuf[Index].FrameType = BT_FRAME_TYPE_NONE;
    mRecvBuf[Index].FrameLen = 0;
}

/*************************************************
  Function:		hw_bluetooth_sync
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_bluetooth_sync(void)
{
    mInitCount = 0;
}

/*************************************************
  Function:		hw_bluetooth_init
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_bluetooth_init(void)
{
    uint8_t i;
    hw_ioctrl_bt_reset();
    for (i = 0 ; i < BT_RECV_MAX; i++)
    {
        mRecvBuf[i].FrameState = BT_FRAME_STATE_NONE;
        mRecvBuf[i].FrameTime = -1;
    }
    for (i = 0 ; i < BT_SEND_MAX; i++)
    {
        mSendBuf[i].FrameState = BT_FRAME_STATE_NONE;
        mSendBuf[i].FrameTime = -1;
    }
    mBTState = BT_INITIALIZING;
    hw_serial_set_proc(USART_BLUETOOTH, hw_bluetooth_uart_proc, hw_bluetooth_uart_end_proc);
    hw_timer_set_proc(TIME_10MS, hw_bluetooth_time_proc, NULL);
}

