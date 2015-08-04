/*********************************************************
 Copyright (C),2012-2018,Electronic Technology Co.,Ltd.
 File name: 		hw_spi.c
 Author: 			Txl
 Version: 			1.0
 Date: 				2015-1-31
 Description: 		
 History: 			
 					
   1.Date:	 		2015-1-31
 	 Author:	 	Txl
 	 Modification:  Created file
 	 
*********************************************************/

#include "hw_spi.h"

/*************************************************
  Function:		hw_spi2_cs
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static void hw_spi2_cs(BitAction data)
{
	GPIO_WriteBit(SPI2_SPI_GPIO, SPI2_SPI_PIN_NSS, data);
}

/*************************************************
  Function:		hw_spi2_cs_on
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_spi2_cs_on(void)
{
    hw_spi2_cs(Bit_RESET);
}

/*************************************************
  Function:		hw_spi2_cs_off
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_spi2_cs_off(void)
{
    hw_spi2_cs(Bit_SET);
}

/*************************************************
  Function:		hw_spi2_writereg
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
uint16_t hw_spi2_writereg(uint16_t data)
{
	while (SPI_I2S_GetFlagStatus(SPI2_SPI, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI2_SPI, data);
	while (SPI_I2S_GetFlagStatus(SPI2_SPI, SPI_I2S_FLAG_RXNE) == RESET);
    return  SPI_I2S_ReceiveData(SPI2_SPI);
}

/*************************************************
  Function:		hw_spi2_write
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_spi2_write(uint16_t addr, uint16_t data)
{
	hw_spi2_cs(Bit_RESET);
	hw_spi2_writereg(addr);
	hw_spi2_writereg(data);	
	hw_spi2_cs(Bit_SET);
}

/*************************************************
  Function:		hw_spi2_readbyte
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
uint16_t hw_spi2_readbyte(uint16_t addr)
{
	uint16_t data = 0;
	hw_spi2_cs(Bit_RESET);
	hw_spi2_writereg(addr);
	data = hw_spi2_writereg(0x00);
	hw_spi2_cs(Bit_SET);
	return data;
}

/*************************************************
  Function:		hw_spi2_writebuf
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_spi2_writebuf(uint16_t addr, uint16_t* buf, uint16_t len)
{
	int i;
	hw_spi2_cs(Bit_RESET);
	hw_spi2_writereg(addr);
    for (i = 0; i < len; i++)
    {
    	hw_spi2_writereg(buf[i]);
    }
	hw_spi2_cs(Bit_SET);
}

/*************************************************
  Function:		hw_spi2_readbuf
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_spi2_readbuf(uint16_t addr, uint16_t* buf, uint16_t len)
{
	int i;
	hw_spi2_cs(Bit_RESET);
	hw_spi2_writereg(addr);
    for (i = 0; i < len; i++)
    {	
   		buf[i]= hw_spi2_writereg(0x00);
    }
	hw_spi2_cs(Bit_SET);
}


/*************************************************
  Function:		hw_spi_init
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_spi_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

	/* PCLK2 = HCLK/2 */
	RCC_PCLK2Config(RCC_HCLK_Div2); 

	/* Enable SPI clock and GPIO clock for SPI */
	RCC_APB2PeriphClockCmd(SPI2_SPI_GPIO_CLK|RCC_APB2Periph_AFIO, ENABLE);
	
	/* Enable SPI_SLAVE Periph clock */
	//RCC_APB1PeriphClockCmd(SPI2_SPI_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(SPI2_SPI_CLK, ENABLE);

	/* Configure SPI pins: NSS, SCK and MOSI */
	GPIO_InitStructure.GPIO_Pin = SPI2_SPI_PIN_SCK | SPI2_SPI_PIN_MOSI;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(SPI2_SPI_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  SPI2_SPI_PIN_MISO;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(SPI2_SPI_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SPI2_SPI_PIN_NSS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI2_SPI_GPIO, &GPIO_InitStructure);
    
 	/* Spi Init*/
	SPI_Cmd(SPI2_SPI, DISABLE);  
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2_SPI, &SPI_InitStructure);
	//SPI_SSOutputCmd(SPI2_SPI, ENABLE);
	SPI_Cmd(SPI2_SPI, ENABLE);
	
	
}

