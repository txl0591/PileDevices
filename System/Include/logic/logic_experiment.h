/*********************************************************
 Copyright (C),2011-2017,Electronic Technology Co.,Ltd.
 File name: 		logic_experiment.h
 Author: 			Txl
 Version: 			1.0
 Date: 				2014-12-27
 Description: 		
 History: 			
 					
   1.Date:	 		2014-12-27
 	 Author:	 	Txl
 	 Modification:  Created file
 	 
*********************************************************/
#ifndef _LOGIC_EXPERIMENT_H_
#define _LOGIC_EXPERIMENT_H_

#include "Storage.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef enum
{
    WORK_STATE_OFF,
    WORK_STATE_LOAD,    
    WORK_STATE_UNLOAD,
}WORK_STATE;                 // 工作模式

typedef enum
{
    MODE_AUTO = 0,
    MODE_CONTROL,    
}LOADMODE;                   // 加载模式

typedef enum
{
    PRESSURE_NONE,           // 无操作
    PRESSURE_LOAD,           // 加压
    PRESSURE_UNLOAD,         // 减压
}PRESSURE_STATE;             

typedef enum
{
    ERROR_NONE,              // 没有错误
    ERROR_LOAD_OVER,         // 加载超时
    ERROR_MAX_SETTLE,        // 最大沉降允许值
    ERROR_AVG_SETTLE,        // 不均匀沉降允许值
}ERROR_STATE;                // 错误状态

typedef enum
{
    STANDARD_NONE,           // 不自动判稳，   用于特殊场合人工判稳 
    STANDARD_2014F,          // 2014快速
    STANDARD_2014S,          // 2014慢速
    STANDARD_FAST1,          // 快速加载方式1  不判稳，每一小时加一级
    STANDARD_FAST2,          // 快速加载方式2  不判稳，每二小时加一级
}PileStandard;

#define LOAD_TIME            6

typedef struct
{
    WORK_STATE mState;
    float TargetValue;      // 目标控载值
    int NowValue;           // 当前控载值
    float SupplyValue;        // 补载下限
    int OneValue;           // 单级加荷值    
    int MaxValue;           // 最大加荷值
    LOADMODE Mode;          // 加载方式
    uint16_t HoldTime;      // 维荷时间
    uint16_t TestTime;      // 判稳时间
    uint16_t HoldCount;     // 判稳次数     
    PileStandard mStandard; // 规范
    float   HoldDown;       // 稳定沉降 mm

    uint32_t LoadTime[LOAD_TIME]; //加载间隔
    uint32_t UnLoadTime[LOAD_TIME]; //卸载间隔
    uint32_t ULoadLevel;    // 卸载级数
    uint32_t NoZeroTime;    // 非0测试时间
    uint32_t ZeroTime;      // 0测试时间
    
    ZONE_DATE_TIME StartTimer;
    ZONE_DATE_TIME NowTimer;

    uint32_t LoadProtectTimer; // 加载保护时间
    uint32_t MaxSettle;     // 最大沉降
    uint32_t UniformSettle; // 不均匀沉降
    uint32_t AlarmValue;    // 位移量程报警值

    uint16_t AllLoadTime;   // 加载总时间 
    uint16_t CheckLoadTime; // 开始检测时间
    uint16_t CheckTime;     // 检测判稳时间
    

    ERROR_STATE Echo;       // 错误状态
    PRESSURE_STATE mPressure; // 加压状态
    uint8_t LoadLevel;      // 加载级别  
    uint8_t mSaveSettling;  // 定时到，保存沉降

    uint8_t mSensorNum;
	uint8_t mSensor[MAX_SENSOR];  
    float mSensorStart[MAX_SENSOR]; // 位移传感器沉降量开始
    float mSensorNow[MAX_SENSOR]; // 位移传感器沉降量当前
    uint16_t HoldNowCount;  // 当前判稳次数   
}ExpInfo,*PExpInfo;

/*************************************************
 Function:		logic_experiment_start
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
void logic_experiment_start(int ID);

/*************************************************
 Function:		logic_experiment_proc
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
void logic_experiment_proc(void);

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
void logic_experiment_set_standard(uint8_t* data, uint8_t len);

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
void logic_experiment_set_checktime(uint8_t* data, uint8_t len);

/*************************************************
 Function:		logic_experiment_init
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
void logic_experiment_init(void);

/*************************************************
 Function:		logic_sensor_sync_voltage
 Descroption:	 
 Input: 
	1.voltage
 Output: 
 Return: 	
 Other:  
*************************************************/
void logic_sensor_sync_voltage(uint8_t* data,uint8_t len);

/*************************************************
 Function:		logic_sensor_sync_press
 Descroption:	 
 Input: 
	1.voltage
 Output: 
 Return: 	
 Other:  
*************************************************/
void logic_sensor_sync_press(uint8_t* data,uint8_t len);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* ifndef _LOGIC_EXPERIMENT_H_ Edit By Txl 2014-12-27 */
