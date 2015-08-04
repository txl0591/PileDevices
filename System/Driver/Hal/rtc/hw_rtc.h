/*********************************************************
  Copyright (C), 2013-2016
  File name:	hw_rtc.h
  Author:   	
  Version:   	1.0
  Date: 		
  Description:  
  History:                   
    1. Date:
       Author:
       Modification:
    2. ...
*********************************************************/
#ifndef _HW_RTC_H_
#define _HW_RTC_H_

#include "stm32f10x.h"

#ifdef __cplusplus
 extern "C" {
#endif 

typedef struct _ZONE_DATE_TIME_
{
    uint16_t year;
    uint8_t  month;
    uint8_t  day;
    uint8_t  hour;
    uint8_t  min;   
    uint8_t  sec;
    uint8_t  week;
}ZONE_DATE_TIME,*PZONE_DATE_TIME;

typedef struct _ZONE_DAY_TIME_
{
    uint8_t  hour;
    uint8_t  min;   
}ZONE_DAY_TIME,*PZONE_DAY_TIME;

#define DEFAULT_YEAR        2015
#define SEC_IN_DAY          86400
#define IsLeapYear(AYear)   (((AYear) % 4 == 0) && (((AYear) % 100 != 0) || ((AYear) % 400 == 0)))

typedef void (*hw_rtc_alarm_proc)(void* param); 

/*************************************************
  Function:		WeekDay
  Description: 	特定年月日对应的星期数
  Input: 		
  	1.AYear		年份
  	2.AMonth	月
  	3.ADay		日
  Output:		无
  Return:		无
  Others:
*************************************************/
uint16_t WeekDay(uint16_t AYear, uint16_t AMonth, uint16_t ADay);

/*************************************************
  Function:		MonthDays
  Description: 	获取当前年份月份的天数
  Input: 		
  	1.iYear		年份
  	2.iMonth	月
  Output:		无
  Return:		无
  Others:
*************************************************/
uint16_t MonthDays(uint16_t iYear, uint16_t iMonth);

/*************************************************
  Function:		YearDays
  Description: 	获取当前年份的天数
  Input: 		
  	1.iYear		年份
  Output:		无
  Return:		无
  Others:
*************************************************/
uint16_t YearDays(uint16_t iYear);

/*************************************************
 Function:		RTC_IRQHandler
 Descroption:	时钟
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
void RTC_IRQHandler(void);

/*************************************************
 Function:		hw_rtc_rtc_set
 Descroption:	时间设置
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
uint8_t hw_rtc_set_rtc_time(PZONE_DATE_TIME timer);

/*************************************************
 Function:		hw_rtc_get_rtc_time
 Descroption:	获取时钟
 Input: 
	1.timer
 Output: 
 Return: 	
 Other:  
*************************************************/
void hw_rtc_get_rtc_time(PZONE_DATE_TIME timer);

/*************************************************
 Function:		hw_rtc_set_rtc_alarm
 Descroption:	闹钟设置
 Input: 
	1.timer
	2.proc
	3.param
 Output: 
 Return: 	
 Other:  
*************************************************/
uint8_t hw_rtc_set_rtc_alarm(PZONE_DATE_TIME timer, hw_rtc_alarm_proc proc, void* param);

/*************************************************
 Function:		hw_rtc_time_dump
 Descroption:	 
 Input: 
    1.timer
	2.min
 Output: 
 Return: 	
 Other:  
*************************************************/
void hw_rtc_time_dump(PZONE_DATE_TIME timesrc, PZONE_DATE_TIME timedst);

/*************************************************
 Function:		hw_rtc_printf
 Descroption:	 
 Input: 
	1.timesrc
 Output: 
 Return: 	
 Other:  
*************************************************/
void hw_rtc_printf(PZONE_DATE_TIME timesrc);

/*************************************************
 Function:		hw_rtc_init
 Descroption:	Timer Init
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
void hw_rtc_init(void);

#ifdef __cplusplus
}
#endif

#endif

