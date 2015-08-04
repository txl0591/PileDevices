/*********************************************************
  Copyright (C), 2013-2016
  File name:	hw_sensor.h
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
#ifndef _HW_SENSOR_H_
#define _HW_SENSOR_H_

#include "stm32f10x.h"

#ifdef __cplusplus
 extern "C" {
#endif 

#define SENSOR_ENABLE				 Bit_SET
#define SENSOR_DISABLE				 Bit_RESET

#define SENSORAD_ENABLE				 Bit_RESET
#define SENSORAD_DISABLE			 Bit_SET


#define SENSOR_SCAN_TIMEOUT			 50000	
#define SENSOR_CLK_ACTIVE			 Bit_RESET
#define SENSOR_CLK_IDLE				 Bit_SET
#define SENSOR_DATA_ACTIVE			 Bit_RESET
#define SENSOR_SCAN_COUNT			 5 	

#define SENSORAD_CLK_ACTIVE			 Bit_RESET
#define SENSORAD_CLK_IDLE			 Bit_SET
#define SENSORAD_DATA_ACTIVE		 Bit_SET
#define SENSORAD_DATA_IDLE		     Bit_RESET
#define SENSOR_GPIO_CLK				 (RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO)
    
#define SENSOR_PORTSOURCE            GPIO_PortSourceGPIOB   
#define SENSOR_PINSOURCE             GPIO_PinSource6
#define SENSOR_EXTLINE               EXTI_Line6

#define SENSOR_DATA_GPIO_1		 	 GPIOC
#define SENSOR_DATA_1				 GPIO_Pin_12 
#define SENSOR_CLK_GPIO_1		 	 GPIOD
#define SENSOR_CLK_1				 GPIO_Pin_2 	
#define SENSOR_EN_GPIO_1		 	 GPIOC
#define SENSOR_EN_1					 GPIO_Pin_10

#define SENSOR_DATA_GPIO_2		 	 GPIOB
#define SENSOR_DATA_2				 GPIO_Pin_5 
#define SENSOR_CLK_GPIO_2		 	 GPIOB
#define SENSOR_CLK_2				 GPIO_Pin_6 	
#define SENSOR_EN_GPIO_2			 GPIOC
#define SENSOR_EN_2					 GPIO_Pin_11

#define SENSOR_DATA_GPIO_3		 	 GPIOA
#define SENSOR_DATA_3				 GPIO_Pin_8 
#define SENSOR_CLK_GPIO_3		 	 GPIOC
#define SENSOR_CLK_3				 GPIO_Pin_9 	
#define SENSOR_EN_GPIO_3			 GPIOC
#define SENSOR_EN_3					 GPIO_Pin_6

#define SENSOR_DATA_GPIO_4		 	 GPIOC
#define SENSOR_DATA_4				 GPIO_Pin_5 
#define SENSOR_CLK_GPIO_4		 	 GPIOB
#define SENSOR_CLK_4				 GPIO_Pin_0 	
#define SENSOR_EN_GPIO_4			 GPIOB
#define SENSOR_EN_4					 GPIO_Pin_2

#define SENSOR_DATA_GPIO_5		 	 GPIOA
#define SENSOR_DATA_5				 GPIO_Pin_6 
#define SENSOR_CLK_GPIO_5		 	 GPIOA
#define SENSOR_CLK_5				 GPIO_Pin_5 	
#define SENSOR_EN_GPIO_5			 GPIOA
#define SENSOR_EN_5					 GPIO_Pin_4

#define SENSOR1						 0
#define SENSOR2						 1
#define SENSOR3						 2
#define SENSOR4						 3
#define SENSOR5						 4

#define SENSORAD					 SENSOR5 	 

#define SENSOR_MAX					 5

#define SENSOR_FREE                  0
#define SENSOR_WORKING               1
#define SENSOR_FINISH                2
#define SENSOR_FINISH_DELAY          3


#define SENSOR_ECHO_OK               0
#define SENSOR_ECHO_TIMEOUT          1
#define SENSOR_ECHO_OVER             2

#define SENSOR_ONLINE                100
#define SENSOR_OFFLINE               0

#define SENSOR_DATA_GPIO(n)		 	(SENSOR_DATA_GPIO_##n)
#define SENSOR_DATA_PIN(n)			(SENSOR_DATA_##n)
#define SENSOR_CLK_GPIO(n)		 	(SENSOR_CLK_GPIO_##n)
#define SENSOR_CLK_PIN(n)			(SENSOR_CLK_##n)	
#define SENSOR_EN_GPIO(n)		 	(SENSOR_EN_GPIO_##n)
#define SENSOR_EN_PIN(n)			(SENSOR_EN_##n)

typedef struct 
{
	uint8_t mSensor;
	GPIO_TypeDef * mGPIOData;
	uint16_t mPinData;
	GPIO_TypeDef * mGPIOClk;
	uint16_t mPinClk;
	GPIO_TypeDef * mGPIOEn;
	uint16_t mPinEn;
}SensorParam,*PSensorParam;

typedef void (*hw_sensor_callback)(uint8_t sensor, uint8_t echo); 

/*************************************************
  Function:		hw_sensor_scan_start
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
int8_t hw_sensor_scan_start(uint8_t sensor, uint8_t *Data);

/*************************************************
  Function:		hw_sensor_interrupt
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_sensor_interrupt(void);

/*************************************************
  Function:		hw_sensor_proc
  Description:  
  Input:		
    1.sensor
  Output:		
  Return:		
  Others:
*************************************************/
void hw_sensor_proc(void);

/*************************************************
  Function:		hw_sensor_set_callback
  Description:  
  Input:		
    1.proc
  Output:		
  Return:		
  Others:
*************************************************/
void hw_sensor_set_callback(hw_sensor_callback proc);

/*************************************************
  Function:		hw_sensor_get_state
  Description:  
  Input:		
    1.proc
  Output:		
  Return:		
  Others:
*************************************************/
uint8_t hw_sensor_get_state(void);

/*************************************************
  Function:		hw_sensor_init
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_sensor_init(void);


#ifdef __cplusplus
}
#endif

#endif

