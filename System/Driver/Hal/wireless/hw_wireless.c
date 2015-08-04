/*********************************************************
  Copyright (C), 2013-2016
  File name:	hw_wireless.c
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
#define REG_NUM		16

#ifdef WIRELESS_24G
/*************************************************
  Function:		hw_wireless_spi_cs
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static void hw_wireless_spi_cs(BitAction data)
{
	GPIO_WriteBit(WIRELESS_SPI_GPIO, WIRELESS_SPI_PIN_NSS, data);
}

/*************************************************
  Function:		hw_wireless_spi_ce
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static void hw_wireless_spi_ce(BitAction data)
{
	GPIO_WriteBit(WIRELESS_SPI_GPIO, WIRELESS_SPI_PIN_CE, data);
}

/*************************************************
  Function:		hw_wireless_spi_writereg
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static uint16_t hw_wireless_spi_writereg(uint16_t data)
{
	while (SPI_I2S_GetFlagStatus(WIRELESS_SPI, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(WIRELESS_SPI, data);
	while (SPI_I2S_GetFlagStatus(WIRELESS_SPI, SPI_I2S_FLAG_RXNE) == RESET);
    return  SPI_I2S_ReceiveData(WIRELESS_SPI);
}

/*************************************************
  Function:		hw_wireless_spi_writereg
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static void hw_wireless_spi_writecmd(uint16_t addr, uint16_t data)
{
	hw_wireless_spi_cs(Bit_RESET);
	hw_wireless_spi_writereg(addr);
	hw_wireless_spi_writereg(data);	
	hw_wireless_spi_cs(Bit_SET);
}
/*************************************************
  Function:		hw_wireless_spi_writebyte
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static void hw_wireless_spi_writebyte(uint16_t addr, uint16_t data)
{
	hw_wireless_spi_cs(Bit_RESET);
	hw_wireless_spi_writereg(RF_WRITE_REG|addr);
	hw_wireless_spi_writereg(data);	
	hw_wireless_spi_cs(Bit_SET);
}

/*************************************************
  Function:		hw_wireless_spi_readbyte
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
static uint16_t hw_wireless_spi_readbyte(uint16_t addr)
{
	hw_wireless_spi_cs(Bit_RESET);
	hw_wireless_spi_writereg(RF_READ_REG|addr);
	uint16_t data = hw_wireless_spi_writereg(RF_NOP);
	hw_wireless_spi_cs(Bit_SET);
	return data;
}

/*************************************************
  Function:		hw_wireless_spi_writebuf
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_wireless_spi_writebuf(uint16_t addr, uint16_t* buf, uint16_t len)
{
	int i;
	hw_wireless_spi_cs(Bit_RESET);
	hw_wireless_spi_writereg(RF_WRITE_REG|addr);
    for (i = 0; i < len; i++)
    {
    	hw_wireless_spi_writereg(buf[i]);
    }
	hw_wireless_spi_cs(Bit_SET);
}

/*************************************************
  Function:		hw_wireless_spi_readbuf
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_wireless_spi_readbuf(uint16_t addr, uint16_t* buf, uint16_t len)
{
	int i;
	hw_wireless_spi_cs(Bit_RESET);
	hw_wireless_spi_writereg(RF_READ_REG|addr);
    for (i = 0; i < len; i++)
    {	
   		buf[i]= hw_wireless_spi_writereg(RF_NOP);
    }
	hw_wireless_spi_cs(Bit_SET);
}

/*************************************************
  Function:		hw_wireless_tx_mode
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_wireless_tx_mode(void)
{
	hw_wireless_spi_ce(Bit_RESET);
	hw_wireless_spi_writecmd(RF_FLUSH_TX,0);
	uint16_t value = hw_wireless_spi_readbyte(RF_CONFIG);
	value = value|0x02;
	value = value&0xfe;
  	hw_wireless_spi_writebyte(RF_CONFIG, value);
	hw_wireless_spi_ce(Bit_SET);
}

/*************************************************
  Function:		hw_wireless_rx_mode
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_wireless_rx_mode(void)
{
	hw_wireless_spi_ce(Bit_RESET);
	hw_wireless_spi_writecmd(RF_FLUSH_RX,0);
	uint16_t value = hw_wireless_spi_readbyte(RF_CONFIG);
	value = value|0x03;
  	hw_wireless_spi_writebyte(RF_CONFIG, value);
	hw_wireless_spi_ce(Bit_SET);
}

/*************************************************
  Function:		hw_wireless_set_address
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_wireless_set_tx_addr(uint16_t *buf, uint16_t len)
{
	hw_wireless_spi_ce(Bit_RESET);
	hw_wireless_spi_writebuf(RF_TX_ADDR, buf, len);  
	hw_wireless_spi_ce(Bit_SET);
}

/*************************************************
  Function:		hw_wireless_set_address
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_wireless_set_rx_addr(uint16_t ch, uint16_t *buf, uint16_t len)
{
	hw_wireless_spi_ce(Bit_RESET);
	hw_wireless_spi_writebuf(RF_RX_ADDR_P0+ch, buf, len); 
	hw_wireless_spi_ce(Bit_SET);
}

/*************************************************
  Function:		hw_wireless_init_mode
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_wireless_init_mode(void)
{
	uint8_t i;
	uint16_t Reg[REG_NUM][2]=
	{
		{RF_CONFIG,0x0F},
		{RF_EN_AA,0x01},
		{RF_EN_RXADDR,0x01},
		{RF_SETUP_AW,0x03},
		{RF_SETUP_RETR,0x1a},
		{RF_CH,40},
		{RF_SETUP,0X26},
		{RF_STATUS,0xff},
		{RF_OBSERVE_TX,0x00},
		{RF_CD,0x00},

		{RF_RX_PW_P0,0x20},
		{RF_RX_PW_P1,0x20},
		{RF_RX_PW_P2,0x20},
		{RF_RX_PW_P3,0x20},
		{RF_RX_PW_P4,0x20},
		{RF_RX_PW_P5,0x20},
	};

	hw_wireless_spi_ce(Bit_RESET);
	for(i = 0; i < REG_NUM; i++)
	{
		hw_wireless_spi_writebyte(Reg[i][0], Reg[i][1]);
	}
	hw_wireless_spi_ce(Bit_SET);
}

/*************************************************
  Function:		hw_wireless_send
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_wireless_send(uint16_t* buf, uint16_t len)
{
	int i;
	hw_wireless_spi_cs(Bit_RESET);
	hw_wireless_spi_writereg(RF_WR_TX_PLOAD);
	for(i = 0; i < len; i++)
	{
		hw_wireless_spi_writereg(buf[i]);	
	}
	hw_wireless_spi_cs(Bit_SET);
}


/*************************************************
  Function:		hw_wireless_recv
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_wireless_recv(uint16_t* buf, uint16_t len)
{
	int i;
	hw_wireless_spi_cs(Bit_RESET);
	uint16_t value = hw_wireless_spi_writereg(RF_RD_RX_PLOAD);
	for (i = 0; i < len; i++)
    {	
   		buf[i]= hw_wireless_spi_writereg(RF_NOP);
    }
	hw_wireless_spi_cs(Bit_SET); 
}

/*************************************************
  Function:		hw_wireless_printf
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_wireless_printf(uint16_t reg)
{
	uint16_t address[5];
	uint16_t value;
	
	switch (reg)
	{
		case RF_CONFIG:
			value = hw_wireless_spi_readbyte(RF_CONFIG);
			printf("RF_CONFIG [%x]\n",value);
			break;
			
		case RF_EN_AA:
			value = hw_wireless_spi_readbyte(RF_EN_AA);
			printf("RF_EN_AA [%x]\n",value);
			break;
			
		case RF_EN_RXADDR:
			value = hw_wireless_spi_readbyte(RF_EN_RXADDR);
			printf("RF_EN_RXADDR [%x]\n",value);
			break;
			
		case RF_SETUP_AW:
			value = hw_wireless_spi_readbyte(RF_SETUP_AW);
			printf("RF_SETUP_AW [%x]\n",value);
			break;
			
		case RF_SETUP_RETR:
			value = hw_wireless_spi_readbyte(RF_SETUP_RETR);
			printf("RF_SETUP_RETR [%x]\n",value);
			break;
			
		case RF_CH:
			value = hw_wireless_spi_readbyte(RF_CH);
			printf("RF_CH [%x]\n",value);
			break;
			
		case RF_SETUP:
			value = hw_wireless_spi_readbyte(RF_SETUP);
			printf("RF_SETUP [%x]\n",value);
			break;
			
		case RF_STATUS:
			value = hw_wireless_spi_readbyte(RF_STATUS);
			printf("RF_STATUS [%x]\n",value);
			break;
			
		case RF_OBSERVE_TX:
			value = hw_wireless_spi_readbyte(RF_OBSERVE_TX);
			printf("RF_OBSERVE_TX [%x]\n",value);
			break;
			
		case RF_CD:
			value = hw_wireless_spi_readbyte(RF_CD);
			printf("RF_CD [%x]\n",value);
			break;
			
		case RF_RX_ADDR_P0:
			hw_wireless_spi_readbuf(reg, address, 5);
			printf("RF_RX_ADDR_P0 [%x %x %x %x %x]\n",address[0],address[1],address[2],address[3],address[4]);
			break;
		case RF_RX_ADDR_P1:
			hw_wireless_spi_readbuf(reg, address, 5);
			printf("RF_RX_ADDR_P1 [%x %x %x %x %x]\n",address[0],address[1],address[2],address[3],address[4]);
			break;
		case RF_RX_ADDR_P2:
			hw_wireless_spi_readbuf(reg, address, 5);
			printf("RF_RX_ADDR_P2 [%x %x %x %x %x]\n",address[0],address[1],address[2],address[3],address[4]);
			break;
		case RF_RX_ADDR_P3:
			hw_wireless_spi_readbuf(reg, address, 5);
			printf("RF_RX_ADDR_P3 [%x %x %x %x %x]\n",address[0],address[1],address[2],address[3],address[4]);
			break;
		case RF_RX_ADDR_P4:
			hw_wireless_spi_readbuf(reg, address, 5);
			printf("RF_RX_ADDR_P4 [%x %x %x %x %x]\n",address[0],address[1],address[2],address[3],address[4]);
			break;
		case RF_RX_ADDR_P5:
			hw_wireless_spi_readbuf(reg, address, 5);
			printf("RF_RX_ADDR_P5 [%x %x %x %x %x]\n",address[0],address[1],address[2],address[3],address[4]);
			break;
		case RF_TX_ADDR:
			hw_wireless_spi_readbuf(reg, address, 5);
			printf("RF_TX_ADDR [%x %x %x %x %x]\n",address[0],address[1],address[2],address[3],address[4]);
			break;
			
		case RF_RX_PW_P0:
			value = hw_wireless_spi_readbyte(RF_RX_PW_P0);
			printf("RF_RX_PW_P0 [%x]\n",value);
			break;
		case RF_RX_PW_P1:
			value = hw_wireless_spi_readbyte(RF_RX_PW_P1);
			printf("RF_RX_PW_P1 [%x]\n",value);
			break;
		case RF_RX_PW_P2:
			value = hw_wireless_spi_readbyte(RF_RX_PW_P2);
			printf("RF_RX_PW_P2 [%x]\n",value);
			break;
		case RF_RX_PW_P3:
			value = hw_wireless_spi_readbyte(RF_RX_PW_P3);
			printf("RF_RX_PW_P3 [%x]\n",value);
			break;
		case RF_RX_PW_P4:
			value = hw_wireless_spi_readbyte(RF_RX_PW_P4);
			printf("RF_RX_PW_P4 [%x]\n",value);
			break;
		case RF_RX_PW_P5:
			value = hw_wireless_spi_readbyte(RF_RX_PW_P5);
			printf("RF_RX_PW_P5 [%x]\n",value);
			break;
			
		case RF_FIFO_STATUS:
			value = hw_wireless_spi_readbyte(RF_FIFO_STATUS);
			printf("RF_FIFO_STATUS [%x]\n",value);
			break;

	}
}

/*************************************************
  Function:		hw_wireless_interrupt
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_wireless_interrupt(void)
{
	if(EXTI_GetITStatus(WIRELESS_SPI_EXTI) != RESET)
	{	
		uint16_t state = hw_wireless_spi_readbyte(RF_STATUS);
		if(state && RF_STA_MARK_TX)
		{
			hw_wireless_spi_writecmd(RF_FLUSH_TX,0);
		}

		if(state && RF_STA_MARK_RX)
		{
			uint16_t buf[2];
			hw_wireless_recv(buf,2);
			hw_wireless_spi_writecmd(RF_FLUSH_RX,0);			
		}
		hw_wireless_spi_writebyte(RF_STATUS,0xFF);
		EXTI_ClearFlag(WIRELESS_SPI_EXTI);
	}
}

/*************************************************
  Function:		hw_wireless_init
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_wireless_init(void)
{
	/* PCLK2 = HCLK/2 */
	RCC_PCLK2Config(RCC_HCLK_Div2); 

	/* Enable SPI clock and GPIO clock for SPI */
	RCC_APB2PeriphClockCmd(WIRELESS_SPI_GPIO_CLK|WIRELESS_SPI_IRQ_CLK, ENABLE);
	
	/* Enable SPI_SLAVE Periph clock */
	RCC_APB1PeriphClockCmd(WIRELESS_SPI_CLK, ENABLE);

	/* Configure SPI pins: NSS, SCK and MOSI */
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = WIRELESS_SPI_PIN_SCK | WIRELESS_SPI_PIN_MOSI;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(WIRELESS_SPI_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = WIRELESS_SPI_PIN_NSS|WIRELESS_SPI_PIN_CE;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(WIRELESS_SPI_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  WIRELESS_SPI_PIN_MISO;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(WIRELESS_SPI_GPIO, &GPIO_InitStructure);

	/* Configure PA.00 pin as input floating */
	GPIO_InitStructure.GPIO_Pin = WIRELESS_SPI_PIN_IRQ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(WIRELESS_SPI_IRQ_GPIO, &GPIO_InitStructure);

	/* Enable AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	/* Connect EXTI0 Line to PA.00 pin */
	GPIO_EXTILineConfig(WIRELESS_SPI_IRQ_RES, WIRELESS_SPI_GPIO_RES);
	
	/* Configure EXTI0 line */
	EXTI_InitTypeDef   EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = WIRELESS_SPI_EXTI;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI0 Interrupt to the lowest priority */
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = WIRELESS_SPI_RX_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	EXTI_ClearFlag(WIRELESS_SPI_EXTI);
	
 	/* Spi Init*/
	SPI_InitTypeDef  SPI_InitStructure;
	SPI_Cmd(WIRELESS_SPI, DISABLE);  
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(WIRELESS_SPI, &SPI_InitStructure);
	SPI_SSOutputCmd(WIRELESS_SPI, ENABLE);
	SPI_Cmd(WIRELESS_SPI, ENABLE);
	
	uint16_t TX_ADDRESS[5];
	RF_SET_DEFAULT_ADDR(TX_ADDRESS);
	hw_wireless_init_mode();
	hw_wireless_set_tx_addr(TX_ADDRESS, 5);
	hw_wireless_set_rx_addr(0, TX_ADDRESS, 5);
	hw_wireless_rx_mode();
    int i;
	hw_serial_printf("**************************************************\n");
	for (i = 0; i < (RF_FIFO_STATUS+1); i++)
	{
		hw_wireless_printf(i);
	}
	hw_serial_printf("**************************************************\n");	
}
#endif
