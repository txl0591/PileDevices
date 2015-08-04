/*********************************************************
 Copyright (C),2012-2018,Electronic Technology Co.,Ltd.
 File name: 		hw_spi.h
 Author: 			Txl
 Version: 			1.0
 Date: 				2015-1-31
 Description: 		
 History: 			
 					
   1.Date:	 		2015-1-31
 	 Author:	 	Txl
 	 Modification:  Created file
 	 
*********************************************************/
#ifndef _HW_SPI_H_
#define _HW_SPI_H_

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

//#define SPI2_SPI					 SPI2
//#define SPI2_SPI_CLK				 RCC_APB1Periph_SPI2
//#define SPI2_SPI_GPIO			     GPIOB
//#define SPI2_SPI_GPIO_CLK		     RCC_APB2Periph_GPIOB
//#define SPI2_SPI_PIN_NSS			 GPIO_Pin_12
//#define SPI2_SPI_PIN_SCK			 GPIO_Pin_13
//#define SPI2_SPI_PIN_MISO		     GPIO_Pin_14
//#define SPI2_SPI_PIN_MOSI		     GPIO_Pin_15

#define SPI2_SPI					 SPI1
#define SPI2_SPI_CLK				 RCC_APB2Periph_SPI1
#define SPI2_SPI_GPIO			     GPIOA
#define SPI2_SPI_GPIO_CLK		     RCC_APB2Periph_GPIOA
#define SPI2_SPI_PIN_NSS			 GPIO_Pin_4
#define SPI2_SPI_PIN_SCK			 GPIO_Pin_5
#define SPI2_SPI_PIN_MISO		     GPIO_Pin_6
#define SPI2_SPI_PIN_MOSI		     GPIO_Pin_7

/*************************************************
  Function:		hw_spi2_cs_on
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_spi2_cs_on(void);

/*************************************************
  Function:		hw_spi2_cs_off
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_spi2_cs_off(void);

/*************************************************
  Function:		hw_spi2_writereg
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
uint16_t hw_spi2_writereg(uint16_t data);

/*************************************************
  Function:		hw_spi2_readbyte
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
uint16_t hw_spi2_readbyte(uint16_t addr);

/*************************************************
  Function:		hw_spi2_write
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_spi2_write(uint16_t addr, uint16_t data);

/*************************************************
  Function:		hw_spi2_readbuf
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_spi2_readbuf(uint16_t addr, uint16_t* buf, uint16_t len);

/*************************************************
  Function:		hw_spi2_writebuf
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_spi2_writebuf(uint16_t addr, uint16_t* buf, uint16_t len);

/*************************************************
  Function:		hw_spi_init
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_spi_init(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* ifndef _HW_SPI_H_ Edit By Txl 2015-1-31 */

