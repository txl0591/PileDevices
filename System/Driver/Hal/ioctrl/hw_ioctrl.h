/*********************************************************
  Copyright (C), 2013-2016
  File name:	hw_ioctrl.h
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
#ifndef _HW_IOCTRL_H_
#define _HW_IOCTRL_H_

#include "stm32f10x.h"

#ifdef __cplusplus
 extern "C" {
#endif 

#define OIL_CTLR_ACTIVE				 Bit_SET
#define OIL_CTLR_IDLE				 Bit_RESET

#define IOCTRL_GPIO_CLK				 RCC_APB2Periph_GPIOC

#define OIL_CTRL_GPIO_24V		 	 GPIOC
#define OIL_CTRL_PIN_24V			 GPIO_Pin_8

#define OIL_CTRL_GPIO_220V		 	 GPIOC
#define OIL_CTRL_PIN_220V			 GPIO_Pin_7

#define BT_GPIO_CLK				     RCC_APB2Periph_GPIOB
#define BT_RST_GPIO		 	         GPIOB
#define BT_CTRL_PIN			         GPIO_Pin_8

/*************************************************
  Function:		hw_ioctrl_oil_24v_off
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_ioctrl_oil_24v_off(void);

/*************************************************
  Function:		hw_ioctrl_oil_24v_on
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_ioctrl_oil_24v_on(void);

/*************************************************
  Function:		hw_ioctrl_oil_220v_off
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_ioctrl_oil_220v_off(void);

/*************************************************
  Function:		hw_ioctrl_oil_220v_on
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_ioctrl_oil_220v_on(void);

/*************************************************
  Function:		hw_ioctrl_bt_reset
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_ioctrl_bt_reset(void);

/*************************************************
  Function:		hw_ioctrl_init
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_ioctrl_init(void);


#ifdef __cplusplus
}
#endif

#endif

