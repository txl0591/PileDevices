/*********************************************************
  Copyright (C), 2013-2016
  File name:	hw_wireless.h
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
#ifndef _HW_WIRELESS_H_
#define _HW_WIRELESS_H_

#include "stm32f10x.h"

#ifdef __cplusplus
 extern "C" {
#endif 

#define WIRELESS_SPI					 SPI2
#define WIRELESS_SPI_CLK				 RCC_APB1Periph_SPI2
#define WIRELESS_SPI_GPIO				 GPIOB
#define WIRELESS_SPI_GPIO_CLK			 RCC_APB2Periph_GPIOB
#define WIRELESS_SPI_IRQ_CLK			 RCC_APB2Periph_GPIOA
#define WIRELESS_SPI_PIN_NSS			 GPIO_Pin_12
#define WIRELESS_SPI_PIN_SCK			 GPIO_Pin_13
#define WIRELESS_SPI_PIN_MISO			 GPIO_Pin_14
#define WIRELESS_SPI_PIN_MOSI			 GPIO_Pin_15
#define WIRELESS_SPI_PIN_CE				 GPIO_Pin_1
#define WIRELESS_SPI_IRQn				 SPI2_IRQn
#define WIRELESS_SPI_DMA				 DMA1
#define WIRELESS_SPI_DMA_CLK			 RCC_AHBPeriph_DMA1
#define WIRELESS_SPI_Rx_DMA_Channel		 DMA1_Channel4
#define WIRELESS_SPI_Rx_DMA_FLAG		 DMA1_FLAG_TC4
#define WIRELESS_SPI_Tx_DMA_Channel		 DMA1_Channel5
#define WIRELESS_SPI_Tx_DMA_FLAG		 DMA1_FLAG_TC5
#define WIRELESS_SPI_DR_Base			 0x4000380C 

#define WIRELESS_SPI_IRQ_GPIO			 GPIOA
#define WIRELESS_SPI_PIN_IRQ			 GPIO_Pin_7
#define WIRELESS_SPI_RX_IRQn			 EXTI9_5_IRQn
#define WIRELESS_SPI_IRQ_RES 			 GPIO_PortSourceGPIOA
#define WIRELESS_SPI_GPIO_RES 			 GPIO_PinSource7
#define WIRELESS_SPI_EXTI				 EXTI_Line7

#define WIRELESS_TX						 1
#define WIRELESS_RX						 0


#define RF_READ_REG        0x00  // Define read command to register
#define RF_WRITE_REG       0x20  // Define write command to register
#define RF_RD_RX_PLOAD     0x61  // Define RX payload register address
#define RF_WR_TX_PLOAD     0xA0  // Define TX payload register address
#define RF_FLUSH_TX        0xE1  // Define flush TX register command
#define RF_FLUSH_RX        0xE2  // Define flush RX register command
#define RF_REUSE_TX_PL     0xE3  // Define reuse TX payload register command
#define RF_NOP             0xFF  // Define No Operation, might be used to read status register

#define RF_CONFIG          0x00  // 'Config' register address
#define RF_EN_AA           0x01  // 'Enable Auto Acknowledgment' register address
#define RF_EN_RXADDR       0x02  // 'Enabled RX addresses' register address
#define RF_SETUP_AW        0x03  // 'Setup address width' register address
#define RF_SETUP_RETR      0x04  // 'Setup Auto. Retrans' register address
#define RF_CH              0x05  // 'RF channel' register address
#define RF_SETUP           0x06  // 'RF setup' register address
#define RF_STATUS          0x07  // 'Status' register address
#define RF_OBSERVE_TX      0x08  // 'Observe TX' register address
#define RF_CD              0x09  // 'Carrier Detect' register address
#define RF_RX_ADDR_P0      0x0A  // 'RX address pipe0' register address
#define RF_RX_ADDR_P1      0x0B  // 'RX address pipe1' register address
#define RF_RX_ADDR_P2      0x0C  // 'RX address pipe2' register address
#define RF_RX_ADDR_P3      0x0D  // 'RX address pipe3' register address
#define RF_RX_ADDR_P4      0x0E  // 'RX address pipe4' register address
#define RF_RX_ADDR_P5      0x0F  // 'RX address pipe5' register address
#define RF_TX_ADDR         0x10  // 'TX address' register address
#define RF_RX_PW_P0        0x11  // 'RX payload width, pipe0' register address
#define RF_RX_PW_P1        0x12  // 'RX payload width, pipe1' register address
#define RF_RX_PW_P2        0x13  // 'RX payload width, pipe2' register address
#define RF_RX_PW_P3        0x14  // 'RX payload width, pipe3' register address
#define RF_RX_PW_P4        0x15  // 'RX payload width, pipe4' register address
#define RF_RX_PW_P5        0x16  // 'RX payload width, pipe5' register address
#define RF_FIFO_STATUS     0x17  // 'FIFO Status Register' register address
#define RF_STA_MARK_RX     0X40
#define RF_STA_MARK_TX     0X20
#define RF_STA_MARK_MX     0X10	

#define RF_SET_DEFAULT_ADDR(data)	data[0] = 0x00;	data[1] = 0x00; data[2] = 0x00; data[3] = 0x00; data[4] = 0x01
	
		
#ifdef WIRELESS_24G
/*************************************************
  Function:		hw_wireless_init
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_wireless_init(void);
/*************************************************
  Function:		hw_wireless_rx_mode
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_wireless_rx_mode(void);

/*************************************************
  Function:		hw_wireless_interrupt
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_wireless_interrupt(void);

/*************************************************
  Function:		hw_wireless_send
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_wireless_send(uint16_t* buf, uint16_t len);
#endif

#ifdef __cplusplus
}
#endif

#endif

