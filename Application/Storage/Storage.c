/*********************************************************
 Copyright (C),2011-2017,Electronic Technology Co.,Ltd.
 File name: 		Storage.c
 Author: 			Txl
 Version: 			1.0
 Date: 				2014-12-28
 Description: 		
 History: 			
 					
   1.Date:	 		2014-12-28
 	 Author:	 	Txl
 	 Modification:  Created file
 	 
*********************************************************/
#include "hw_define.h"
#include "storage.h"

#define _DEBUG_EXPERIMENT_INFO_

#ifdef _DEBUG_EXPERIMENT_INFO_
static EXPERIMENT_INFO mExperiment;
#endif

/*************************************************
 Function:		storage_get_experiment
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
PEXPERIMENT_PARAM storage_get_experiment(uint16_t index)
{
    #ifdef _DEBUG_EXPERIMENT_INFO_
    return &mExperiment.Param[mExperiment.Count-index];
    #endif
}

/*************************************************
 Function:		storage_add_experiment
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
void storage_add_experiment(PZONE_DATE_TIME time, float* Sensor, float Pressure)
{
    uint8_t i;
    #ifdef _DEBUG_EXPERIMENT_INFO_
    if(mExperiment.Count < MAX_SENSOR_INFO)
    {
        mExperiment.Param[mExperiment.Count].Time.hour = time->hour;
        mExperiment.Param[mExperiment.Count].Time.min = time->min;
        mExperiment.Param[mExperiment.Count].mPressure = Pressure;
        for (i = 0 ; i < MAX_SENSOR; i++)
        {
            mExperiment.Param[mExperiment.Count].mSensor[i] = Sensor[i];
        }
        
        mExperiment.Count++;
    }
    else
    {
        hw_serial_printf("Save experiment Over\r\n");
    }
    #else
    #endif
}

/*************************************************
 Function:		storage_init
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
void storage_init(void)
{
    #ifdef _DEBUG_EXPERIMENT_INFO_
    mExperiment.Count = 0;
    #endif
}

