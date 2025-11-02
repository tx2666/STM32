#include "stm32f10x.h"                  // Device header
#include "Serial.h"

void PWM_Init(void)
{
	// 开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	// GPIOA 8 10
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 设置内部时钟
	TIM_InternalClockConfig(TIM1);
	
	// TIM1 时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;
	// 72 000 000 / 72 = 1 000 000
	// 1 000 000 / 1 000 = 1 000
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);
	
	// TIM1 Channel 1
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				// PWM模式1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		// 极性不反转
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	// 输出启用
	TIM_OCInitStructure.TIM_Pulse = 500;								// 占空比0/1000
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	/*
	// TIM1 Channel 4
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				// PWM模式1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		// 极性不反转
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	// 输出启用
	TIM_OCInitStructure.TIM_Pulse = 0;								// 占空比0/1000
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	*/
	// TIM1使能
	TIM_CtrlPWMOutputs(TIM1, ENABLE); // 否则无输出！
	TIM_Cmd(TIM1, ENABLE);
	
	Serial_Printf("PWM Init End\r\n");
}

void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM1, Compare);
}

void PWM_SetCompare4(uint16_t Compare)
{
	TIM_SetCompare4(TIM1, Compare);
}
