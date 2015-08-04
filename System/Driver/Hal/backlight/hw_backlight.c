/*********************************************************
 Copyright (C),2012-2018,Electronic Technology Co.,Ltd.
 File name: 		hw_backlight.c
 Author: 			Txl
 Version: 			1.0
 Date: 				2015-3-1
 Description: 		
 History: 			
 					
   1.Date:	 		2015-3-1
 	 Author:	 	Txl
 	 Modification:  Created file
 	 
*********************************************************/
#include "hw_define.h"
#include "hw_backlight.h"

/*************************************************
  Function:		hw_backlight_on
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_backlight_on(void)
{
    hw_serial_printf("hw_backlight_on\r\n");
	GPIO_WriteBit(BACKLIGHT_GPIO, BACKLIGHT_PIN, BACKLIGHT_ACTIVE);	
}

/*************************************************
  Function:		hw_ioctrl_bt_reset
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_backlight_off(void)
{
    hw_serial_printf("hw_backlight_off\r\n");
    GPIO_WriteBit(BACKLIGHT_GPIO, BACKLIGHT_PIN, BACKLIGHT_IDLE);
}

/*************************************************
  Function:		hw_backlight_init
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_backlight_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	

	RCC_APB2PeriphClockCmd(BACKLIGHT_GPIO_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = BACKLIGHT_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(BACKLIGHT_GPIO, &GPIO_InitStructure);
}
