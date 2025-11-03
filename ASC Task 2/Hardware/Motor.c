#include "stm32f10x.h"                  // Device header
#include "PWM.h"

void Motor_Init(void)
{
	// 开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	// GPIOB 12 13
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	PWM_Init();
	
	PWM_SetCompare2(0);
}

void Motor1_SetSpeed(int16_t Speed)
{
	if (Speed > 1000) Speed = 1000;
	else if (Speed < -1000) Speed = -1000;
	
	if (Speed >= 0)
	{
		PWM_SetCompare3(0);
		GPIO_SetBits(GPIOB, GPIO_Pin_12);
		GPIO_ResetBits(GPIOB, GPIO_Pin_13);
		PWM_SetCompare3(Speed);
	}
	else if (Speed < 0)
	{
		PWM_SetCompare3(0);
		GPIO_SetBits(GPIOB, GPIO_Pin_13);
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);
		PWM_SetCompare3(-Speed);	
	}
}

void Motor2_SetSpeed(int16_t Speed)
{
	if (Speed > 1000) Speed = 1000;
	else if (Speed < -1000) Speed = -1000;
	
	if (Speed >= 0)
	{
		PWM_SetCompare2(0);
		GPIO_SetBits(GPIOB, GPIO_Pin_10);
		GPIO_ResetBits(GPIOB, GPIO_Pin_11);
		PWM_SetCompare2(Speed);
	}
	else if (Speed < 0)
	{
		PWM_SetCompare2(0);
		GPIO_SetBits(GPIOB, GPIO_Pin_11);
		GPIO_ResetBits(GPIOB, GPIO_Pin_10);
		PWM_SetCompare2(-Speed);	
	}
}