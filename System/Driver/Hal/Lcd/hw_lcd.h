/*********************************************************
 Copyright (C),2012-2018,Electronic Technology Co.,Ltd.
 File name: 		hw_lcd.h
 Author: 			Txl
 Version: 			1.0
 Date: 				2015-1-31
 Description: 		
 History: 			
 					
   1.Date:	 		2015-1-31
 	 Author:	 	Txl
 	 Modification:  Created file
 	 
*********************************************************/
#ifndef _HW_LCD_H_
#define _HW_LCD_H_

#ifdef LCD_T35
#include "Lcd_t35.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define COLOR_WHITE						0xFFFF
#define COLOR_BLACK						0x0000	  
#define COLOR_BLUE						0x001F  
#define COLOR_BRED						0XF81F
#define COLOR_GRED						0XFFE0
#define COLOR_GBLUE						0X07FF
#define COLOR_RED			    		0xF800
#define COLOR_MAGENTA					0xF81F
#define COLOR_GREEN						0x07E0
#define COLOR_CYAN						0x7FFF
#define COLOR_YELLOW					0xFFE0
#define COLOR_BROWN						0XBC40 //×ØÉ«
#define COLOR_BRRED						0XFC07 //×ØºìÉ«
#define COLOR_GRAY						0X8430 //»ÒÉ«

#ifdef LCD_T35
#define LCD_W                       LCD_T35_W
#define LCD_H                       LCD_T35_H
#endif

/*************************************************
  Function:		hw_lcd_Init
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_lcd_Init(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* ifndef _HW_LCD_H_ Edit By Txl 2015-1-31 */

