/*********************************************************
  Copyright (C), 2013-2016
  File name:	Main.c
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
#include <stdio.h>
#include "stm32f10x.h"
#include "hw_define.h"
#include "appmain.h"

/*************************************************
  Function:		hw_init
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static void hw_init(void)
{
	SystemInit();
   // hw_rtc_init();
   // hw_timer_init();
   // hw_serial_init();
	hw_ioctrl_init();
    #ifdef WIRELESS_24G
	hw_wireless_init();
    #endif
//	hw_sensor_init();
 //   hw_bluetooth_init();
    hw_spi_init();
    hw_backlight_init();
    hw_lcd_Init();
}

/*************************************************
  Function:		hw_proc
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static void hw_proc(void)
{
    hw_bluetooth_proc();
    hw_sensor_proc();
}

/*************************************************
  Function:		Main
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
int main(void)
{
	hw_init();
    hw_serial_printf("************************************************\r\n");
    hw_serial_printf("                  CoreSoft.Ltd                  \r\n");
    hw_serial_printf("  BuildTime:    %s\r\n",_BUILE_TIMER_);
    hw_serial_printf("  Hardware Ver: %s\r\n",_HARDWARE_VERSION_);
    hw_serial_printf("  Software Ver: %s\r\n",_SOFTWARE_VERSION_);
	hw_serial_printf("************************************************\r\n");

	appmain_init();
    hw_bluetooth_sync();
    hw_timer_start();
	while(1)
	{
	    hw_proc();
		appmain_proc();
	}
}


