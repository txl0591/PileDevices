/*********************************************************
  Copyright (C), 2013-2016
  File name:	flash.h
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
#ifndef _HW_FLASH_H_
#define _HW_FLASH_H_

#include "stm32f10x.h"

#ifdef __cplusplus
 extern "C" {
#endif 

#if defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL) || defined (STM32F10X_XL)
  #define FLASH_PAGE_SIZE    ((uint16_t)0x800)
#else
  #define FLASH_PAGE_SIZE    ((uint16_t)0x400)
#endif


/*************************************************
 Function:		hw_flash_init
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
void hw_flash_init(void);

/*************************************************
 Function:		hw_flash_deinit
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
void hw_flash_deinit(void);

/*************************************************
 Function:		hw_flash_erase
 Descroption:	 
 Input: 
	1.SectorAddress
 Output: 
 Return: 	
 Other:  
*************************************************/
uint32_t hw_flash_erase(uint32_t SectorAddress);

/*************************************************
 Function:		hw_flash_program
 Descroption:	 
 Input: 
	1.SectorAddress
	2.*buf
	3.buflen
 Output: 
 Return: 	
 Other:  
*************************************************/
int32_t hw_flash_program(uint32_t SectorAddress, uint8_t *buf, uint32_t buflen) ;

/*************************************************
 Function:		hw_flash_read
 Descroption:	 
 Input: 
	1.SectorAddress
	2.*buf
	3.buflen
 Output: 
 Return: 	
 Other:  
*************************************************/
uint32_t hw_flash_read(uint32_t SectorAddress, uint8_t *buf , uint32_t buflen);


#ifdef __cplusplus
}
#endif

#endif

