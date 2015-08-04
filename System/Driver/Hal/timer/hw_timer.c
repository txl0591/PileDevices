/*********************************************************
 Copyright (C),2011-2017,NewLand Auto-ID Tech.,Ltd.
 File name: 		hw_timer.c
 Author: 			Tangxl
 Version: 			1.0
 Date: 				2014-10-5
 Description: 		
 History: 			
 					
   1.Date:	 		2014-10-5
 	 Author:	 	Tangxl
 	 Modification:  Created file
 	 
*********************************************************/
#include "hw_define.h"

static TIME_BASE mTimerBase[MAX_TIMER_BASE];
/*************************************************
  Function:		hw_timer_delay_us
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_timer_delay_us(uint16_t us)
{
	uint16_t n;		    
	while(us--)
	{
		for(n = 0; n < TIME_CLOCK; n++); 	 
	}
}

/*************************************************
  Function:		hw_timer_delay_ms
  Description:  
  Input:		
  Output:		
  Return:		
  Others:
*************************************************/
void hw_timer_delay_ms(uint16_t ms)
{
	while(ms--)
	{
		hw_timer_delay_us(1000);	 
	}
}


/*************************************************
 Function:		hw_time_find_index
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
static int hw_time_find_index(int type)
{
    int i,ret = -1;
    for (i = 0 ; i < MAX_TIMER_BASE; i++)
    {
        if(mTimerBase[i].Id == type)
        {
            ret = i;
            break;
        }
    }

    return ret;
}

/*************************************************
 Function:		hw_time_run_proc
 Descroption:	 
 Input: 
	1.type
 Output: 
 Return: 	
 Other:  
*************************************************/
static int hw_time_run_proc(int type)
{
    int index = hw_time_find_index(type);
    if(index != -1)
    {
        if(mTimerBase[index].Proc)
        {
            mTimerBase[index].Proc(mTimerBase[index].Param);
        }
    }
    
    return 0;
}
/*************************************************
 Function:		TIM2_IRQHandler
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
void TIM2_IRQHandler(void)
{
    uint16_t capture;
    if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
        capture = TIM_GetCapture1(TIM2);       
        hw_time_run_proc(TIME_1MS);
        TIM_SetCompare1(TIM2, capture + TIME_1MS_PULSE);
    }
    else if (TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
        capture = TIM_GetCapture2(TIM2);
        hw_time_run_proc(TIME_2MS);
        TIM_SetCompare2(TIM2, capture + TIME_2MS_PULSE);
    }
    else if (TIM_GetITStatus(TIM2, TIM_IT_CC3) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
        capture = TIM_GetCapture3(TIM2);
        hw_time_run_proc(TIME_5MS);
        TIM_SetCompare3(TIM2, capture + TIME_5MS_PULSE);
    }
    else
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);
        capture = TIM_GetCapture4(TIM2);
        hw_time_run_proc(TIME_10MS);
        TIM_SetCompare4(TIM2, capture + TIME_10MS_PULSE);
    }
}

/*************************************************
 Function:		hw_timer_set_proc
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
void hw_timer_set_proc(uint8_t type, TimeProc Proc, void* param)
{
    int index = hw_time_find_index(TIME_NONE);

    if(-1 != index)
    {
        mTimerBase[index].Id = type;
        mTimerBase[index].Proc = Proc;      
        mTimerBase[index].Param = param;    
    }
}

/*************************************************
 Function:		hw_timer_start
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
void hw_timer_start(void)
{   
    TIM_Cmd(TIM2, ENABLE);  
}

/*************************************************
 Function:		hw_timer_stop
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:  
*************************************************/
void hw_timer_stop(void)
{   
    TIM_Cmd(TIM2, DISABLE);  
}

/*************************************************
 Function:		hw_timer_init
 Descroption:	 
 Input: 		None
 Output: 
 Return: 	
 Other:
 ((1+TIM_Prescaler )/72M)*(1+TIM_Period )=((1+7199)/72M)*(1+9999)=1秒
 TIM_Period 自动重装载寄存器周期的值
 TIM_Prescaler 时钟频率=72/(时钟预分频+1)
*************************************************/
void hw_timer_init(void)
{
    uint8_t i;
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    uint16_t PrescalerValue = 0;

    RCC_PCLK1Config(RCC_HCLK_Div4);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    /* Enable the TIM2 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);

    PrescalerValue = (uint16_t) (SystemCoreClock / 12000000) - 1;

    TIM_TimeBaseStructure.TIM_Period = 65535; 
    TIM_TimeBaseStructure.TIM_Prescaler = 0; 
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    /* Prescaler configuration */
    TIM_PrescalerConfig(TIM2, PrescalerValue, TIM_PSCReloadMode_Immediate);

    /* Output Compare Timing Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = TIME_1MS_PULSE;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OC1Init(TIM2, &TIM_OCInitStructure);

    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);

    /* Output Compare Timing Mode configuration: Channel2 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = TIME_2MS_PULSE;

    TIM_OC2Init(TIM2, &TIM_OCInitStructure);

    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Disable);

    /* Output Compare Timing Mode configuration: Channel3 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = TIME_5MS_PULSE;

    TIM_OC3Init(TIM2, &TIM_OCInitStructure);

    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Disable);

    /* Output Compare Timing Mode configuration: Channel4 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = TIME_10MS_PULSE;

    TIM_OC4Init(TIM2, &TIM_OCInitStructure);

    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Disable);

    /* TIM IT enable */
    TIM_ITConfig(TIM2, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4, ENABLE);


    for (i = 0 ; i < MAX_TIMER_BASE; i++)
    {
        mTimerBase[i].Id = TIME_NONE;
    }
    
}
