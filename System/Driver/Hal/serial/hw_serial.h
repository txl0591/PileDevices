/*********************************************************
  Copyright (C), 2013-2016
  File name:	hw_serial.h
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
#ifndef _HW_SERIAL_H_
#define _HW_SERIAL_H_

#ifdef __cplusplus
 extern "C" {
#endif 
#include "stm32f10x.h"

#define USART_PORT1             	  USART1
#define USART_PORT1_GPIO              GPIOA
#define USART_PORT1_CLK               RCC_APB2Periph_USART1
#define USART_PORT1_GPIO_CLK          RCC_APB2Periph_GPIOA
#define USART_PORT1_RxPin             GPIO_Pin_10
#define USART_PORT1_TxPin             GPIO_Pin_9
#define	USART_PORT1_Remap_GPIO		  GPIO_Remap_USART1
#define USART_PORT1_Tx_DMA_Channel    DMA1_Channel4
#define USART_PORT1_Tx_DMA_FLAG       DMA1_FLAG_TC4
#define USART_PORT1_Tx_DMA_IRQ		  DMA1_Channel4_IRQn
#define USART_PORT1_DR_Base           0x40013804
#define USART_PORT1_IRQn              USART1_IRQn
#define USART_PORT1_BAUDRATE 		  115200

#define USART_PORT2                   USART2
#define USART_PORT2_GPIO              GPIOA
#define USART_PORT2_CLK               RCC_APB1Periph_USART2
#define USART_PORT2_GPIO_CLK          RCC_APB2Periph_GPIOA
#define USART_PORT2_RxPin             GPIO_Pin_3
#define USART_PORT2_TxPin             GPIO_Pin_2
#define	USART_PORT2_Remap_GPIO		  GPIO_Remap_USART2 
#define USART_PORT2_Tx_DMA_Channel    DMA1_Channel7
#define USART_PORT2_Tx_DMA_FLAG       DMA1_FLAG_TC7
#define USART_PORT2_Tx_DMA_IRQ		  DMA1_Channel7_IRQn 
#define USART_PORT2_DR_Base           0x40004404
#define USART_PORT2_IRQn              USART2_IRQn
#define USART_PORT2_BAUDRATE 		  9600

#define USART_PORT3                   USART3
#define USART_PORT3_GPIO              GPIOB
#define USART_PORT3_CLK               RCC_APB1Periph_USART3
#define USART_PORT3_GPIO_CLK          RCC_APB2Periph_GPIOB
#define USART_PORT3_RxPin             GPIO_Pin_11
#define USART_PORT3_TxPin             GPIO_Pin_10
#define	USART_PORT3_Remap_GPIO		  GPIO_PartialRemap_USART3
#define USART_PORT3_Tx_DMA_Channel    DMA1_Channel2
#define USART_PORT3_Tx_DMA_FLAG       DMA1_FLAG_TC2
#define USART_PORT3_Tx_DMA_IRQ		  DMA1_Channel2_IRQn
#define USART_PORT3_DR_Base           0x40004804
#define USART_PORT3_IRQn              USART3_IRQn
#define USART_PORT3_BAUDRATE 		  115200

typedef int (*UartProc)(uint8_t buf); 

/*************************************************
 Function:		Get_USART
 Descroption:	 
 Input: 
	1.n
 Output: 
 Return: 	
 Other:  
*************************************************/
USART_TypeDef* Get_USART(uint8_t n);

/*************************************************
 Function:		Get_USART_GPIO
 Descroption:	 
 Input: 
	1.n
 Output: 
 Return: 	
 Other:  
*************************************************/
GPIO_TypeDef* Get_USART_GPIO(uint8_t n);

/*************************************************
 Function:		Get_USART_BAUDRATE
 Descroption:	 
 Input: 
	1.n
 Output: 
 Return: 	
 Other:  
*************************************************/
int Get_USART_BAUDRATE(uint8_t n);

/*************************************************
 Function:		Get_USART_CLK
 Descroption:	 
 Input: 
	1.n
 Output: 
 Return: 	
 Other:  
*************************************************/
uint32_t Get_USART_CLK(uint8_t n);

/*************************************************
 Function:		Get_USART_GPIO_CLK
 Descroption:	 
 Input: 
	1.n
 Output: 
 Return: 	
 Other:  
*************************************************/
uint32_t Get_USART_GPIO_CLK(uint8_t n);

/*************************************************
 Function:		Get_USART_RxPin
 Descroption:	 
 Input: 
	1.n
 Output: 
 Return: 	
 Other:  
*************************************************/
uint16_t Get_USART_RxPin(uint8_t n);

/*************************************************
 Function:		Get_USART_TxPin
 Descroption:	 
 Input: 
	1.n
 Output: 
 Return: 	
 Other:  
*************************************************/
uint16_t Get_USART_TxPin(uint8_t n);

/*************************************************
 Function:		Get_USART_Remap_GPIO
 Descroption:	 
 Input: 
	1.n
 Output: 
 Return: 	
 Other:  
*************************************************/
uint32_t Get_USART_Remap_GPIO(uint8_t n);

/*************************************************
 Function:		Get_USART_Tx_DMA_Channel
 Descroption:	 
 Input: 
	1.n
 Output: 
 Return: 	
 Other:  
*************************************************/
DMA_Channel_TypeDef* Get_USART_Tx_DMA_Channel(uint8_t n);

/*************************************************
 Function:		Get_USART_Tx_DMA_FLAG
 Descroption:	 
 Input: 
	1.n
 Output: 
 Return: 	
 Other:  
*************************************************/
uint32_t Get_USART_Tx_DMA_FLAG(uint8_t n);

/*************************************************
 Function:		Get_USART_Tx_DMA_IRQ
 Descroption:	 
 Input: 
	1.n
 Output: 
 Return: 	
 Other:  
*************************************************/
uint32_t Get_USART_Tx_DMA_IRQ(uint8_t n);

/*************************************************
 Function:		Get_USART_DR_Base
 Descroption:	 
 Input: 
	1.n
 Output: 
 Return: 	
 Other:  
*************************************************/
uint32_t Get_USART_DR_Base(uint8_t n);

/*************************************************
 Function:		Get_USART_IRQn
 Descroption:	 
 Input: 
	1.n
 Output: 
 Return: 	
 Other:  
*************************************************/
uint32_t Get_USART_IRQn(uint8_t n);


#define USART(n)					 Get_USART(n)
#define USART_GPIO(n) 		         Get_USART_GPIO(n) 
#define USART_CLK(n)             	 Get_USART_CLK(n)
#define USART_GPIO_CLK(n)          	 Get_USART_GPIO_CLK(n)
#define USART_RxPin(n)             	 Get_USART_RxPin(n)
#define USART_TxPin(n)               Get_USART_TxPin(n)
#define USART_Remap_GPIO(n) 		 Get_USART_Remap_GPIO(n)
#define USART_Tx_DMA_Channel(n)      Get_USART_Tx_DMA_Channel(n)
#define USART_Tx_DMA_FLAG(n)         Get_USART_Tx_DMA_FLAG(n)
#define USART_Tx_DMA_IRQ(n)		 	 Get_USART_Tx_DMA_IRQ(n)
#define USART_DR_Base(n)             Get_USART_DR_Base(n)
#define USART_IRQn(n)                Get_USART_IRQn(n)
#define USART_BAUDRATE(n) 		     Get_USART_BAUDRATE(n)

/*************************************************
  Function:		hw_serial_init
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_serial_init(void);

/*************************************************
  Function:		hw_serial_printf
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_serial_printf(const char *fmt, ...);

/*************************************************
  Function:		hw_serial_printf_null
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_serial_printf_null(const char *fmt, ...);

/*************************************************
  Function:		hw_serial_send_rs232
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_serial_send_gps(uint32_t buf, uint16_t len);

/*************************************************
  Function:		hw_serial_send_gprs
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_serial_send_gprs(uint32_t buf, uint16_t len);

/*************************************************
  Function:		hw_serial_send_rs232
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_serial_send_rs232(uint32_t buf, uint16_t len);

/*************************************************
  Function:		hw_serial_send_bluetooth
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_serial_send_bluetooth(uint32_t buf, uint16_t len);

/*************************************************
 Function:		hw_serial_set_proc
 Descroption:	 
 Input: 
	1.Index
	2.Proc
	3.EndProc
 Output: 
 Return: 	
 Other:  
*************************************************/
void hw_serial_set_proc(int Index, UartProc Proc, UartProc EndProc);

#ifdef __cplusplus
}
#endif

#endif
