/*********************************************************
 Copyright (C),2011-2017,Electronic Technology Co.,Ltd.
 File name: 		Storage.h
 Author: 			Txl
 Version: 			1.0
 Date: 				2014-12-28
 Description: 		
 History: 			
 					
   1.Date:	 		2014-12-28
 	 Author:	 	Txl
 	 Modification:  Created file
 	 
*********************************************************/
#ifndef _STORAGE_H_
#define _STORAGE_H_

#include "hw_define.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define BootInfoStart    0x08007800
#define BootInfoEnd      0x08007BFF

#define MAX_SENSOR       4
#define MAX_SENSOR_INFO  50

typedef struct 
{
    ZONE_DAY_TIME Time;
    float mSensor[MAX_SENSOR]; 
    float mPressure;
}EXPERIMENT_PARAM,*PEXPERIMENT_PARAM;

typedef struct 
{
    uint8_t Count;
    EXPERIMENT_PARAM Param[MAX_SENSOR_INFO];
}EXPERIMENT_INFO;

/*************************************************
 Function:		storage_get_experiment
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
PEXPERIMENT_PARAM storage_get_experiment(uint16_t index);

/*************************************************
 Function:		storage_add_experiment
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
void storage_add_experiment(PZONE_DATE_TIME time, float* Sensor, float Pressure);

/*************************************************
 Function:		storage_init
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
void storage_init(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* ifndef _STORAGE_H_ Edit By Txl 2014-12-28 */

