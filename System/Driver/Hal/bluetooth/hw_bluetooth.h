/*********************************************************
 Copyright (C),2011-2017,NewLand Auto-ID Tech.,Ltd.
 File name: 		hw_bluetooth.h
 Author: 			Tangxl
 Version: 			1.0
 Date: 				2014-10-2
 Description: 		
 History: 			
 					
   1.Date:	 		2014-10-2
 	 Author:	 	Tangxl
 	 Modification:  Created file
 	 
*********************************************************/
#ifndef _HW_BLUETOOTH_H_
#define _HW_BLUETOOTH_H_

#include "stm32f10x.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define MAX_BT_LEN          40
#define MAX_BT_CMD          20

#define BAUD_1200           1
#define BAUD_2400           2
#define BAUD_4800           3
#define BAUD_9600           4
#define BAUD_19200          5
#define BAUD_38400          6
#define BAUD_57600          7
#define BAUD_115200         8
#define BAUD_230400         9
#define BAUD_460800         0x0A
#define BAUD_921600         0x0B
#define BAUD_1382400        0x0C

#define STOP_BIT_NONE       0
#define STOP_BIT_1B         1
#define STOP_BIT_2B         2

#define CRC_NONE            0
#define CRC_ODD             1
#define CRC_EVEN            2

#define LOW_POWER_DIS       0
#define LOW_POWER_EN        1

#define BT_INITIALIZING     0
#define BT_READY            1
#define BT_INQUIRING        2
#define BT_PAIRABLE         3
#define BT_CONNECTING       4
#define BT_CONNECTED        5

#define BT_CMD_AT           0
#define BT_CMD_NAME         1
#define BT_CMD_VERSION      2
#define BT_CMD_DEFAULT      3
#define BT_CMD_RESET        4
#define BT_CMD_PIN          5
#define BT_CMD_BAUD         6
#define BT_CMD_UARTMODE     7
#define BT_CMD_LADDR        8
#define BT_CMD_STATE        9
#define BT_CMD_CMODE        10
#define BT_CMD_LOWPOWER     11
#define BT_CMD_AUTOCONN     12
#define BT_CMD_ROLE         13
#define BT_CMD_ENABLEIND    14   
#define BT_CMD_CLEAR        15    
#define BT_CMD_SUPERVERSION 16

#define BT_DEFAULT_NAME     "CoreSoft111"
#define BT_DEFAULT_PIN      "111111"

typedef struct _BT_CMD_
{
    uint8_t sendCMD;
    unsigned char Cmd[MAX_BT_CMD];
    uint8_t CmdLen;
    uint8_t setCMD;
}BT_CMD,*PBT_CMD;


#define BT_FRAME_TYPE_NONE       0
#define BT_FRAME_TYPE_CMD        1
#define BT_FRAME_TYPE_DATA       2
#define BT_FRAME_TYPE_DATA_HEAD  3
#define BT_FRAME_TYPE_DATA_END   4

#define BT_FRAME_STATE_NONE      0
#define BT_FRAME_STATE_RECV      1
#define BT_FRAME_STATE_SEND      1
#define BT_FRAME_STATE_FINISH    2
#define BT_FRAME_STATE_SENDING   2

#define BT_FRAME_FIND_FULL       0
#define BT_FRAME_FIND_EMPTY      1

#define BT_FRAME_MAX             30
typedef struct 
{
    int8_t FrameTime;           
    uint8_t FrameType;
	uint8_t FrameState;
    uint8_t FrameIndex;
	uint8_t FrameLen;
	uint8_t FrameBuf[BT_FRAME_MAX];
}BT_FRAME,*PBT_FRAME;

#define BT_RECV_MAX             3
#define BT_SEND_MAX             2

/*************************************************
  Function:		hw_bluetooth_uart_proc
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
int hw_bluetooth_uart_proc(uint8_t buf);

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
int hw_bluetooth_sendcmd(int CMD, uint8_t setCMD, uint8_t * setBuf, uint8_t setBufLen);

/*************************************************
 Function:		hw_bluetooth_proc
 Descroption:	  
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
void hw_bluetooth_proc(void);

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
void hw_bluetooth_set_pin(uint8_t * setBuf, uint8_t setBufLen);

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
void hw_bluetooth_set_name(uint8_t * setBuf, uint8_t setBufLen);

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
void hw_bluetooth_send_buf(uint8_t * setBuf, uint8_t setBufLen);

/*************************************************
  Function:		hw_bluetooth_sync
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_bluetooth_sync(void);

/*************************************************
 Function:		hw_bluetooth_get_frame
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
PBT_FRAME hw_bluetooth_get_frame(void);

/*************************************************
 Function:		hw_bluetooth_free_frame
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
void hw_bluetooth_free_frame(int Index);

/*************************************************
  Function:		hw_bluetooth_init
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_bluetooth_init(void);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* ifndef _HW_BLUETOOTH_H_ Edit By Tangxl 2014-10-2 */

