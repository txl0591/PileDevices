/*********************************************************
 Copyright (C),2011-2017,NewLand Auto-ID Tech.,Ltd.
 File name: 		logic_communicate.c
 Author: 			Tangxl
 Version: 			1.0
 Date: 				2014-10-7
 Description: 		
 History: 			
 					
   1.Date:	 		2014-10-7
 	 Author:	 	Tangxl
 	 Modification:  Created file
 	 
*********************************************************/
#include <string.h>
#include "hw_define.h"
#include "logic_communicate.h"
#include "log.h"

#if(LEVEL_DEBUG & LEVEL_NET)
#define LOGD        hw_serial_printf
#else
#define LOGD        hw_serial_printf_null  
#endif

static uint8_t mUartID = 0;
static FRAME_COM mFrameCom;
static FrameProc LogicDistribute = NULL;
static FrameProc LogicResponsion = NULL;
/*************************************************
 Function:		logic_sendframe_com
 Descroption:	 
 Input: 
	1.Cmd
	2.Send
	3.NeedAck
	4.Id
	5.*Data
	6.Len
 Output: 
 Return: 	
 Other:  
*************************************************/
static void logic_sendframe_com(uint8_t Cmd, uint8_t Send,uint8_t NeedAck, uint8_t Id, uint8_t *Data, uint16_t Len)
{
    uint8_t i,myCRC = 0x00;
    uint8_t mUartSendBuf[50];
    uint8_t BufLen = 0;
	mUartSendBuf[FTRAME_INDEX_START] = FRAME_START;
	mUartSendBuf[FTRAME_INDEX_VERSION] = _FRAMEVERSION_;
	myCRC += mUartSendBuf[FTRAME_INDEX_VERSION];
	mUartSendBuf[FTRAME_INDEX_CTRL] = Send+NeedAck;	
	myCRC += mUartSendBuf[FTRAME_INDEX_CTRL];
	mUartSendBuf[FTRAME_INDEX_DST] = FRAME_ADDR_ANDROID;	
	myCRC += mUartSendBuf[FTRAME_INDEX_DST];
	mUartSendBuf[FTRAME_INDEX_SRC] = FRAME_ADDR_DEVICES;	
	myCRC += mUartSendBuf[FTRAME_INDEX_SRC];
	mUartSendBuf[FTRAME_INDEX_LEN] = Len+3;	
	myCRC += mUartSendBuf[FTRAME_INDEX_LEN];
	mUartSendBuf[FTRAME_INDEX_ID] = Id;	
	myCRC += mUartSendBuf[FTRAME_INDEX_ID];
	mUartSendBuf[FTRAME_INDEX_CMD] = Cmd;
	myCRC += mUartSendBuf[FTRAME_INDEX_CMD];
	
	for (i = 0; i < Len; i++)
	{
		mUartSendBuf[FTRAME_INDEX_DATA+i] = Data[i];
		myCRC += mUartSendBuf[FTRAME_INDEX_DATA+i];
	}

	mUartSendBuf[FTRAME_INDEX_DATA+Len] = myCRC;
	BufLen = 9+Len;
    hw_bluetooth_send_buf(mUartSendBuf, BufLen);
}

/*************************************************
 Function:		logic_communicate_getframe
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
static PFRAME_COM logic_communicate_getframe(void)
{
    int index = -1;
    PBT_FRAME Frame = (PBT_FRAME) hw_bluetooth_get_frame();
    if(NULL != Frame)
    {
        memcpy(mFrameCom.FrameBuf, Frame->FrameBuf, Frame->FrameLen);
        mFrameCom.FrameLen = Frame->FrameLen;
        index = Frame->FrameIndex;
        hw_bluetooth_free_frame(index);
        return &mFrameCom;
    }
    else
    {
        return NULL;
    }
}

/*************************************************
 Function:		logic_sendcommand_ack
 Descroption:	 
 Input: 
	1.Cmd
	2.*Data
	3.Len
 Output: 
 Return: 	
 Other:  
*************************************************/
void logic_sendcommand_ack(uint8_t Cmd, uint8_t *Data, uint8_t Len)
{
    mUartID++;
	logic_sendframe_com(Cmd, FRAME_SEND, FRAME_NEED_ACK, mUartID, Data, Len);
}

/*************************************************
 Function:		logic_sendcommand
 Descroption:	 
 Input: 
	1.Cmd
	2.*Data
	3.Len
 Output: 
 Return: 	
 Other:  
*************************************************/
void logic_sendcommand(uint8_t Cmd, uint8_t *Data, uint8_t Len)
{
	mUartID++;
	logic_sendframe_com(Cmd, FRAME_SEND, FRAME_NONEED_ACK, mUartID, Data, Len);
}

/*************************************************
 Function:		logic_ackcommand
 Descroption:	 
 Input: 
	1.Cmd
	2.Id
	3.*Data
	4.Len
 Output: 
 Return: 	
 Other:  
*************************************************/
void logic_ackcommand(uint8_t Cmd, uint8_t Id, uint8_t *Data, uint8_t Len)
{
	logic_sendframe_com(Cmd, FRAME_ACK, FRAME_NONEED_ACK, Id, Data, Len);
}

/*************************************************
  Function:		logic_communicate_set_proc
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void logic_communicate_set_proc(FrameProc Distribute, FrameProc Responsion)
{
    LogicDistribute = Distribute;
    LogicResponsion = Responsion;
}

/*************************************************
  Function:		logic_communicate_proc
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void logic_communicate_proc(void)
{
	uint8_t FrameType;
	uint8_t FrameAck;
	uint8_t FrameDst;
	uint8_t FrameCmd;
	uint8_t FrameID;
	
	PFRAME_COM Frame = logic_communicate_getframe();
	
	if (NULL != Frame)
	{    
		FrameDst = Frame->FrameBuf[FTRAME_INDEX_DST]; 
		FrameID = Frame->FrameBuf[FTRAME_INDEX_ID];
        
		if(Frame->FrameLen < FRAME_MIN_SIZE)
		{
		    LOGD("FRAME_MIN_SIZE [%d]\r\n",Frame->FrameLen);
		}

		if(FrameDst != FRAME_ADDR_DEVICES)
		{
		    LOGD("FRAME_ADDR_DEVICES [%d]\r\n",FrameDst);
		}
		
		if (Frame->FrameLen >= FRAME_MIN_SIZE && FrameDst == FRAME_ADDR_DEVICES)
		{
			FrameType = Frame->FrameBuf[FTRAME_INDEX_CTRL] & FRAME_ACK;
			FrameAck = Frame->FrameBuf[FTRAME_INDEX_CTRL] & FRAME_NONEED_ACK;
			FrameCmd = Frame->FrameBuf[FTRAME_INDEX_CMD];
            
			if (FrameType == FRAME_SEND)
			{
			    if(NULL != LogicDistribute)
                {
                    LogicDistribute(FrameCmd, FrameAck, FrameID, &Frame->FrameBuf[FTRAME_INDEX_DATA], Frame->FrameBuf[FTRAME_INDEX_LEN]-3);    
                }         
			}
			else
			{
			    if(NULL != LogicResponsion)
                {
                    LogicResponsion(FrameCmd, FrameAck, FrameID, &Frame->FrameBuf[FTRAME_INDEX_DATA], Frame->FrameBuf[FTRAME_INDEX_LEN]-3);    
                }         
			}
		}
		
	}
}
