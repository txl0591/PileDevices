/*********************************************************
  Copyright (C), 2013-2016
  File name:	hw_ioctrl.c
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
#include "hw_ioctrl.h"
/*************************************************
  Function:		hw_ioctrl_oil_24v_off
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_ioctrl_oil_24v_off(void)
{
	GPIO_WriteBit(OIL_CTRL_GPIO_24V, OIL_CTRL_PIN_24V, OIL_CTLR_IDLE);	
}

/*************************************************
  Function:		hw_ioctrl_oil_24v_on
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_ioctrl_oil_24v_on(void)
{
	GPIO_WriteBit(OIL_CTRL_GPIO_24V, OIL_CTRL_PIN_24V, OIL_CTLR_ACTIVE);
}

/*************************************************
  Function:		hw_ioctrl_oil_220v_off
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_ioctrl_oil_220v_off(void)
{
	GPIO_WriteBit(OIL_CTRL_GPIO_220V, OIL_CTRL_PIN_220V, OIL_CTLR_IDLE);	
}

/*************************************************
  Function:		hw_ioctrl_oil_220v_on
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_ioctrl_oil_220v_on(void)
{
	GPIO_WriteBit(OIL_CTRL_GPIO_220V, OIL_CTRL_PIN_220V, OIL_CTLR_ACTIVE);	
}

/*************************************************
  Function:		hw_ioctrl_bt_reset
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_ioctrl_bt_reset(void)
{
    GPIO_WriteBit(BT_RST_GPIO, BT_CTRL_PIN, Bit_SET);	
    hw_timer_delay_ms(150);
    GPIO_WriteBit(BT_RST_GPIO, BT_CTRL_PIN, Bit_RESET);	
    hw_timer_delay_ms(150);
    GPIO_WriteBit(BT_RST_GPIO, BT_CTRL_PIN, Bit_SET);	
    hw_timer_delay_ms(500);
}

/*************************************************
  Function:		hw_ioctrl_init
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_ioctrl_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	

	RCC_APB2PeriphClockCmd(IOCTRL_GPIO_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = OIL_CTRL_PIN_24V;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(OIL_CTRL_GPIO_24V, &GPIO_InitStructure);

	hw_ioctrl_oil_24v_off();
	
	GPIO_InitStructure.GPIO_Pin = OIL_CTRL_PIN_220V;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(OIL_CTRL_GPIO_220V, &GPIO_InitStructure);

	hw_ioctrl_oil_220v_off();

    RCC_APB2PeriphClockCmd(BT_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = BT_CTRL_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(BT_RST_GPIO, &GPIO_InitStructure);
}

