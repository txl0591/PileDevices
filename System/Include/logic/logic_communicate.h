/*********************************************************
 Copyright (C),2011-2017,NewLand Auto-ID Tech.,Ltd.
 File name: 		logic_communicate.h
 Author: 			Tangxl
 Version: 			1.0
 Date: 				2014-10-7
 Description: 		
 History: 			
 					
   1.Date:	 		2014-10-7
 	 Author:	 	Tangxl
 	 Modification:  Created file
 	 
*********************************************************/
#ifndef _LOGIC_COMMUNICATE_H_
#define _LOGIC_COMMUNICATE_H_

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define FRAME_MIN_SIZE				9
#define FRAME_START					0xAA

#define FRAME_ADDR_NONE				0xFF
#define FRAME_ADDR_DEVICES          0x00
#define FRAME_ADDR_PC				0x01
#define FRAME_ADDR_ANDROID			0x02

#define FRAME_SEND					0x00
#define FRAME_ACK					0x80

#define FRAME_NEED_ACK				0x00
#define FRAME_NONEED_ACK			0x01

#define FRAMEBUF_OK					0x02
#define FRAMEBUF_RECV				0x01
#define FRAMEBUF_NONE				0x00

#define FTRAME_INDEX_START 			0x00
#define FTRAME_INDEX_VERSION		0x01
#define FTRAME_INDEX_CTRL 			0x02
#define FTRAME_INDEX_DST 			0x03
#define FTRAME_INDEX_SRC 			0x04
#define FTRAME_INDEX_LEN 			0x05
#define FTRAME_INDEX_ID 			0x06
#define FTRAME_INDEX_CMD 			0x07
#define FTRAME_INDEX_DATA			0x08

#define _FRAMEVERSION_              0x10

typedef enum
{
	COMM_UPDATE     		      = 0x00,
    COMM_SYSTIME                  = 0x01,
    COMM_SYNCSTRANDARD            = 0x02,
    COMM_SYNCSENSOR               = 0x03,
    COMM_SYNCCHECKTIME            = 0x04,
    COMM_SYNCPRESS                = 0x05,
    COMM_SYNCVOLTAGE              = 0x06,
    COMM_GET_VOLTAGE              = 0x07,

    COMM_WORK_START               = 0x10,
    COMM_WORK_GET_STATE           = 0x11,
    COMM_DEBUG                    = 0xF0,    
}COMM_COMMAND;


typedef enum
{
	ECHO_OK 			          = 0x00,
	ECHO_ERR 			          = 0x01,		
}ECHO_COMMAND;


#define FRAME_MAX_LEN               30
typedef struct _FRAME_COM_
{
    uint8_t FrameLen;
    uint8_t FrameBuf[FRAME_MAX_LEN];
}FRAME_COM,*PFRAME_COM;

typedef void (*FrameProc)(uint8_t Cmd, uint8_t NeedAck, uint8_t Id, uint8_t* Data, uint8_t len);

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
void logic_sendcommand_ack(uint8_t Cmd, uint8_t *Data, uint8_t Len);

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
void logic_sendcommand(uint8_t Cmd, uint8_t *Data, uint8_t Len);

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
void logic_ackcommand(uint8_t Cmd, uint8_t Id, uint8_t *Data, uint8_t Len);

/*************************************************
  Function:		logic_communicate_set_proc
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void logic_communicate_set_proc(FrameProc Distribute, FrameProc Responsion);

/*************************************************
  Function:		logic_communicate_proc
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void logic_communicate_proc(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* ifndef _LOGIC_COMMUNICATE_H_ Edit By Tangxl 2014-10-7 */

