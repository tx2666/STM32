#include "stm32f10x.h"                  // Device header

void LED_Init(void) 
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2);
}

void LED1_On(void)  // 打开LED1
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
}

void LED1_Off(void)  // 关闭LED1
{
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
}

void LED1_Turn(void)  // LED1切换状态
{
	if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1) == 0)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_1);
	}
	else 
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_1);
	}

}

void LED2_On(void)  // 打开LED2
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_2);
}

void LED2_Off(void)  // 关闭LED2
{
	GPIO_SetBits(GPIOA, GPIO_Pin_2);
}

void LED2_Turn(void)  // LED2切换状态
{
	if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_2) == 0)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_2);
	}
	else 
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_2);
	}

}
