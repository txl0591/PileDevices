/*********************************************************
 Copyright (C),2011-2017,Electronic Technology Co.,Ltd.
 File name: 		logic_scan.c
 Author: 			Tangxl
 Version: 			1.0
 Date: 				2017-5-18
 Description: 		
 History: 			
 					
   1.Date:	 		2017-5-18
 	 Author:	 	Tangxl
 	 Modification:  Created fil
   2. ... 
*********************************************************/
#include "logic.h"
#include "log.h"
#include "math.h"

//#if (LEVEL_DEBUG && LEVEL_SCAN)
//#define LOGD        hw_serial_printf
//#else
//#define LOGD        hw_serial_printf_null  
//#endif

#define LOGD        hw_serial_printf

#define MAX_AD_BUF          50
#define MAX_485_BUF         2
#define MAX_TICK            50

#define LAST_485            0
#define NOW_485             1

static uint8_t mSensorScan[6];
static uint8_t mSensorScan1[MAX_485_BUF][6];
static uint8_t mSensorScan2[MAX_485_BUF][6];
static uint8_t mSensorScan3[MAX_485_BUF][6];
static uint8_t mSensorScan4[MAX_485_BUF][6];
static uint8_t mSensorScanAd[MAX_AD_BUF][3];
static uint8_t mSensorScanIndex[SENSOR_MAX] = {0,0,0,0,0};
static uint8_t mSensorScanOnline[SENSOR_MAX] = {0,0,0,0,0};
static uint8_t mSensorChannel = SENSOR1;
static float mBaseVoltage = 0.0;
static float mBasePress = 0.0;

/*************************************************
 Function:		SwitchADIndex
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
static void SwitchSensorChannel(void)
{
    mSensorChannel++;
    if(mSensorChannel > SENSORAD)
    {
        mSensorChannel = SENSOR1;
    }
}

/*************************************************
 Function:		SwitchScanIndex
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
static void SwitchScanIndex(int Channel)
{
    if(SENSORAD >= Channel)
    {
        mSensorScanIndex[Channel]++;
        if(mSensorScanIndex[Channel] >= MAX_AD_BUF)
        {
            mSensorScanIndex[Channel] = 0;
        }
    }
}

/*************************************************
 Function:		GetSwitchScanIndex
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
static int GetSwitchScanIndex(int Channel)
{
    return mSensorScanIndex[Channel];
}

/*************************************************
 Function:		GetAvgVolage
 Descroption:	 
 Input: 
	1.value1
	2.value2
	3.value3
 Output: 
 Return: 	
 Other:  
*************************************************/
static float GetAvgVolage(uint8_t value1, uint8_t value2, uint8_t value3)
{
    float volage = 0;
    float tmp = 0;
    uint32_t tmpvalue = 0;
    
    value1 &= 0x0F;
    tmpvalue = value1*256*256+value2*256+value3;
   
    if(tmpvalue > MAX_VOLAGE_BASE)
    {
    	tmpvalue -= MAX_VOLAGE_BASE;
    	tmpvalue = MAX_VOLAGE_BASE-tmpvalue;
    }
    tmp = tmpvalue;
    tmp = (float)tmp*MAX_VOLAGE;
    volage = tmp /MAX_VOLAGE_BASE;    

    return volage;
}

/*************************************************
 Function:		logic_sensor_get_voltage
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
void logic_sensor_get_voltage(uint8_t Id)
{
    uint8_t i;
    uint8_t data[4];
    float Volage[MAX_AD_BUF]; 
    float MaxVolage = 0;
    uint32_t VolageEnd = 0;

    for (i = 0 ; i < MAX_AD_BUF; i++)
    {      
        Volage[i] = GetAvgVolage(mSensorScanAd[i][0], mSensorScanAd[i][1], mSensorScanAd[i][2]);
    }
    
    for (i = 0 ; i < MAX_AD_BUF; i++)
    {
        MaxVolage += Volage[i];
    }
    
    MaxVolage = MaxVolage/(MAX_AD_BUF);
    VolageEnd = (uint32_t)(MaxVolage*1000000);
    data[0] = SENSORAD;     
    data[3] = VolageEnd;
    VolageEnd = VolageEnd >> 8;
    data[2] = VolageEnd;
    VolageEnd = VolageEnd >> 8;
    data[1] = VolageEnd;
    logic_ackcommand(COMM_GET_VOLTAGE, Id, data, 4);
    LOGD("MaxVolage [%f]\r\n",MaxVolage);
}

/*************************************************
 Function:		logic_sensor_change_value
 Descroption:	 
 Input: 
	1.sensor
	2.echo
 Output: 
 Return: 	
 Other:  
*************************************************/
static float logic_sensor_change_value(uint8_t *Scan)
{
    long long tmp = 0;
    float ret = 0.0;
    float ret1 = 0.0;
    if(Scan)
    {
        tmp = Scan[3]*256*256+Scan[4]*256+Scan[5];
        if(tmp > 9437183)
        {
            
            tmp -= 16777209;
        }
        else
        {
            tmp -= 5;
        }
        
        ret = tmp/806.4;
    }
    ret1 = ret*100;
    //ret = round(ret1);
    ret = ret1/100;
    
    return ret;
}

/*************************************************
 Function:		logic_sensor_update_485
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
static void logic_sensor_update_485(uint8_t Channel,uint8_t Id)
{
    uint8_t Data[6];
    uint8_t i;
    float tmp = 0.0;
    long value = 0;

    if(mSensorScanOnline[Channel] == SENSOR_OFFLINE)
    {
        value = 0xFFFFFFFFL;
    }
    else
    {
        switch(Channel)
        {
            case SENSOR1:
                for (i = 0 ; i < 6; i++)
                {
                    Data[i] = mSensorScan1[NOW_485][i];
                }
                break;

            case SENSOR2:
                for (i = 0 ; i < 6; i++)
                {
                    Data[i] = mSensorScan2[NOW_485][i];
                }
                break;    

            case SENSOR3:
                for (i = 0 ; i < 6; i++)
                {
                    Data[i] = mSensorScan3[NOW_485][i];
                }
                break;

            case SENSOR4:
                for (i = 0 ; i < 6; i++)
                {
                    Data[i] = mSensorScan4[NOW_485][i];
                }
                break;     
        }
        
        tmp = logic_sensor_change_value(Data);
        
        value = (long)(tmp*1000);        
    }


    Data[0] = Channel;
    Data[4] = value&0xFF;
    Data[3] = (value>>8)&0xFF;
    Data[2] = (value>>16)&0xFF;
    Data[1] = (value>>24)&0xFF;

//    LOGD("Sensor%d [%f] ",Channel, tmp);
//    LOGD(" [%x] ",value);
//    LOGD(" = [ %x %x",Data[1], Data[2]);
//    LOGD(" %x %x]\r\n",Data[3], Data[4]);    
    logic_ackcommand(COMM_UPDATE, Id, Data, 5);
}

/*************************************************
 Function:		logic_sensor_press
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
void logic_sensor_press(uint8_t Id)
{
    uint8_t i;
    uint8_t data[5];
    float Volage[MAX_AD_BUF]; 
    float MaxVolage = 0;

    long mVoltage = 0;
    for (i = 0 ; i < MAX_AD_BUF; i++)
    {      
        Volage[i] = GetAvgVolage(mSensorScanAd[i][0], mSensorScanAd[i][1], mSensorScanAd[i][2]);
    }
    
    for (i = 0 ; i < MAX_AD_BUF; i++)
    {
        MaxVolage += Volage[i];
    }
    
    MaxVolage = MaxVolage/(MAX_AD_BUF);
    MaxVolage -= mBaseVoltage;
    mVoltage = (long)(MaxVolage/mBasePress);
    
    data[4] = mVoltage&0xFF;
    data[3] = (mVoltage>>8)&0xFF;
    data[2] = (mVoltage>>16)&0xFF;
    data[1] = (mVoltage>>24)&0xFF;
    data[0] = SENSORAD;
//    LOGD("AD Press = [%d] \r\n",mVoltage);
    logic_ackcommand(COMM_UPDATE, Id, data, 5);
}

/*************************************************
 Function:		logic_sensor_update
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
void logic_sensor_update(uint8_t Id, uint8_t Channel)
{
    switch(Channel)
    {
       case SENSORAD:
            logic_sensor_press(Id);
            break;

        default:
            logic_sensor_update_485(Channel, Id);
            break;
    }
    
}

/*************************************************
 Function:		logic_sensor_sync_voltage
 Descroption:	 
 Input: 
	1.voltage
 Output: 
 Return: 	
 Other:  
*************************************************/
void logic_sensor_sync_voltage(uint8_t* data,uint8_t len)
{
    uint8_t i;
    long voltage = 0;
    float voltagetmp = 0;
    for (i = 0 ; i < len; i++)
    {
        voltage <<= 8;
        voltage = voltage|data[i];
    }
    voltagetmp = voltage;
    mBaseVoltage = (float)(voltagetmp/1000000);
    LOGD("mBaseVoltage [%f]\r\n",mBaseVoltage);
}

/*************************************************
 Function:		logic_sensor_sync_press
 Descroption:	 
 Input: 
	1.voltage
 Output: 
 Return: 	
 Other:  
*************************************************/
void logic_sensor_sync_press(uint8_t* data,uint8_t len)
{
    uint8_t i;
    long press = 0;
    float presstmp = 0;
    for (i = 0 ; i < len; i++)
    {
        press = press << 8;
        press = press|data[i];
    }
    presstmp = press;
    mBasePress = (float)(presstmp/10000000);
    LOGD("mBasePress [%f]\r\n",mBasePress);
}

/*************************************************
  Function:		logic_scan_proc
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void logic_scan_proc(void)
{
    if(SENSOR_FREE == hw_sensor_get_state())
    {
        if(0 == hw_sensor_scan_start(mSensorChannel, mSensorScan))
        {
            SwitchSensorChannel();    
        }
    }
}

/*************************************************
 Function:		logic_sensor_callback
 Descroption:	 
 Input: 
	1.sensor
	2.echo
 Output: 
 Return: 	
 Other:  
*************************************************/
static void logic_sensor_callback(uint8_t sensor, uint8_t echo)
{
    uint8_t index = 0;
    uint8_t i;
    if(SENSOR_ECHO_OK == echo)
    {
        mSensorScanOnline[sensor] = SENSOR_ONLINE;
        index = GetSwitchScanIndex(sensor);
        switch(sensor)
        {
            case SENSOR1:
                if(mSensorScan1[LAST_485][0] == mSensorScan[0] && mSensorScan1[LAST_485][1] == mSensorScan[1])
                {
                    for (i = 0 ; i < 6; i++)
                    {
                        mSensorScan1[NOW_485][i] = mSensorScan[i];
                    } 
                }
                else
                {
                    for (i = 0 ; i < 6; i++)
                    {
                        mSensorScan1[LAST_485][i] = mSensorScan[i];
                    }
                }
                break;
            
            case SENSOR2:
                if(mSensorScan2[LAST_485][0] == mSensorScan[0] && mSensorScan2[LAST_485][1] == mSensorScan[1])
                {
                    for (i = 0 ; i < 6; i++)
                    {
                        mSensorScan2[NOW_485][i] = mSensorScan[i];
                    } 
                }
                else
                {
                    for (i = 0 ; i < 6; i++)
                    {
                        mSensorScan2[LAST_485][i] = mSensorScan[i];
                    }
                }              
                break;
            
            case SENSOR3:
                if(mSensorScan3[LAST_485][0] == mSensorScan[0] && mSensorScan3[LAST_485][1] == mSensorScan[1])
                {
                    for (i = 0 ; i < 6; i++)
                    {
                        mSensorScan3[NOW_485][i] = mSensorScan[i];
                    } 
                }
                else
                {
                    for (i = 0 ; i < 6; i++)
                    {
                        mSensorScan3[LAST_485][i] = mSensorScan[i];
                    }
                }    
                break;
            
            case SENSOR4:
                if(mSensorScan4[LAST_485][0] == mSensorScan[0] && mSensorScan4[LAST_485][1] == mSensorScan[1])
                {
                    for (i = 0 ; i < 6; i++)
                    {
                        mSensorScan4[NOW_485][i] = mSensorScan[i];
                    } 
                }
                else
                {
                    for (i = 0 ; i < 6; i++)
                    {
                        mSensorScan4[LAST_485][i] = mSensorScan[i];
                    }
                }            
                break;
            
            case SENSOR5:
                mSensorScanAd[index][0] = mSensorScan[0];
                mSensorScanAd[index][1] = mSensorScan[1];
                mSensorScanAd[index][2] = mSensorScan[2];
                break;
            
            default:
                break;
        }
        SwitchScanIndex(sensor);   
    }
    else
    {
        if(mSensorScanOnline[sensor] > SENSOR_OFFLINE)
        {
            mSensorScanOnline[sensor]--;
        }
    }
}

/*************************************************
  Function:		logic_scan_init
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void logic_scan_init(void)
{
    hw_sensor_set_callback(logic_sensor_callback);
}
