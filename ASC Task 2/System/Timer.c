#include "stm32f10x.h"                  // Device header


void Timer_Init(void)
{
	// 开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	// 设置内部时钟
	TIM_InternalClockConfig(TIM1);
	// 初始化
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 10;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1; // 72 000 000 / 7 200 = 10 000
	// 1ms进一次中断
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);
	// 清除中断位
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);
	// 启用中断
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
	// NVIC配置
	// NVIC_PriorityGroupConfig主函数写了，这里不写
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM1, ENABLE);
}

/*
 * 中断函数，哪里需要扔哪里
void TIM1_UP_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
	{
		
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}
}
*/
