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
}WORK_STATE;                 // ����ģʽ

typedef enum
{
    MODE_AUTO = 0,
    MODE_CONTROL,    
}LOADMODE;                   // ����ģʽ

typedef enum
{
    PRESSURE_NONE,           // �޲���
    PRESSURE_LOAD,           // ��ѹ
    PRESSURE_UNLOAD,         // ��ѹ
}PRESSURE_STATE;             

typedef enum
{
    ERROR_NONE,              // û�д���
    ERROR_LOAD_OVER,         // ���س�ʱ
    ERROR_MAX_SETTLE,        // ����������ֵ
    ERROR_AVG_SETTLE,        // �����ȳ�������ֵ
}ERROR_STATE;                // ����״̬

typedef enum
{
    STANDARD_NONE,           // ���Զ����ȣ�   �������ⳡ���˹����� 
    STANDARD_2014F,          // 2014����
    STANDARD_2014S,          // 2014����
    STANDARD_FAST1,          // ���ټ��ط�ʽ1  �����ȣ�ÿһСʱ��һ��
    STANDARD_FAST2,          // ���ټ��ط�ʽ2  �����ȣ�ÿ��Сʱ��һ��
}PileStandard;

#define LOAD_TIME            6

typedef struct
{
    WORK_STATE mState;
    float TargetValue;      // Ŀ�����ֵ
    int NowValue;           // ��ǰ����ֵ
    float SupplyValue;        // ��������
    int OneValue;           // �����Ӻ�ֵ    
    int MaxValue;           // ���Ӻ�ֵ
    LOADMODE Mode;          // ���ط�ʽ
    uint16_t HoldTime;      // ά��ʱ��
    uint16_t TestTime;      // ����ʱ��
    uint16_t HoldCount;     // ���ȴ���     
    PileStandard mStandard; // �淶
    float   HoldDown;       // �ȶ����� mm

    uint32_t LoadTime[LOAD_TIME]; //���ؼ��
    uint32_t UnLoadTime[LOAD_TIME]; //ж�ؼ��
    uint32_t ULoadLevel;    // ж�ؼ���
    uint32_t NoZeroTime;    // ��0����ʱ��
    uint32_t ZeroTime;      // 0����ʱ��
    
    ZONE_DATE_TIME StartTimer;
    ZONE_DATE_TIME NowTimer;

    uint32_t LoadProtectTimer; // ���ر���ʱ��
    uint32_t MaxSettle;     // ������
    uint32_t UniformSettle; // �����ȳ���
    uint32_t AlarmValue;    // λ�����̱���ֵ

    uint16_t AllLoadTime;   // ������ʱ�� 
    uint16_t CheckLoadTime; // ��ʼ���ʱ��
    uint16_t CheckTime;     // �������ʱ��
    

    ERROR_STATE Echo;       // ����״̬
    PRESSURE_STATE mPressure; // ��ѹ״̬
    uint8_t LoadLevel;      // ���ؼ���  
    uint8_t mSaveSettling;  // ��ʱ�����������

    uint8_t mSensorNum;
	uint8_t mSensor[MAX_SENSOR];  
    float mSensorStart[MAX_SENSOR]; // λ�ƴ�������������ʼ
    float mSensorNow[MAX_SENSOR]; // λ�ƴ�������������ǰ
    uint16_t HoldNowCount;  // ��ǰ���ȴ���   
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
