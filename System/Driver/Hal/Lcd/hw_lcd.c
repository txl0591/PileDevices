/*********************************************************
 Copyright (C),2012-2018,Electronic Technology Co.,Ltd.
 File name: 		hw_lcd.c
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

/*************************************************
 Function:		Lcd_t35_Fill
 Descroption:	 
 Input: 
	1.xStart
	2.yStart
	3.xLong
	4.yLong
	5.Color
 Output: 
 Return: 	
 Other:  
*************************************************/
void Lcd_Fill(uint16_t xStart,uint16_t yStart,uint16_t xLong,uint16_t yLong,uint16_t Color)
{
    #ifdef LCD_T35
    Lcd_t35_Fill(xStart,yStart,xLong,yLong,Color);
    #endif
}

/*************************************************
  Function:		hw_lcd_Init
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_lcd_Init(void)
{
    hw_serial_printf("hw_lcd_Init\r\n");
    #ifdef LCD_T35
    Lcd_t35_init();
    #endif

    hw_backlight_on();
    Lcd_Fill(0, 0, LCD_W, LCD_H, COLOR_RED);
}

