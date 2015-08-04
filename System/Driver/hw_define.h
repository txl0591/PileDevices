/*********************************************************
  Copyright (C), 2013-2016
  File name:	hw_define.h
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
#ifndef _HW_DEFINE_H_
#define _HW_DEFINE_H_

#include <stdio.h>

#include "config.h"
#ifdef __cplusplus
extern "C" {
#endif 

#define SYS_CLOCK					 72

/*******************TIME********************/
#define TIME_CLOCK 					(SYS_CLOCK/8) 

/*******************************************/
/*            �궨���ڹ����ж���           */ 
/*******************************************/

/*******************USART*******************/
#define USART_GPRS					 0
#define USART_GPS					 1
#define USART_BLUETOOTH              1    
#define USART_RS232					 2
#define USART_DEBUG                  USART_RS232
#define USART_MAX                    3
/********************************************/

/****************�ж����ȼ�******************
Group_0 0-WDT 15-�����ⲿ�ж�
Group_1 0-�����ж�RX 1-RTC�ж� 2-�����ж�DMA TX
Group_2 0-Timer
Group_3
Group_4
****************�ж����ȼ�******************/

/********************SPI********************/
#include "type.h"
#include "hw_ioctrl.h"
#include "hw_timer.h"
#include "hw_rtc.h"
#include "hw_sensor.h"
#include "hw_wireless.h"
#include "hw_serial.h"
#include "hw_bluetooth.h"
#include "hw_flash.h"  
#include "hw_spi.h"
#include "hw_backlight.h"  
#include "hw_lcd.h"  

#ifdef __cplusplus
}
#endif

#endif

