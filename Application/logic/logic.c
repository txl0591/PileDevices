/*********************************************************
  Copyright (C), 2013-2016
  File name:	logic.c
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

#include "logic.h"

#if(LEVEL_DEBUG && LEVEL_LOGIC)
#define LOGD        hw_serial_printf
#else
#define LOGD        hw_serial_printf_null  
#endif


/*************************************************
 Function:		HextoDec
 Descroption:	 
 Input: 
	1.*Data
 Output: 
 Return: 	
 Other:  
*************************************************/
uint8_t HextoDec(uint8_t Data)
{
    return (10*(Data>>4)+(Data&0x0F));
    
}

/*************************************************
 Function:		logic_sysnc_time
 Descroption:	 
 Input: 
	1.*Data
 Output: 
 Return: 	
 Other:  
*************************************************/
static void logic_sysnc_time(uint8_t *Data)
{
    ZONE_DATE_TIME Time;
    Time.year  = 2000+HextoDec(Data[0]);
    Time.month = HextoDec(Data[1]);
    Time.day   = HextoDec(Data[2]);   
    Time.hour  = HextoDec(Data[3]);   
    Time.min   = HextoDec(Data[4]);   
    Time.sec   = HextoDec(Data[5]);   
    hw_rtc_set_rtc_time(&Time);
    hw_rtc_printf(&Time);
}

/*************************************************
 Function:		logic_experiment_debug
 Descroption:	 
 Input: 
	1.*Data
 Output: 
 Return: 	
 Other:  
*************************************************/
static void logic_experiment_debug(uint8_t *Data)
{
    float mDebugPressure = 0.0;
    float mDebugSensor[MAX_SENSOR];
    float temp  = Data[0]*256*256*256+Data[1]*256*256+Data[2]*256+Data[3];
    mDebugPressure = temp/0x7FFF;

    temp = Data[4]*256*256+Data[5]*256+Data[6];
    mDebugSensor[0] = temp/0x7FFF;
    temp = Data[7]*256*256+Data[8]*256+Data[9];
    mDebugSensor[1] = temp/0x7FFF;
    temp = Data[10]*256*256+Data[11]*256+Data[12];
    mDebugSensor[2] = temp/0x7FFF;
    temp = Data[13]*256*256+Data[14]*256+Data[15];
    mDebugSensor[3] = temp/0x7FFF;

    LOGD(" 压力 [%f]\r\n",mDebugPressure);
    LOGD(" 位移1 [%f]\r\n",mDebugSensor[0]);
    LOGD(" 位移2 [%f]\r\n",mDebugSensor[1]);
    LOGD(" 位移3 [%f]\r\n",mDebugSensor[2]);
    LOGD(" 位移4 [%f]\r\n",mDebugSensor[3]);
}

/*************************************************
 Function:		LogicDistribute
 Descroption:	 
 Input: 
	1.Cmd
	2.NeedAck
	3.Id
	4.Data
	5.len
 Output: 
 Return: 	
 Other:  
*************************************************/
void LogicDistribute(uint8_t Cmd, uint8_t NeedAck, uint8_t Id, uint8_t* Data, uint8_t len)
{   
    switch(Cmd)
    {
        case COMM_UPDATE:
            logic_sensor_update(Id, Data[0]);
            break;

        case COMM_SYSTIME:
            logic_sysnc_time(Data);
            break;

		case COMM_SYNCSENSOR:
			break;

        case COMM_SYNCSTRANDARD:
            logic_experiment_set_standard(Data, len);
            break;
        
        case COMM_SYNCCHECKTIME:
            logic_experiment_set_checktime(Data, len);
            break;

        case COMM_SYNCPRESS:
            logic_sensor_sync_press(Data, len);
            break;
            
        case COMM_SYNCVOLTAGE:
            logic_sensor_sync_voltage(Data, len);
            break;

        case COMM_GET_VOLTAGE:
            logic_sensor_get_voltage(Id);
            break;
            
        case COMM_DEBUG:
            logic_experiment_debug(Data);
            break;

        case COMM_WORK_START:
            logic_experiment_start(Id);
            break;



        default:
            break;
    }
}

/*************************************************
 Function:		LogicResponsion
 Descroption:	 
 Input: 
	1.Cmd
	2.NeedAck
	3.Id
	4.Data
	5.len
 Output: 
 Return: 	
 Other:  
*************************************************/
void LogicResponsion(uint8_t Cmd, uint8_t NeedAck, uint8_t Id, uint8_t* Data, uint8_t len)
{
    
}

/*************************************************
  Function:		logic_proc
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void logic_proc(void)
{	
    logic_communicate_proc();
    logic_scan_proc();
    //logic_experiment_proc();
}

/*************************************************
  Function:		logic_init
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void logic_init(void)
{
    logic_communicate_set_proc(LogicDistribute, LogicResponsion);
    logic_scan_init();
   // logic_experiment_init();
   
}

