/*********************************************************
 Copyright (C),2012-2018,Electronic Technology Co.,Ltd.
 File name: 		Lcd_t35.h
 Author: 			Txl
 Version: 			1.0
 Date: 				2015-1-31
 Description: 		
 History: 			
 					
   1.Date:	 		2015-1-31
 	 Author:	 	Txl
 	 Modification:  Created file
 	 
*********************************************************/
#ifndef _LCD_T35_H_
#define _LCD_T35_H_

#include "stm32f10x.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct
{
    unsigned char Addr;
    unsigned char len;
    unsigned char data[12];
    unsigned char delay;
}T35InitCmd;

//#define LCD_RS_GPIO_CLK		 RCC_APB2Periph_GPIOA
//#define LCD_RS_GPIO		 	 GPIOA
//#define LCD_RS_PIN			 GPIO_Pin_7

#define LCD_RS_GPIO_CLK		 RCC_APB2Periph_GPIOA
#define LCD_RS_GPIO		 	 GPIOA
#define LCD_RS_PIN			 GPIO_Pin_3

#define LCD_T35_W            480
#define LCD_T35_H            320

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
void Lcd_t35_Fill(uint16_t xStart,uint16_t yStart,uint16_t xLong,uint16_t yLong,uint16_t Color);

/*************************************************
  Function:		Lcd_t35_init
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void Lcd_t35_init(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* ifndef _LCD_T35_H_ Edit By Txl 2015-1-31 */

