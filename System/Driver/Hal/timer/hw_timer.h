/*********************************************************
 Copyright (C),2011-2017,NewLand Auto-ID Tech.,Ltd.
 File name: 		hw_timer.h
 Author: 			Tangxl
 Version: 			1.0
 Date: 				2014-10-5
 Description: 		
 History: 			
 					
   1.Date:	 		2014-10-5
 	 Author:	 	Tangxl
 	 Modification:  Created file
 	 
*********************************************************/
#ifndef _HW_TIMER_H_
#define _HW_TIMER_H_

#include "stm32f10x.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define TIME_NONE           0
#define TIME_1MS            1   // 1MHZ
#define TIME_2MS            2   // 500HZ
#define TIME_5MS            3   // 200HZ
#define TIME_10MS           4   // 100HZ


#define TIME_1MS_PULSE      6000
#define TIME_2MS_PULSE      12000
#define TIME_5MS_PULSE      30000
#define TIME_10MS_PULSE     60000


#define MAX_TIMER_BASE      5

typedef int (*TimeProc)(void* Param); 

typedef struct _TIME_BASE_
{
    uint8_t Id;
    TimeProc Proc;
    void* Param;
}TIME_BASE,*PTIME_BASE;


/*************************************************
  Function:		hw_timer_delay_us
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_timer_delay_us(uint16_t us);

/*************************************************
  Function:		hw_timer_delay_ms
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_timer_delay_ms(uint16_t ms);

/*************************************************
 Function:		hw_timer_set_proc
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
void hw_timer_set_proc(uint8_t type, TimeProc Proc, void* param);

/*************************************************
 Function:		hw_timer_start
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
void hw_timer_start(void);

/*************************************************
 Function:		hw_timer_stop
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
void hw_timer_stop(void);

/*************************************************
 Function:		hw_timer_init
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
void hw_timer_init(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* ifndef _HW_TIMER_H_ Edit By Tangxl 2014-10-5 */
