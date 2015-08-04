/*********************************************************
 Copyright (C),2011-2017,Electronic Technology Co.,Ltd.
 File name: 		logic_experiment.c
 Author: 			Txl
 Version: 			1.0
 Date: 				2014-12-27
 Description: 		
 History: 			
 					
   1.Date:	 		2014-12-27
 	 Author:	 	Txl
 	 Modification:  Created file
 	 
*********************************************************/

#include "logic.h"

#define _DEBUG_DATA_             1

#if(LEVEL_DEBUG && LEVEL_EXPER)
#define LOGD        hw_serial_printf
#else
#define LOGD        hw_serial_printf_null  
#endif

static ExpInfo mExpInfo;

#ifdef _DEBUG_DATA_
float mDebugSensor[MAX_SENSOR] = {0.0,0.0,0.0,0.0}; 
float mDebugPressure = 0.0;

/*************************************************
 Function:		logic_experiment_get_echo
 Descroption:	 
 Input: 
 Output: 
 Return: 	
 Other:  
*************************************************/
void logic_experiment_set_debug(float* Sensor, float Pressure)
{
    mDebugPressure = Pressure;
    mDebugSensor[0] = Sensor[0];
    mDebugSensor[1] = Sensor[1];
    mDebugSensor[2] = Sensor[2];
    mDebugSensor[3] = Sensor[3];
}
#else
void logic_experiment_set_debug(float* Sensor, float Pressure)
{
}
#endif

/*************************************************
 Function:		get_avg_diff
 Descroption:	获取差值
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
float get_avg_diff(float Value1, float Value2)
{
    if(Value1 >= Value2)
    {
        return (Value1-Value2);
    }
    else
    {
        return (Value2-Value1);        
    }
}

/*************************************************
 Function:		logic_experiment_get_state
 Descroption:	 
 Input: 
 Output: 
 Return: 	
 Other:  
*************************************************/
static WORK_STATE logic_experiment_get_state(void)
{
    return mExpInfo.mState;
}

/*************************************************
 Function:		logic_experiment_set_state
 Descroption:	 
 Input: 
    1.state
 Output: 
 Return: 	
 Other:  
*************************************************/
static void logic_experiment_set_state(WORK_STATE state)
{
    mExpInfo.mState = state;
}

/*************************************************
 Function:		logic_experiment_get_echo
 Descroption:	 
 Input: 
 Output: 
 Return: 	
 Other:  
*************************************************/
static ERROR_STATE logic_experiment_get_echo(void)
{
    return mExpInfo.Echo;
}

/*************************************************
 Function:		logic_experiment_set_echo
 Descroption:	 
 Input: 
    1.state
 Output: 
 Return: 	
 Other:  
*************************************************/
static void logic_experiment_set_echo(ERROR_STATE state)
{
    mExpInfo.Echo = state;
}

/*************************************************
 Function:		logic_experiment_read_pressure_hw
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
static float logic_experiment_read_pressure_hw(void)
{
    #ifdef _DEBUG_DATA_
    return mDebugPressure;
    #endif
}

/*************************************************
 Function:		logic_experiment_read_drift_hw
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
static float logic_experiment_read_drift_hw(uint8_t Index)
{
    #ifdef _DEBUG_DATA_
    return mDebugSensor[Index];
    #else
    return logic_sensor_read_drift(Index);
    #endif
}

/*************************************************
 Function:		logic_experiment_add_pressure_hw
 Descroption:	硬件加压接口
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
static void logic_experiment_add_pressure_hw(void)
{
    if(mExpInfo.mPressure != PRESSURE_LOAD)
    {
        mExpInfo.mPressure = PRESSURE_LOAD;
        LOGD("加压中.........\r\n");
    }
}

/*************************************************
 Function:		logic_experiment_del_pressure_hw
 Descroption:	硬件减压接口 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
static void logic_experiment_del_pressure_hw(void)
{
    if(mExpInfo.mPressure != PRESSURE_UNLOAD)
    {
        mExpInfo.mPressure = PRESSURE_UNLOAD;
        LOGD("卸压中.........\r\n");
    }
}

/*************************************************
 Function:		logic_experiment_add_min
 Descroption:	 
 Input: 
    1.timer
	2.min
 Output: 
 Return: 	
 Other:  
*************************************************/
static void logic_experiment_add_min(PZONE_DATE_TIME timer,uint8_t min)
{
    uint16_t Day;
    if(NULL == timer)
    {
        return;
    }

    timer->min += min;
    if(timer->min >= 60)
    {
       timer->hour++;
       if(timer->hour >= 24)
       {
            timer->hour = 0;
            timer->day++;
            Day = MonthDays(timer->year, timer->month);    
            if(timer->day > Day)
            {
                timer->day = 1;
                timer->month++;
                if(timer->month > 12)
                {
                    timer->month  = 1;
                    timer->year++;
                }
            }
       }
    }

}

/*************************************************
 Function:		logic_experiment_printf
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
void logic_experiment_printf(void)
{
    int i;
    LOGD("\r\n");    
    LOGD("************************************************\r\n");  
    LOGD("*规范       : %d\r\n",mExpInfo.mStandard);
    LOGD("*当前控载值 : %d\r\n",mExpInfo.NowValue);
    LOGD("*补载下限   : %f\r\n",mExpInfo.SupplyValue);
    LOGD("*单级加荷值 : %d\r\n",mExpInfo.OneValue);
    LOGD("*最大加荷值 : %d\r\n",mExpInfo.MaxValue);
    LOGD("*加载方式   : %d\r\n",mExpInfo.Mode);
    LOGD("*维荷时间   : %d\r\n",mExpInfo.HoldTime);
    LOGD("*判稳时间   : %d\r\n",mExpInfo.TestTime);
    LOGD("*判稳次数   : %d\r\n",mExpInfo.HoldCount);
    LOGD("*稳定沉降   : %f\r\n",mExpInfo.HoldDown);
    LOGD("*卸载级数   : %d\r\n",mExpInfo.ULoadLevel);
    LOGD("*非0测试    : %d\r\n",mExpInfo.NoZeroTime);
    LOGD("*0测试      : %d\r\n",mExpInfo.ZeroTime);
    LOGD("*加载间隔   : ");
    for (i = 0 ; i < LOAD_TIME; i++)
    {
        LOGD("[%02d] ",mExpInfo.LoadTime[i]);
    }
    LOGD("\r\n");
    
    LOGD("*卸载间隔   : ");
    for (i = 0 ; i < LOAD_TIME; i++)
    {
        LOGD("[%02d] ",mExpInfo.UnLoadTime[i]);
    }
    LOGD("\r\n");
    LOGD("************************************************\r\n");    
}

/*************************************************
 Function:		logic_experiment_init_param
 Descroption:	 
 Input: 
    1.nExpInfo
	2.mStandard
 Output: 
 Return: 	
 Other:  
*************************************************/
static void logic_experiment_init_param(PExpInfo nExpInfo,PileStandard mStandard)
{
    switch(mStandard)
    {
    
        case STANDARD_NONE:
            
            break;
               
        case STANDARD_2014F:
            
            break;
        
        case STANDARD_2014S:
            nExpInfo->mSensorNum    = 4;
			nExpInfo->mSensor[0]    = 1;
			nExpInfo->mSensor[1]    = 1;
			nExpInfo->mSensor[2]    = 1;			
			nExpInfo->mSensor[3]    = 1;
            nExpInfo->NowValue      = 0;
            nExpInfo->TargetValue   = 44;
            nExpInfo->OneValue      = 22;              
            nExpInfo->MaxValue      = 220;   
            nExpInfo->SupplyValue   = (nExpInfo->OneValue/10); 
            nExpInfo->Mode          = MODE_AUTO;        
            nExpInfo->HoldTime      = 120;             
            nExpInfo->TestTime      = 60;              
            nExpInfo->HoldCount     = 2;               
            nExpInfo->mStandard     = STANDARD_2014S;  
            nExpInfo->HoldDown      = 0.1;             
            nExpInfo->LoadTime[0]   = 5;               
            nExpInfo->LoadTime[1]   = 10;              
            nExpInfo->LoadTime[2]   = 15;              
            nExpInfo->LoadTime[3]   = 15;              
            nExpInfo->LoadTime[4]   = 15;              
            nExpInfo->LoadTime[5]   = 30;              
            nExpInfo->UnLoadTime[0] = 15;              
            nExpInfo->UnLoadTime[1] = 15;                  
            nExpInfo->UnLoadTime[2] = 30;              
            nExpInfo->UnLoadTime[3] = 30;              
            nExpInfo->UnLoadTime[4] = 30;                  
            nExpInfo->UnLoadTime[5] = 30;              
            nExpInfo->ULoadLevel    = 2;               
            nExpInfo->NoZeroTime    = 60;              
            nExpInfo->ZeroTime      = 180;   
            nExpInfo->CheckLoadTime = 30;
            break;
               
        case STANDARD_FAST1:
            
            break;
        
        case STANDARD_FAST2:
            
            break;
        
        default:
            
            break;
    }

    nExpInfo->mState        = WORK_STATE_OFF; 
    nExpInfo->AllLoadTime   = 0;
    nExpInfo->LoadLevel     = 0;
    nExpInfo->Echo          = ERROR_NONE;
    nExpInfo->mSaveSettling = 0;
    nExpInfo->HoldNowCount  = 0;
    nExpInfo->CheckTime     = 0;
}

/*************************************************
 Function:		logic_experiment_timer
 Descroption:	 
 Input: 
	1.param
 Output: 
 Return: 	
 Other:  
*************************************************/
static void logic_experiment_timer(void* param)
{
     mExpInfo.mSaveSettling = 1; 
}

/*************************************************
 Function:		logic_experiment_read_settling
 Descroption:	读取沉降
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
static void logic_experiment_read_settling(void)
{
    uint8_t i;
    for (i = 0 ; i < mExpInfo.mSensorNum; i++)
    {
       mExpInfo.mSensorNow[i] = logic_experiment_read_drift_hw(i);
    }
}

/*************************************************
 Function:		logic_experiment_read_settling_start
 Descroption:	读取沉降
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
static void logic_experiment_read_settling_start(void)
{
    logic_experiment_add_min(&mExpInfo.NowTimer, mExpInfo.LoadTime[mExpInfo.LoadLevel]);
    hw_rtc_set_rtc_alarm(&mExpInfo.NowTimer, logic_experiment_timer, (void*)&mExpInfo);   
}

/*************************************************
 Function:		logic_experiment_check_stable_standard_2014s
 Descroption:	判稳
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
static bool logic_experiment_check_stable_standard_2014s(void)
{
    return FALSE;
}

/*************************************************
 Function:		logic_experiment_check_stable
 Descroption:	判稳
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
static void logic_experiment_check_stable(void)
{
    bool ret = FALSE;
    
    switch(mExpInfo.mStandard)
    {
    
        case STANDARD_NONE:
            
            break;
        
        case STANDARD_2014F:
            
            break;
        
        case STANDARD_2014S:
            ret = logic_experiment_check_stable_standard_2014s();
            break;
        
        case STANDARD_FAST1:
            
            break;
        
        case STANDARD_FAST2:
            
            break;
        
        default:
            
            break;
    }

    if(ret)
    {
        
    }
        
}

/*************************************************
 Function:		logic_experiment_save_settling
 Descroption:	保存
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
static void logic_experiment_save_settling(void)
{
    storage_add_experiment(&mExpInfo.NowTimer, mExpInfo.mSensorNow, mExpInfo.NowValue);
}

/*************************************************
 Function:		logic_experiment_save_settling_proc
 Descroption:	读取压力值 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
static void logic_experiment_save_settling_proc(void)
{
    if(ERROR_NONE == logic_experiment_get_echo() && mExpInfo.mSaveSettling)
    {    
        // 为下一次存储准备时间
        if(mExpInfo.LoadLevel >= LOAD_TIME)
        {
           mExpInfo.LoadLevel = (LOAD_TIME-1);
        }
        mExpInfo.AllLoadTime += mExpInfo.LoadTime[mExpInfo.LoadLevel];
        hw_rtc_get_rtc_time(&mExpInfo.NowTimer);
        // 存储当前值
        logic_experiment_save_settling();
        logic_experiment_add_min(&mExpInfo.NowTimer, mExpInfo.LoadTime[mExpInfo.LoadLevel]);
        hw_rtc_set_rtc_alarm(&mExpInfo.NowTimer, logic_experiment_timer, (void*)&mExpInfo);   
        if(mExpInfo.AllLoadTime > mExpInfo.CheckLoadTime)
        {
            logic_experiment_check_stable();
        }
    }
    mExpInfo.mSaveSettling = 0;
}

/*************************************************
 Function:		logic_experiment_check_max_settling
 Descroption:	检测最大沉降量
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
static void logic_experiment_check_max_settling(void)
{
    uint8_t i; 
    float Value[MAX_SENSOR];
    for (i = 0 ; i < mExpInfo.mSensorNum; i++)
    {
        Value[i] = get_avg_diff(mExpInfo.mSensorNow[i], mExpInfo.mSensorStart[i]);
        if(Value[i] >= mExpInfo.MaxSettle)
        {
            logic_experiment_set_echo(ERROR_MAX_SETTLE);
            break;
        }
    }    
}

/*************************************************
 Function:		logic_experiment_check_avg_settling
 Descroption:	检测不均匀沉降量
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
static void logic_experiment_check_avg_settling(void)
{
    uint8_t i; 
    float Value[MAX_SENSOR];
    float Check = 0;

    PEXPERIMENT_PARAM Param = storage_get_experiment(0);
    if(NULL == Param)
    {
        return;
    }
    
    for (i = 0 ; i < mExpInfo.mSensorNum; i++)
    {
        Value[i] = get_avg_diff(mExpInfo.mSensorNow[i], Param->mSensor[i]); 
    }    

    if(mExpInfo.mSensorNum == 4)
    {
        Check = get_avg_diff(Value[3], Value[2]);    
        if(Check >= mExpInfo.UniformSettle)
        {
            logic_experiment_set_echo(ERROR_AVG_SETTLE);
            return;
        }

        Check = get_avg_diff(Value[3], Value[1]);    
        if(Check >= mExpInfo.UniformSettle)
        {
            logic_experiment_set_echo(ERROR_AVG_SETTLE);
            return;
        }

        Check = get_avg_diff(Value[3], Value[0]);    
        if(Check >= mExpInfo.UniformSettle)
        {
            logic_experiment_set_echo(ERROR_AVG_SETTLE);
            return;
        }
    }
    
    if(mExpInfo.mSensorNum == 3)
    {
        Check = get_avg_diff(Value[2], Value[1]);    
        if(Check >= mExpInfo.UniformSettle)
        {
            logic_experiment_set_echo(ERROR_AVG_SETTLE);
            return;
        }

        Check = get_avg_diff(Value[2], Value[0]);    
        if(Check >= mExpInfo.UniformSettle)
        {
            logic_experiment_set_echo(ERROR_AVG_SETTLE);
            return;
        }
    }

    Check = get_avg_diff(Value[1], Value[0]);    
    if(Check >= mExpInfo.UniformSettle)
    {
        logic_experiment_set_echo(ERROR_AVG_SETTLE);
        return;
    }

    
}

/*************************************************
 Function:		logic_experiment_check_settling_proc
 Descroption:	判稳操作
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
static void logic_experiment_check_settling_proc(void)
{
    logic_experiment_read_settling();
    logic_experiment_check_max_settling();
    logic_experiment_check_avg_settling();
}

/*************************************************
 Function:		logic_experiment_check_loadprotect
 Descroption:	加载保护时间 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
static void logic_experiment_check_loadprotect(void)
{

}

/*************************************************
 Function:		logic_experiment_check_pressure_proc
 Descroption:	判稳操作
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
static void logic_experiment_check_pressure_proc(void)
{
    mExpInfo.NowValue = logic_experiment_read_pressure_hw();
    if(mExpInfo.NowValue < (mExpInfo.TargetValue-mExpInfo.SupplyValue))
    {
        logic_experiment_add_pressure_hw();
    }
    else if(mExpInfo.NowValue > (mExpInfo.TargetValue+mExpInfo.SupplyValue))
    {
        logic_experiment_del_pressure_hw();
    }
    logic_experiment_check_loadprotect();
}

/*************************************************
 Function:		logic_experiment_load_proc
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
static void logic_experiment_load_proc(void)
{
    logic_experiment_check_settling_proc();
    logic_experiment_save_settling_proc();
    logic_experiment_check_pressure_proc();
}

/*************************************************
 Function:		logic_experiment_unload_proc
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
static void logic_experiment_unload_proc(void)
{
    
}

/*************************************************
 Function:		logic_experiment_set_standard
 Descroption:	 
 Input: 
	1.Data
	2.len
 Output: 
 Return: 	
 Other:  
*************************************************/
void logic_experiment_set_standard(uint8_t* data, uint8_t len)
{
    float param;
    
    mExpInfo.NowValue      = 0;
    mExpInfo.TargetValue   = data[0];
    mExpInfo.OneValue      = data[0]/2;              
    mExpInfo.MaxValue      = mExpInfo.OneValue*10;   
    mExpInfo.SupplyValue   = ((mExpInfo.OneValue*data[6])/100); 
    mExpInfo.Mode          = (LOADMODE)data[1];        
    mExpInfo.HoldTime      = data[2];             
    mExpInfo.TestTime      = data[3];              
    mExpInfo.HoldCount     = data[4];               
    mExpInfo.mStandard     = (PileStandard)data[5];            
    param = (data[7]*256+data[8]);
	mExpInfo.HoldDown = (param/0x7FFF);
}

/*************************************************
 Function:		logic_experiment_set_checktime
 Descroption:	 
 Input: 
	1.Data
	2.len
 Output: 
 Return: 	
 Other:  
*************************************************/
void logic_experiment_set_checktime(uint8_t* data, uint8_t len)
{
    mExpInfo.LoadTime[0]   = data[0];               
    mExpInfo.LoadTime[1]   = data[1];              
    mExpInfo.LoadTime[2]   = data[2];              
    mExpInfo.LoadTime[3]   = data[3];              
    mExpInfo.LoadTime[4]   = data[4];              
    mExpInfo.LoadTime[5]   = data[5];              
    mExpInfo.UnLoadTime[0] = data[6];              
    mExpInfo.UnLoadTime[1] = data[7];                  
    mExpInfo.UnLoadTime[2] = data[8];              
    mExpInfo.UnLoadTime[3] = data[9];              
    mExpInfo.UnLoadTime[4] = data[10];                  
    mExpInfo.UnLoadTime[5] = data[11];              
    mExpInfo.ULoadLevel    = data[12];               
    mExpInfo.NoZeroTime    = data[13];              
    mExpInfo.ZeroTime      = data[14];   

    logic_experiment_printf();
}


/*************************************************
 Function:		logic_experiment_start
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
void logic_experiment_start(int ID)
{
    uint8_t i;
    uint8_t data[4];
    if(WORK_STATE_OFF == logic_experiment_get_state())
    {         
        logic_experiment_init_param(&mExpInfo, STANDARD_2014S);
        hw_rtc_get_rtc_time(&mExpInfo.StartTimer);
        hw_rtc_time_dump(&mExpInfo.StartTimer, &mExpInfo.NowTimer);
        logic_experiment_read_settling_start();        
        for (i = 0 ; i < mExpInfo.mSensorNum; i++)
        {
           mExpInfo.mSensorStart[i] = logic_experiment_read_drift_hw(i);
           mExpInfo.mSensorNow[i] = mExpInfo.mSensorStart[i];
        }
        logic_experiment_set_state(WORK_STATE_LOAD); 
        logic_experiment_printf();
    }

    data[0] = logic_experiment_get_state();    
    logic_ackcommand(COMM_WORK_START, ID, data, 1);
}
/*************************************************
 Function:		logic_experiment_proc
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
void logic_experiment_proc(void)
{
    switch(logic_experiment_get_state())
    {
    
        case WORK_STATE_OFF:
            break;
        
        case WORK_STATE_LOAD:
            logic_experiment_load_proc();
            break;
        
        case WORK_STATE_UNLOAD: 
            logic_experiment_unload_proc();
            break;
        
        default:
            
            break;
    }
}

/*************************************************
 Function:		logic_experiment_init
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
void logic_experiment_init(void)
{

}

