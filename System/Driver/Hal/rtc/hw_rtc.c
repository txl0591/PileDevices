/*********************************************************
  Copyright (C), 2013-2016
  File name:	hw_rtc.c
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
#include "hw_define.h"
#include "hw_rtc.h"
#include "log.h"
static hw_rtc_alarm_proc g_AlarmProc;
static void* g_AlarmParam = NULL;

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
uint16_t WeekDay(uint16_t AYear, uint16_t AMonth, uint16_t ADay)
{
    uint8_t monthday[] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5};
    uint16_t iDays = (AYear - 1) % 7 + (AYear - 1) / 4 - (AYear - 1) / 100 + (AYear - 1) / 400;
    iDays += monthday[AMonth-1] + ADay;

    if (IsLeapYear(AYear) && AMonth > 2)
    {
    	iDays++;
    }
    
    return iDays % 7;
}

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
uint16_t MonthDays(uint16_t iYear, uint16_t iMonth)
{
    switch (iMonth)
    {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            return 31;
            
        case 4:
        case 6:
        case 9:
        case 11:
            return 30;

        case 2:
            if (IsLeapYear(iYear))
            {
                return 29;
            }
            else
            {
                return 28;
            }

         default:
		 	break;
    }
    return 0;
}

/*************************************************
  Function:		YearDays
  Description: 	获取当前年份的天数
  Input: 		
  	1.iYear		年份
  Output:		无
  Return:		无
  Others:
*************************************************/
uint16_t YearDays(uint16_t iYear)
{
	uint16_t Days = MonthDays(iYear,2)+30*4+31*7;

    return Days;
}

/*************************************************
 Function:		TimerToSec
 Descroption:	时间转秒
 Input: 
	1.timer
 Output: 
 Return: 	
 Other:  
*************************************************/
static uint32_t TimerToSec(PZONE_DATE_TIME timer)
{
	uint32_t TotalSeconds;
    uint16_t nYear;
    uint16_t nMonth;
    uint16_t Days;

    if(NULL == timer)
    {
        return 0;
    }
    
    TotalSeconds = timer->sec;
    nYear = timer->year;
    nMonth = timer->month;
    Days = 0;

    if((nYear < DEFAULT_YEAR) || (nYear > (DEFAULT_YEAR + 30)))
    {
        return 0;
    }
    
    TotalSeconds += timer->min * 60;
    TotalSeconds += timer->hour * 3600;
    TotalSeconds += (timer->day - 1) * SEC_IN_DAY;

    while(nMonth > 1)
    {
        nMonth--;
        Days = MonthDays(timer->year, nMonth);
        TotalSeconds += Days * SEC_IN_DAY;
    }
  
    while(nYear > DEFAULT_YEAR)
    {
        nYear--;
        if (IsLeapYear(nYear))
        {
            Days = 366;
        }
        else
        {
            Days = 365;
        }
        TotalSeconds += Days * SEC_IN_DAY;
    }

    return TotalSeconds;
} 

/*************************************************
 Function:		SecToTimer
 Descroption:	秒转时间
 Input: 
	1.timer
 Input: 
	2.Sec
 Output: 
 Return: 	
 Other:  
*************************************************/
static uint8_t SecToTimer(PZONE_DATE_TIME timer, uint32_t Sec)
{
	uint32_t TotalDays, RemainSeconds;
    uint16_t DaysInYear;
    uint16_t DaysInMonth;

    if (NULL == timer)
    {
        return 0;
    }

    DaysInYear = 365;
    DaysInMonth = 30;
 
    timer->year = DEFAULT_YEAR;
    timer->month = 1;  
    timer->day= 1;

    TotalDays = Sec / SEC_IN_DAY;
    RemainSeconds = Sec % SEC_IN_DAY;

    timer->hour = (uint8_t)(RemainSeconds / 3600);
    timer->min = (uint8_t)((RemainSeconds / 60) % 60);
    timer->sec = (uint8_t)(RemainSeconds % 60);
    
    while(1)
    {
        if (IsLeapYear(timer->year))
        {
            DaysInYear = 366;
        }
        else
        {
            DaysInYear = 365;   
        }
        
        if (TotalDays >= DaysInYear)
        {
            TotalDays -= DaysInYear;
            timer->year++;
        }
        else
        {
            break;
        }
    }

    while(1)
    {
        DaysInMonth = MonthDays(timer->year, timer->month);
        if (TotalDays >= DaysInMonth)
        {
            TotalDays -= DaysInMonth;
            timer->month++;
        }
        else
        {
            break;
        }
    }

    timer->day += TotalDays;
    timer->week = WeekDay(timer->year, timer->month, timer->day);
    
    return 1;
}

/*************************************************
 Function:		hw_rtc_printf
 Descroption:	 
 Input: 
	1.timesrc
 Output: 
 Return: 	
 Other:  
*************************************************/
void hw_rtc_printf(PZONE_DATE_TIME timesrc)
{
    if(NULL == timesrc)
    {
        return;
    }
    #if _DEBUG_
    hw_serial_printf("%04d-%02d-%02d ",timesrc->year,timesrc->month,timesrc->day);
    hw_serial_printf(" %02d:%02d:%02d \r\n",timesrc->hour,timesrc->min,timesrc->sec);
    #endif
}

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
void hw_rtc_time_dump(PZONE_DATE_TIME timesrc, PZONE_DATE_TIME timedst)
{
    if(NULL == timesrc || NULL == timedst)
    {
        return;
    }

    timedst->year = timesrc->year;
    timedst->month = timesrc->month;
    timedst->day = timesrc->day;
    timedst->hour = timesrc->hour;
    timedst->min = timesrc->min;  
    timedst->sec = timesrc->sec;
    timedst->week = timesrc->week;
}
/*************************************************
 Function:		hw_rtc_rtcconfiguration
 Descroption:	Rtc Config
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
static void hw_rtc_rtcconfiguration(void)
{
    /* Enable PWR and BKP clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    /* Allow access to BKP Domain */
    PWR_BackupAccessCmd(ENABLE);

    /* Reset Backup Domain */
    BKP_DeInit();

    /* Enable LSE */
    RCC_LSEConfig(RCC_LSE_ON);

    /* Wait till LSE is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    {}

    /* Select LSE as RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    /* Enable RTC Clock */
    RCC_RTCCLKCmd(ENABLE);

    /* Wait for RTC registers synchronization */
    RTC_WaitForSynchro();

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

    /* Enable the RTC Second */
    RTC_ITConfig(RTC_IT_SEC, ENABLE);
    
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

    /* Set RTC prescaler: set RTC period to 1sec */
    RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
}

/*************************************************
 Function:		hw_rtc_rtc_init
 Descroption:	RTC Init
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
static void hw_rtc_rtc_default(void)
{
    ZONE_DATE_TIME timer;

    timer.year = DEFAULT_YEAR;
    timer.month = 1;
    timer.day = 1;
    timer.hour = 0;
    timer.min= 0;
    timer.sec = 0;
    
    hw_rtc_set_rtc_time(&timer);
}

/*************************************************
 Function:		hw_rtc_rtc_init
 Descroption:	RTC Init
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
static void hw_rtc_rtc_init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure one bit for preemption priority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    /* Enable the RTC Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
    {
        hw_rtc_rtcconfiguration();
        hw_rtc_rtc_default();
        BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
    }
    else
    {
        RTC_WaitForSynchro();
        RTC_ITConfig(RTC_IT_SEC, ENABLE);
        RTC_WaitForLastTask();
    }

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    PWR_BackupAccessCmd(ENABLE);/* Comment: 注意这边的报警脚输出，还没验证 Edit By Tangxl 2014-5-31 */

    RCC_ClearFlag();
}

/*************************************************
 Function:		RTC_IRQHandler
 Descroption:	时钟
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
void RTC_IRQHandler(void)
{
    uint8_t Task = FALSE;
    if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
    {
        RTC_ClearITPendingBit(RTC_IT_SEC);
        Task = TRUE;
    }

    if (RTC_GetITStatus(RTC_IT_ALR) != RESET)
    {
        if(g_AlarmProc)
        {
            g_AlarmProc(g_AlarmParam);
        }
        RTC_ClearITPendingBit(RTC_IT_ALR);
        Task = TRUE;
    }

    if (Task == TRUE)
    {
        RTC_WaitForLastTask();  
    }
    
}

/*************************************************
 Function:		hw_rtc_set_rtc_time
 Descroption:	时间设置
 Input: 
	1.timer
 Output: 
 Return: 	
 Other:  
*************************************************/
uint8_t hw_rtc_set_rtc_time(PZONE_DATE_TIME timer)
{
    uint32_t count = TimerToSec(timer);

    if (count > 0)
    {
        RTC_WaitForLastTask();
        RTC_SetCounter(count);
        RTC_WaitForLastTask();
        return TRUE;
    }

    return FALSE;
}

/*************************************************
 Function:		hw_rtc_get_rtc_time
 Descroption:	获取时钟
 Input: 
	1.timer
 Output: 
 Return: 	
 Other:  
*************************************************/
void hw_rtc_get_rtc_time(PZONE_DATE_TIME timer)
{
    uint32_t Count = RTC_GetCounter();
    SecToTimer(timer,Count);
}   

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
uint8_t hw_rtc_set_rtc_alarm(PZONE_DATE_TIME timer, hw_rtc_alarm_proc proc, void* param)
{
    uint32_t count = TimerToSec(timer);

    if (count > 0)
    {
        g_AlarmProc = proc;
        g_AlarmParam = param;
        RTC_SetAlarm(count);
        RTC_WaitForLastTask();
        RTC_ITConfig(RTC_FLAG_ALR,ENABLE);
        return TRUE;
    }
    g_AlarmProc = NULL;
    return FALSE;
}

/*************************************************
 Function:		hw_rtc_init
 Descroption:	Timer Init
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
void hw_rtc_init(void)
{
    hw_rtc_rtc_init();
}

