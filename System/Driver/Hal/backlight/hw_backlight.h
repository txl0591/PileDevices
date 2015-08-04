/*********************************************************
 Copyright (C),2012-2018,Electronic Technology Co.,Ltd.
 File name: 		hw_backlight.h
 Author: 			Txl
 Version: 			1.0
 Date: 				2015-1-31
 Description: 		
 History: 			
 					
   1.Date:	 		2015-1-31
 	 Author:	 	Txl
 	 Modification:  Created file
 	 
*********************************************************/
#ifndef _HW_BACKLIGHT_H_
#define _HW_BACKLIGHT_H_

#include "stm32f10x.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define BACKLIGHT_ACTIVE		 Bit_SET
#define BACKLIGHT_IDLE		     Bit_RESET

#ifdef LCD_T35
//#define BACKLIGHT_GPIO_CLK		 RCC_APB2Periph_GPIOB
//#define BACKLIGHT_GPIO		 	 GPIOB
//#define BACKLIGHT_PIN			 GPIO_Pin_1

#define BACKLIGHT_GPIO_CLK		 RCC_APB2Periph_GPIOA
#define BACKLIGHT_GPIO		 	 GPIOA
#define BACKLIGHT_PIN			 GPIO_Pin_2
#endif

#define BACKLIGHT_MODE_IO        0
#define BACKLIGHT_MODE_PWM       1

/*************************************************
  Function:		hw_backlight_on
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_backlight_on(void);

/*************************************************
  Function:		hw_ioctrl_bt_reset
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_backlight_off(void);

/*************************************************
  Function:		hw_backlight_init
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_backlight_init(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* ifndef _HW_BACKLIGHT_H_ Edit By Txl 2015-1-31 */

