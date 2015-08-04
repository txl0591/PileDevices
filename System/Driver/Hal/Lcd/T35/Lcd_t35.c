/*********************************************************
 Copyright (C),2012-2018,Electronic Technology Co.,Ltd.
 File name: 		Lcd_t35.c
 Author: 			Txl
 Version: 			1.0
 Date: 				2015-1-31
 Description: 		
 History: 			
 					
   1.Date:	 		2015-1-31
 	 Author:	 	Txl
 	 Modification:  Created file
 	 
*********************************************************/
#include "hw_define.h"
#include "Lcd_t35.h"
#include "log.h"

#if(LEVEL_DEBUG && LEVEL_DRIVER)
#define LOGD        hw_serial_printf
#else
#define LOGD        hw_serial_printf_null  
#endif


static const T35InitCmd mT35Init[] = 
{
   {0x0001, 0, {0}, 200},
   {0x0011, 0, {0}, 30},
   {0x00D0, 3, {0x0007,0x0041,0x001C}, 0},
   {0x00D1, 3, {0x0000,0x0036,0x001B}, 0}, 
   {0x00D2, 2, {0x0001,0x0011}, 0}, 
   {0x00C0, 5, {0x0010,0x003B,0x0000,0x0012,0x0001}, 0},  
   {0x00C5, 1, {0x0003}, 0},     
   {0x00C8, 12,{0x0000,0x0057,0x0033,0x0000,0x0000,0x0000,0x0044,0x0002,0x0077,0x0000,0x0000,0x0000}, 0}, 
   {0x00F8, 1, {0x0001}, 0}, 
   {0x00FE, 2, {0x0000,0x0002}, 0},
   {0x0036, 1, {0x000A}, 30},   
   {0x003a, 1, {0x0055}, 30},   
   {0x0029, 0, {0}, 20},
   {0x0036, 1, {0x0068}, 0},  
};

/*************************************************
 Function:		Lcd_t35_rs
 Descroption:	 
 Input: 
	1.state
 Output: 
 Return: 	
 Other:  
*************************************************/
static void Lcd_t35_rs(uint8_t state)
{
    if (state)	
	{
	    GPIO_SetBits(LCD_RS_GPIO,LCD_RS_PIN);	
    }
	else
    {   
	    GPIO_ResetBits(LCD_RS_GPIO,LCD_RS_PIN);
    }
}

/*************************************************
 Function:		Lcd_t35_writeCmd
 Descroption:	 
 Input: 
	1.cmd
 Output: 
 Return: 	
 Other:  
*************************************************/
static void Lcd_t35_writeCmd(int cmd)
{
	uint16_t value = 0;
    Lcd_t35_rs(0);
    value = hw_spi2_writereg(cmd);
    hw_serial_printf("Cmd [%x] =  %x\r\n",cmd,value);
}

/*************************************************
 Function:		Lcd_t35_writeData
 Descroption:	 
 Input: 
	1.data
 Output: 
 Return: 	
 Other:  
*************************************************/
static void Lcd_t35_writeData(int data)
{
    Lcd_t35_rs(1);
    hw_spi2_writereg(data);
}

/*************************************************
 Function:		Lcd_t35_chosebox
 Descroption:	 
 Input: 
	1.Xstart
	2.Xend
	3.Ystart
	4.Yend
 Output: 
 Return: 	
 Other:  
*************************************************/
static void Lcd_t35_chosebox(uint16_t Xstart, uint16_t Xend, uint16_t Ystart, uint16_t Yend) 
{
    Lcd_t35_writeCmd(0x2A);
    Lcd_t35_writeData(Xstart>>8);
    Lcd_t35_writeData(Xstart);
    Lcd_t35_writeData(Xend>>8);
    Lcd_t35_writeData(Xend);

    Lcd_t35_writeCmd(0x2B);
    Lcd_t35_writeData(Ystart>>8);
    Lcd_t35_writeData(Ystart);
    Lcd_t35_writeData(Yend>>8);
    Lcd_t35_writeData(Yend);

    Lcd_t35_writeCmd(0x2C);
}

/*************************************************
 Function:		Lcd_t35_Fill
 Descroption:	 
 Input: 
	1.xStart
	2.yStart
	3.xLong
	4.yLong
	5.Color
 Output: 
 Return: 	
 Other:  
*************************************************/
void Lcd_t35_Fill(uint16_t xStart,uint16_t yStart,uint16_t xLong,uint16_t yLong,uint16_t Color)
{
	uint16_t temp;
    uint16_t max1 = xLong*yLong;
    hw_spi2_cs_on();
	Lcd_t35_chosebox(xStart,xStart+xLong-1,yStart,yStart+yLong-1);
	for (temp=0; temp<max1; temp++)
	{
		Lcd_t35_writeData(Color>>8);
		Lcd_t35_writeData(Color);
	}
    hw_spi2_cs_off();
}

#define SPI_CS(a)	\
						if (a)	\
						GPIO_SetBits(GPIOA,GPIO_Pin_4);	\
						else		\
						GPIO_ResetBits(GPIOA,GPIO_Pin_4)
#define LCD_RS(a)	\
						if (a)	\
						GPIO_SetBits(GPIOA,GPIO_Pin_3);	\
						else		\
						GPIO_ResetBits(GPIOA,GPIO_Pin_3)

volatile static void Delay(__IO u32 nCount)
{	
	volatile int i;
	for(i=0;i<720;i++)
    for(; nCount != 0; nCount--);
}  
  
 void WriteComm(u16 CMD)
{			
	LCD_RS(0);
	SPI_I2S_SendData(SPI1, CMD);
}
  WriteData(u16 tem_data)
{			
	LCD_RS(1);
	SPI_I2S_SendData(SPI1, tem_data);
}
#define Lcd_Light_ON   GPIOA->BSRR = GPIO_Pin_2;
#define Lcd_Light_OFF  GPIOA->BRR  = GPIO_Pin_2;
/**********************************************
Lcd初始化函数
Initial condition  (DB0-15,RS,CSB,WRD,RDB,RESETB="L") 
***********************************************/
void Lcd_Initialize(void)
{	
int i;
SPI_CS(1);

WriteComm(0x0001);
Delay(100);
SPI_CS(0);
WriteComm(0x0011);
Delay(20);
WriteComm(0x00D0);    
WriteData(0x0007); 
WriteData(0x0041); 
WriteData(0x001C); 
 
WriteComm(0x00D1);    
WriteData(0x0000); 
WriteData(0x0036); //30 vcm
WriteData(0x001B); //15 vdv

                                                                 
WriteComm(0x00D2);
WriteData(0x0001);   // AP0[2:0]
WriteData(0x0011);   // DC10[2:0],DC00[2:0]
  
WriteComm(0x00C0);     //****** *******
WriteData(0x0010);   // REV & SM & GS
WriteData(0x003B);   // NL[5:0]
WriteData(0x0000);   // SCN[6:0]
WriteData(0x0012);   //02  NDL , PTS[2:0]
WriteData(0x0001);   //11 PTG , ISC[3:0]  
  
WriteComm(0x00C5);    
WriteData(0x0003);


WriteComm(0x00C8);
WriteData(0x0000);
WriteData(0x0057);
WriteData(0x0033);
WriteData(0x0000);
WriteData(0x0000);
WriteData(0x0000);
WriteData(0x0044);
WriteData(0x0002);
WriteData(0x0077);
WriteData(0x0000);
WriteData(0x0000);
WriteData(0x0000);
 
  
WriteComm(0x00F8);
WriteData(0x0001);
  
WriteComm(0x00FE);
WriteData(0x0000);
WriteData(0x0002);
  
WriteComm(0x0036);   // Set_address_mode
WriteData(0x000A);  // Bit3: RGB/BGR
Delay(20);
WriteComm(0x003a);   // Set_address_mode
WriteData(0x0055);  //05---16BIT,06---18BIT  
Delay(20);  

WriteComm(0x0029); 

Delay(10);
	WriteComm(0x36); //Set_address_mode
 	WriteData(0x68); //横屏，从左下角开始，从左到右，从下到上

Lcd_Light_ON;

// while(1)
{
WriteComm(0x36);//竖屏
WriteData(0x48);//
// 	while(!(SPI1->SR&0x0002));
for(i=0;i<320*480;i++)
{
	LCD_RS(1);
	SPI_I2S_SendData(SPI1, COLOR_BLUE>>8);
	LCD_RS(1);
	SPI_I2S_SendData(SPI1, COLOR_BLUE);
}
for(i=0;i<320*480;i++)
{
	LCD_RS(1);
	SPI_I2S_SendData(SPI1, COLOR_YELLOW>>8);
	LCD_RS(1);
	SPI_I2S_SendData(SPI1, COLOR_YELLOW);
}

WriteComm(0x36);
WriteData(0x28);//
}

}

/*
 * 函数名：LCD_GPIO_Config
 * 描述  ：根据FSMC配置LCD的I/O
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用        
 */
 void LCD_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* Enable the FSMC Clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
    
    /* config lcd gpio clock base on FSMC */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE , ENABLE);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    /* config tft back_light gpio base on the PT4101 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;		
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    /* config tft rst gpio */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_13 ; 	 
    GPIO_Init(GPIOC, &GPIO_InitStructure);  		   
    
    /* config tft data lines base on FSMC
	 * data lines,FSMC-D0~D15: PD 14 15 0 1,PE 7 8 9 10 11 12 13 14 15,PD 8 9 10
	 */	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 | 
                                  GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
                                  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                  GPIO_Pin_15;
    GPIO_Init(GPIOE, &GPIO_InitStructure); 
    
    /* config tft control lines base on FSMC
	 * PD4-FSMC_NOE  :LCD-RD
   * PD5-FSMC_NWE  :LCD-WR
	 * PD7-FSMC_NE1  :LCD-CS
   * PD11-FSMC_A16 :LCD-DC
	 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);  
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);  
    
    /* tft control gpio init */	 
    GPIO_SetBits(GPIOD, GPIO_Pin_13);		 // RST = 1 	
    GPIO_SetBits(GPIOC, GPIO_Pin_5);	 	 //	LIGHT
    GPIO_SetBits(GPIOD, GPIO_Pin_4);		 // RD = 1  
    GPIO_SetBits(GPIOD, GPIO_Pin_5);		 // WR = 1 
    GPIO_SetBits(GPIOD, GPIO_Pin_7);		 //	CS = 1 
		
																	//   BL      　　RS      CS
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);

	GPIO_SetBits(GPIOC, GPIO_Pin_13);	 	 //	CS2
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7 | GPIO_Pin_5|GPIO_Pin_11;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOD, GPIO_Pin_7);
}



/*************************************************
  Function:		Lcd_t35_init
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void Lcd_t35_init(void)
{
#if 0
    uint8_t i,j;
//    uint16_t value[6];
	GPIO_InitTypeDef GPIO_InitStructure;	


    
	RCC_APB2PeriphClockCmd(LCD_RS_GPIO_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = LCD_RS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(LCD_RS_GPIO, &GPIO_InitStructure);
    hw_spi2_cs_off();
    for (i = 0 ; i < 14; i++)
    {
        Lcd_t35_writeCmd(mT35Init[i].Addr);
        for (j = 0 ; j < mT35Init[i].len; j++)
        {
            Lcd_t35_writeData(mT35Init[i].data[j]);
        }
        if(mT35Init[i].delay > 0)
        {
            hw_timer_delay_ms(mT35Init[i].delay);
        }
        if(0 == i)
        {
            hw_spi2_cs_on();
        }
    }
	hw_spi2_cs_off();
    #endif
    LCD_GPIO_Config();
    Lcd_Initialize();
//    hw_spi2_readbuf(0xBF, value, 6);
//    hw_serial_printf("0xBF [%x %x %x",value[0],value[1],value[2]);
//    hw_serial_printf(" %x %x %x]\r\n",value[3],value[4],value[5]);

    
}

