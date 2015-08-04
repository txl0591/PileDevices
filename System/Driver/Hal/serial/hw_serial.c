/*********************************************************
  Copyright (C), 2013-2016
  File name:	hw_serial_init.c
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
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif 

#if defined(UART_GPRS)
UartProc mUart1Proc;
UartProc mUart1EndProc;
#endif

#if defined(UART_GPS) || defined(UART_BLUETOOTH)
UartProc mUart2Proc;
UartProc mUart2EndProc;
#endif

/*************************************************
 Function:		Get_USART
 Descroption:	 
 Input: 
	1.n
 Output: 
 Return: 	
 Other:  
*************************************************/
USART_TypeDef* Get_USART(uint8_t n)
{
	switch (n)
	{
		case 0:
			return USART_PORT1;

		case 1:
			return USART_PORT2;

		case 2:
			return USART_PORT3;
	}

	return USART_PORT1;
}

/*************************************************
 Function:		Get_USART_GPIO
 Descroption:	 
 Input: 
	1.n
 Output: 
 Return: 	
 Other:  
*************************************************/
GPIO_TypeDef* Get_USART_GPIO(uint8_t n)
{
	switch (n)
	{
		case 0:
			return USART_PORT1_GPIO;

		case 1:
			return USART_PORT2_GPIO;

		case 2:
			return USART_PORT3_GPIO;
	}

	return USART_PORT1_GPIO;
}

/*************************************************
 Function:		Get_USART_BAUDRATE
 Descroption:	 
 Input: 
	1.n
 Output: 
 Return: 	
 Other:  
*************************************************/
int Get_USART_BAUDRATE(uint8_t n)
{
	switch (n)
	{
		case 0:
			return USART_PORT1_BAUDRATE;

		case 1:
			return USART_PORT2_BAUDRATE;

		case 2:
			return USART_PORT3_BAUDRATE;
	}

	return USART_PORT1_BAUDRATE;
}

/*************************************************
 Function:		Get_USART_CLK
 Descroption:	 
 Input: 
	1.n
 Output: 
 Return: 	
 Other:  
*************************************************/
uint32_t Get_USART_CLK(uint8_t n)
{
	switch (n)
	{
		case 0:
			return USART_PORT1_CLK;

		case 1:
			return USART_PORT2_CLK;

		case 2:
			return USART_PORT3_CLK;
	}

	return USART_PORT1_CLK;
}

/*************************************************
 Function:		Get_USART_GPIO_CLK
 Descroption:	 
 Input: 
	1.n
 Output: 
 Return: 	
 Other:  
*************************************************/
uint32_t Get_USART_GPIO_CLK(uint8_t n)
{
	switch (n)
	{
		case 0:
			return USART_PORT1_GPIO_CLK;

		case 1:
			return USART_PORT2_GPIO_CLK;

		case 2:
			return USART_PORT3_GPIO_CLK;
	}

	return USART_PORT1_GPIO_CLK;
}

/*************************************************
 Function:		Get_USART_RxPin
 Descroption:	 
 Input: 
	1.n
 Output: 
 Return: 	
 Other:  
*************************************************/
uint16_t Get_USART_RxPin(uint8_t n)
{
	switch (n)
	{
		case 0:
			return USART_PORT1_RxPin;

		case 1:
			return USART_PORT2_RxPin;

		case 2:
			return USART_PORT3_RxPin;
	}

	return USART_PORT1_RxPin;
}

/*************************************************
 Function:		Get_USART_TxPin
 Descroption:	 
 Input: 
	1.n
 Output: 
 Return: 	
 Other:  
*************************************************/
uint16_t Get_USART_TxPin(uint8_t n)
{
	switch (n)
	{
		case 0:
			return USART_PORT1_TxPin;

		case 1:
			return USART_PORT2_TxPin;

		case 2:
			return USART_PORT3_TxPin;
	}

	return USART_PORT1_TxPin;
}

/*************************************************
 Function:		Get_USART_Remap_GPIO
 Descroption:	 
 Input: 
	1.n
 Output: 
 Return: 	
 Other:  
*************************************************/
uint32_t Get_USART_Remap_GPIO(uint8_t n)
{
	switch (n)
	{
		case 0:
			return USART_PORT1_Remap_GPIO;

		case 1:
			return USART_PORT2_Remap_GPIO;

		case 2:
			return USART_PORT3_Remap_GPIO;
	}

	return USART_PORT1_Remap_GPIO;
}

/*************************************************
 Function:		Get_USART_Tx_DMA_Channel
 Descroption:	 
 Input: 
	1.n
 Output: 
 Return: 	
 Other:  
*************************************************/
DMA_Channel_TypeDef* Get_USART_Tx_DMA_Channel(uint8_t n)
{
	switch (n)
	{
		case 0:
			return USART_PORT1_Tx_DMA_Channel;

		case 1:
			return USART_PORT2_Tx_DMA_Channel;

		case 2:
			return USART_PORT3_Tx_DMA_Channel;
	}

	return USART_PORT1_Tx_DMA_Channel;
}

/*************************************************
 Function:		Get_USART_Tx_DMA_FLAG
 Descroption:	 
 Input: 
	1.n
 Output: 
 Return: 	
 Other:  
*************************************************/
uint32_t Get_USART_Tx_DMA_FLAG(uint8_t n)
{
	switch (n)
	{
		case 0:
			return USART_PORT1_Tx_DMA_FLAG;

		case 1:
			return USART_PORT2_Tx_DMA_FLAG;

		case 2:
			return USART_PORT3_Tx_DMA_FLAG;
	}

	return USART_PORT1_Tx_DMA_FLAG;
}

/*************************************************
 Function:		Get_USART_Tx_DMA_IRQ
 Descroption:	 
 Input: 
	1.n
 Output: 
 Return: 	
 Other:  
*************************************************/
uint32_t Get_USART_Tx_DMA_IRQ(uint8_t n)
{
	switch (n)
	{
		case 0:
			return USART_PORT1_Tx_DMA_IRQ;

		case 1:
			return USART_PORT2_Tx_DMA_IRQ;

		case 2:
			return USART_PORT3_Tx_DMA_IRQ;
	}

	return USART_PORT1_Tx_DMA_IRQ;
}

/*************************************************
 Function:		Get_USART_DR_Base
 Descroption:	 
 Input: 
	1.n
 Output: 
 Return: 	
 Other:  
*************************************************/
uint32_t Get_USART_DR_Base(uint8_t n)
{
	switch (n)
	{
		case 0:
			return USART_PORT1_DR_Base;

		case 1:
			return USART_PORT2_DR_Base;

		case 2:
			return USART_PORT3_DR_Base;
	}

	return USART_PORT1_DR_Base;
}

/*************************************************
 Function:		Get_USART_IRQn
 Descroption:	 
 Input: 
	1.n
 Output: 
 Return: 	
 Other:  
*************************************************/
uint32_t Get_USART_IRQn(uint8_t n)
{
	switch (n)
	{
		case 0:
			return USART_PORT1_IRQn;

		case 1:
			return USART_PORT2_IRQn;

		case 2:
			return USART_PORT3_IRQn;
	}

	return USART_PORT1_IRQn;
}

#if defined(UART_DEBUG)
/*************************************************
  Function:		fputc
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
#ifdef __GNUC__
int __io_putchar(int ch)
#else
int fputc(int ch, FILE *f)
#endif 
{

  USART_SendData(USART(USART_DEBUG), (uint8_t) ch);
  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART(USART_DEBUG), USART_FLAG_TC) == RESET)
  {}

  return ch;
}
#endif

/*************************************************
  Function:		DMA1_Channel2_IRQHandler
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void DMA1_Channel2_IRQHandler(void)
{
	DMA_ClearFlag(USART_Tx_DMA_FLAG(USART_RS232));
	DMA_Cmd(USART_Tx_DMA_Channel(USART_RS232), DISABLE);
}

/*************************************************
  Function:		DMA1_Channel2_IRQHandler
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void DMA1_Channel4_IRQHandler(void)
{
  	DMA_ClearFlag(USART_Tx_DMA_FLAG(USART_GPRS));
	DMA_Cmd(USART_Tx_DMA_Channel(USART_GPRS), DISABLE);
}

/*************************************************
  Function:		DMA1_Channel7_IRQHandler
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void DMA1_Channel7_IRQHandler(void)
{
	DMA_ClearFlag(USART_Tx_DMA_FLAG(USART_GPS));
	DMA_Cmd(USART_Tx_DMA_Channel(USART_GPS), DISABLE);
    #if defined (UART_GPS) || defined (UART_BLUETOOTH)
    if(NULL != mUart2EndProc)
    {
        mUart2EndProc(0);
    }   
    #endif
}

/*************************************************
  Function:		USART1_IRQHandler
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void USART1_IRQHandler(void)
{

}

/*************************************************
  Function:		USART2_IRQHandler
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void USART2_IRQHandler(void)
{
    #if defined (UART_GPS) || defined (UART_BLUETOOTH)
    uint8_t buf = 0;
    if(USART_GetITStatus(USART(USART_GPS), USART_IT_RXNE) != RESET)
    {
        buf = USART_ReceiveData(USART(USART_GPS));
        if(NULL != mUart2Proc)
        {
            mUart2Proc(buf);
        }
    }    
    #endif
}

/*************************************************
  Function:		USART3_IRQHandler
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void USART3_IRQHandler(void)
{
}

/*************************************************
  Function:		hw_serial_printf
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_serial_printf(const char *fmt, ...)
{
	#if defined(UART_DEBUG)
	printf(fmt);
	#endif
}

/*************************************************
  Function:		hw_serial_printf_null
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_serial_printf_null(const char *fmt, ...)
{

}

/*************************************************
  Function:		hw_usart_configures
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static void hw_usart_configures(uint8_t port, uint32_t BaudRate)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	/*USARTy configuration*/
	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate = BaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	/* Configure USART */
	USART_Init(USART(port), &USART_InitStructure);
	
	/* Enable the USART Receive Interrupt */
	USART_ITConfig(USART(port), USART_IT_RXNE, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  
	NVIC_InitStructure.NVIC_IRQChannel = USART_IRQn(port);
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

    #if defined(UART_DEBUG)
	if (USART_DEBUG != port)
    #endif    
	{
		USART_DMACmd(USART(port), USART_DMAReq_Tx, ENABLE);
	}
	/* Enable the USARTy */
  	USART_Cmd(USART(port), ENABLE);
}

/*************************************************
  Function:		hw_serial_port_init
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_serial_port_init(uint8_t port)
{
	/*Configures the different GPIO ports.*/
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	/* Enable GPIO clock */
	RCC_APB2PeriphClockCmd(USART_GPIO_CLK(port), ENABLE);

	/* Enable USART Clock */
	if(port >= 1)
	{
		RCC_APB1PeriphClockCmd(USART_CLK(port), ENABLE); 	
	}
	else
	{
		RCC_APB2PeriphClockCmd(USART_CLK(port), ENABLE); 	
	}

	/* Enable the USART3 Pins Software Remapping */
  	GPIO_PinRemapConfig(USART_Remap_GPIO(port), ENABLE);
	
	/* Configure USARTy Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = USART_RxPin(port);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART_GPIO(port), &GPIO_InitStructure);

	/* Configure USARTy Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = USART_TxPin(port);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(USART_GPIO(port), &GPIO_InitStructure);

    #if defined(UART_DEBUG)
	if (USART_DEBUG != port)
    #endif   
	{
		/*Configures the DMA.*/
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  
		NVIC_InitStructure.NVIC_IRQChannel = USART_Tx_DMA_IRQ(port);
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		DMA_DeInit(USART_Tx_DMA_Channel(port));
		DMA_InitStructure.DMA_PeripheralBaseAddr = USART_DR_Base(port);
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)NULL;
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
		DMA_InitStructure.DMA_BufferSize = 0;
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
		DMA_InitStructure.DMA_Priority = DMA_Priority_High;
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		DMA_Init(USART_Tx_DMA_Channel(port), &DMA_InitStructure);
		DMA_ITConfig(USART_Tx_DMA_Channel(port), DMA_IT_TC,ENABLE);
	}
	hw_usart_configures(port, USART_BAUDRATE(port));
}

/*************************************************
  Function:		hw_serial_send_rs232
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_serial_send_gps(uint32_t buf, uint16_t len)
{
	DMA_SetBuffer(USART_Tx_DMA_Channel(USART_GPS), buf, len);
	DMA_Cmd(USART_Tx_DMA_Channel(USART_GPS), ENABLE);
}


/*************************************************
  Function:		hw_serial_send_bluetooth
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_serial_send_bluetooth(uint32_t buf, uint16_t len)
{
	DMA_SetBuffer(USART_Tx_DMA_Channel(USART_BLUETOOTH), buf, len);
	DMA_Cmd(USART_Tx_DMA_Channel(USART_BLUETOOTH), ENABLE);
}

/*************************************************
  Function:		hw_serial_send_gprs
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_serial_send_gprs(uint32_t buf, uint16_t len)
{
	DMA_SetBuffer(USART_Tx_DMA_Channel(USART_GPRS), buf, len);
	DMA_Cmd(USART_Tx_DMA_Channel(USART_GPRS), ENABLE);
}

/*************************************************
  Function:		hw_serial_send_rs232
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_serial_send_rs232(uint32_t buf, uint16_t len)
{
	DMA_SetBuffer(USART_Tx_DMA_Channel(USART_RS232), buf, len);
	DMA_Cmd(USART_Tx_DMA_Channel(USART_RS232), ENABLE);
}

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
void hw_serial_set_proc(int Index, UartProc Proc, UartProc EndProc)
{
    if(Index < USART_MAX)
    {
        switch(Index)
        {
            #ifdef UART_GPRS
            case USART_GPRS:
                mUart1Proc = Proc;
                mUart1EndProc = EndProc;
                break;
            #endif    

            #if defined (UART_GPS) || defined (UART_BLUETOOTH)
            #ifdef UART_GPS
            case USART_GPS:
            #endif
            #ifdef UART_BLUETOOTH
            case USART_BLUETOOTH:
            #endif    
                mUart2Proc = Proc;
                mUart2EndProc = EndProc;
                break;
            #endif
            
            case USART_RS232:
                
                break;
            
            default:
                break;
        }
            
    }
}

/*************************************************
  Function:		hw_serial_init
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_serial_init(void)
{   
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    hw_serial_port_init(USART_RS232);
	#if defined(UART_DEBUG)
	printf(" \r\n");
	#endif
    
    #if defined(UART_GPRS)
    mUart1Proc = NULL;
	hw_serial_port_init(USART_GPRS);
    #endif

    #if defined (UART_GPS) || defined (UART_BLUETOOTH)
    mUart2Proc = NULL;
	hw_serial_port_init(USART_GPS);
    #endif
}

