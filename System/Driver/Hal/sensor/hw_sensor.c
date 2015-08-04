/*********************************************************
  Copyright (C), 2013-2016
  File name:	hw_sensor.c
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
#include "hw_serial.h"
#include "hw_sensor.h"
#include "log.h"

#if(LEVEL_DEBUG && LEVEL_DRIVER)
#define LOGD        hw_serial_printf
#else
#define LOGD        hw_serial_printf_null  
#endif

#define SENSOR_TIMEOUT          30
#define SENSOR_DELAY            2

static uint8_t mSensorIndex = 0;
static uint8_t* mSensorData = NULL;
static uint8_t mSensorByteIndex = 0;
static uint8_t mSensorBitCount = 0;
static uint8_t mSensorWork = SENSOR_FREE;
static uint8_t mSensorTick = 0;
static uint8_t mSensorDelay = 0;
static hw_sensor_callback mSensorProc = NULL;

/*************************************************
 Function:		SwitchData
 Descroption:	 
 Input: 
	1.data
 Output: 
 Return: 	
 Other:  
*************************************************/
static uint8_t SwitchData(uint8_t data)
{
    uint8_t i;
    uint8_t ret = 0;
    uint8_t k = 0;
    for (i = 0 ; i < 8; i++)
    {
        ret = ret << 1;
        k = 1<<i;
        if((data & k) > 0) 
        {
            ret |= 1;
        }
    }

    return ret;
}

/*************************************************
 Function:		hw_sensor_485_switch
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
static uint8_t hw_sensor_485_switch(void)
{
    uint8_t Data[6];

    if(NULL == mSensorData)
    {
        return 1;
    }
    
    Data[0] = mSensorData[0];
    Data[1] = mSensorData[1];
    Data[2] = mSensorData[2];
    Data[3] = mSensorData[3];
    Data[4] = mSensorData[4];
    Data[5] = mSensorData[5];
    mSensorData[0] = SwitchData(Data[2]);
    mSensorData[1] = SwitchData(Data[1]);
    mSensorData[2] = SwitchData(Data[0]);
    mSensorData[3] = SwitchData(Data[5]);
    mSensorData[4] = SwitchData(Data[4]);
    mSensorData[5] = SwitchData(Data[3]);
    return 0;
}

/*************************************************
  Function:		hw_sensor_work
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static void hw_sensor_work(uint8_t sensor, BitAction mode)
{
	uint16_t pin = 0;
	GPIO_TypeDef * gpio = NULL;
    
	switch (sensor)
	{
		case SENSOR1:
			gpio = SENSOR_EN_GPIO(1); 
			pin = SENSOR_EN_PIN(1);
			break;

		case SENSOR2:
			gpio = SENSOR_EN_GPIO(2);
			pin = SENSOR_EN_PIN(2);
			break;

		case SENSOR3:
			gpio = SENSOR_EN_GPIO(3);
			pin = SENSOR_EN_PIN(3);
			break;	

		case SENSOR4:
			gpio = SENSOR_EN_GPIO(4);
			pin = SENSOR_EN_PIN(4);
			break;
		
		case SENSOR5:
			gpio = SENSOR_EN_GPIO(5);
			pin = SENSOR_EN_PIN(5);
			break;	
	}
	
	if(sensor == SENSORAD)
	{
		if(SENSOR_DISABLE == mode)
		{
			GPIO_WriteBit(gpio, pin, SENSORAD_DISABLE);	
		}
		else
		{
			GPIO_WriteBit(gpio, pin, SENSORAD_ENABLE);	
		}
	}
	else
	{
		GPIO_WriteBit(gpio, pin, mode);	
	}
}

/*************************************************
 Function:		hw_sensor_time_proc
 Descroption:	 
 Input: 
	1.Param
 Output: 
 Return: 	
 Other:  
*************************************************/
static int hw_sensor_time_proc(void* Param)
{
    if(mSensorDelay > 0)
    {
        mSensorDelay--;
        if(0 == mSensorDelay)
        {
            mSensorWork = SENSOR_FREE; 
        }
    }
    
    if(mSensorWork == SENSOR_WORKING)
    {
        mSensorTick++;
        if(mSensorTick >= SENSOR_TIMEOUT)
        {
            mSensorTick = 0;
            if(mSensorProc)
            {
                if(SENSORAD != mSensorIndex)
                {
                    hw_sensor_work(mSensorIndex, SENSOR_DISABLE);    
                }  
                mSensorProc(mSensorIndex,SENSOR_ECHO_TIMEOUT);
            }
            mSensorDelay = SENSOR_DELAY;
            mSensorWork = SENSOR_FINISH_DELAY;  
        }
    }

    return 0;
}

/*************************************************
  Function:		hw_sensor_init_gpio
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static void hw_sensor_init_gpio(PSensorParam Param)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	GPIO_InitStructure.GPIO_Pin = Param->mPinData;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;
	GPIO_Init(Param->mGPIOData, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = Param->mPinClk;
	if (SENSORAD != Param->mSensor)
	{
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	}
	else
	{
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
	}
	GPIO_Init(Param->mGPIOClk, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = Param->mPinEn;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
	GPIO_Init(Param->mGPIOEn, &GPIO_InitStructure);

	hw_sensor_work(Param->mSensor, SENSOR_DISABLE);
}

/*************************************************
  Function:		hw_sensor_scan_start
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
int8_t hw_sensor_scan_start(uint8_t sensor, uint8_t *Data)
{
    if(mSensorWork == SENSOR_FREE)
    {
        mSensorIndex = sensor;
        mSensorData = Data;
        mSensorByteIndex = 0;
        mSensorBitCount = 0;    
        mSensorTick = 0;
        mSensorWork = SENSOR_WORKING;
        if(SENSORAD != mSensorIndex)
        {
            hw_sensor_work(mSensorIndex, SENSOR_ENABLE);    
        }     
        
        return 0;
    }
    else
    {
        return -1;
    }
    
}

/*************************************************
 Function:		hw_sensor_cs5513
 Descroption:	cs5331…®√Ë
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
static uint8_t hw_sensor_cs5513(void)
{
	uint8_t i,mdata;

	mdata = 0x00;
	for (i = 0; i < 8; i++)
	{
		GPIO_WriteBit(SENSOR_CLK_GPIO(5), SENSOR_CLK_PIN(5), SENSORAD_CLK_ACTIVE);	
        mdata <<= 1;
		__NOP();__NOP();__NOP();__NOP();__NOP();
		if(SENSORAD_DATA_ACTIVE == GPIO_ReadInputDataBit(SENSOR_DATA_GPIO(5),SENSOR_DATA_PIN(5)))
		{
			mdata |= 0x01;
		}
        GPIO_WriteBit(SENSOR_CLK_GPIO(5), SENSOR_CLK_PIN(5), SENSORAD_CLK_IDLE);	
		__NOP();__NOP();__NOP();__NOP();__NOP();
	}
    
	return mdata;
}

/*************************************************
  Function:		hw_sensor_scan_ad
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static uint8_t hw_sensor_scan_ad(uint8_t *Data)
{  
    uint8_t ret = FALSE;
    hw_sensor_work(SENSORAD, SENSOR_ENABLE);   
    GPIO_WriteBit(SENSOR_CLK_GPIO(5), SENSOR_CLK_PIN(5), SENSORAD_CLK_IDLE);
    GPIO_WriteBit(SENSOR_DATA_GPIO(5), SENSOR_DATA_PIN(5), SENSORAD_CLK_IDLE);    
    __NOP();__NOP();__NOP();__NOP();__NOP();   
    while(SENSORAD_DATA_ACTIVE == GPIO_ReadInputDataBit(SENSOR_DATA_GPIO(5),SENSOR_DATA_PIN(5))); 
    
	Data[0] = hw_sensor_cs5513();
	Data[1] = hw_sensor_cs5513();
	Data[2] = hw_sensor_cs5513();
    
    if(1 == (Data[0] && 0x02))
    {

        ret = TRUE;
    }
    
	hw_sensor_work(SENSORAD, SENSOR_DISABLE);
	return ret;
}


/*************************************************
  Function:		hw_sensor_interrupt_485
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static void hw_sensor_interrupt_485(void)
{
    mSensorData[mSensorByteIndex] <<= 1;
    if(SENSOR_DATA_ACTIVE == GPIO_ReadInputDataBit(SENSOR_DATA_GPIO_2,SENSOR_DATA_2))
	{
		mSensorData[mSensorByteIndex] |= 0x01;
	}
    mSensorBitCount++;
    if(mSensorBitCount == 8)
    {
        mSensorBitCount = 0;
        mSensorByteIndex++;
        if(mSensorByteIndex == 6)
        {
            hw_sensor_work(mSensorIndex, SENSOR_DISABLE);
            mSensorWork = SENSOR_FINISH;
        }
    }
}

/*************************************************
  Function:		hw_sensor_interrupt
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_sensor_interrupt(void)
{
    if(EXTI_GetITStatus(SENSOR_EXTLINE) != RESET)
    {
        if(mSensorWork == SENSOR_WORKING && mSensorData != NULL)
        {
            hw_sensor_interrupt_485();
        }

        /* Clear the  EXTI line 0 pending bit */
        EXTI_ClearITPendingBit(SENSOR_EXTLINE);
    }    
}

/*************************************************
  Function:		hw_sensor_get_state
  Description:  
  Input:		
    1.proc
  Output:		
  Return:		
  Others:
*************************************************/
uint8_t hw_sensor_get_state(void)
{
    return mSensorWork;
}


/*************************************************
  Function:		hw_sensor_proc
  Description:  
  Input:		
    1.sensor
  Output:		
  Return:		
  Others:
*************************************************/
void hw_sensor_proc(void)
{
    if(SENSORAD == mSensorIndex && mSensorWork == SENSOR_WORKING)
    {
        hw_sensor_scan_ad(mSensorData);
        if(mSensorProc)
        {
            mSensorProc(mSensorIndex,SENSOR_ECHO_OK);
        }
        mSensorDelay = SENSOR_DELAY;
        mSensorWork = SENSOR_FINISH_DELAY;    
    }

    if(mSensorWork == SENSOR_FINISH)
    {
        if(mSensorProc)
        {
            if(mSensorIndex != SENSORAD)
            {
                hw_sensor_485_switch();
            }
            mSensorProc(mSensorIndex,SENSOR_ECHO_OK);
        }
        mSensorDelay = SENSOR_DELAY;
        mSensorWork = SENSOR_FINISH_DELAY;    
    }
}

/*************************************************
  Function:		hw_sensor_set_callback
  Description:  
  Input:		
    1.proc
  Output:		
  Return:		
  Others:
*************************************************/
void hw_sensor_set_callback(hw_sensor_callback proc)
{
    mSensorProc = proc;
}

/*************************************************
  Function:		hw_sensor_init
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_sensor_init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    
	SensorParam Sensor;
		
	RCC_APB2PeriphClockCmd(SENSOR_GPIO_CLK, ENABLE);

	Sensor.mSensor = SENSOR1;
	Sensor.mGPIOData = SENSOR_DATA_GPIO(1);
	Sensor.mPinData = SENSOR_DATA_PIN(1);
	Sensor.mGPIOClk = SENSOR_CLK_GPIO(1);
	Sensor.mPinClk = SENSOR_CLK_PIN(1);
	Sensor.mGPIOEn = SENSOR_EN_GPIO(1);
	Sensor.mPinEn = SENSOR_EN_PIN(1);
	hw_sensor_init_gpio(&Sensor);

	Sensor.mSensor = SENSOR2;
	Sensor.mGPIOData = SENSOR_DATA_GPIO(2);
	Sensor.mPinData = SENSOR_DATA_PIN(2);
	Sensor.mGPIOClk = SENSOR_CLK_GPIO(2);
	Sensor.mPinClk = SENSOR_CLK_PIN(2);
	Sensor.mGPIOEn = SENSOR_EN_GPIO(2);
	Sensor.mPinEn = SENSOR_EN_PIN(2);
	hw_sensor_init_gpio(&Sensor);

	Sensor.mSensor = SENSOR3;
	Sensor.mGPIOData = SENSOR_DATA_GPIO(3);
	Sensor.mPinData = SENSOR_DATA_PIN(3);
	Sensor.mGPIOClk = SENSOR_CLK_GPIO(3);
	Sensor.mPinClk = SENSOR_CLK_PIN(3);
	Sensor.mGPIOEn = SENSOR_EN_GPIO(3);
	Sensor.mPinEn = SENSOR_EN_PIN(3);
	hw_sensor_init_gpio(&Sensor);

	Sensor.mSensor = SENSOR4;
	Sensor.mGPIOData = SENSOR_DATA_GPIO(4);
	Sensor.mPinData = SENSOR_DATA_PIN(4);
	Sensor.mGPIOClk = SENSOR_CLK_GPIO(4);
	Sensor.mPinClk = SENSOR_CLK_PIN(4);
	Sensor.mGPIOEn = SENSOR_EN_GPIO(4);
	Sensor.mPinEn = SENSOR_EN_PIN(4);
	hw_sensor_init_gpio(&Sensor);

	Sensor.mSensor = SENSOR5;
	Sensor.mGPIOData = SENSOR_DATA_GPIO(5);
	Sensor.mPinData = SENSOR_DATA_PIN(5);
	Sensor.mGPIOClk = SENSOR_CLK_GPIO(5);
	Sensor.mPinClk = SENSOR_CLK_PIN(5);
	Sensor.mGPIOEn = SENSOR_EN_GPIO(5);
	Sensor.mPinEn = SENSOR_EN_PIN(5);
	hw_sensor_init_gpio(&Sensor);

    EXTI_ClearITPendingBit(SENSOR_PINSOURCE);
    /* Connect EXTI0 Line to PB.06 pin */
    GPIO_EXTILineConfig(SENSOR_PORTSOURCE, SENSOR_PINSOURCE);
    
    /* Configure EXTI0 line */
    EXTI_InitStructure.EXTI_Line = SENSOR_EXTLINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Configure one bit for preemption priority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

    /* Enable and set EXTI0 Interrupt to the lowest priority */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    hw_timer_set_proc(TIME_1MS, hw_sensor_time_proc, NULL);
}

