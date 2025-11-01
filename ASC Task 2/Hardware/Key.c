#include "stm32f10x.h"                  // Device header
#include "Delay.h"

uint8_t Key_Num = 0;

void Key_Init (void) 
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入，默认高电平1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}


// 返回0为松开，返回1为按下
uint8_t Key_GetState(void)
{
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
	{
		return 1;
	}
	return 0;
}

uint8_t Key_GetNum(void)
{
	uint8_t temp = Key_Num;
	Key_Num = 0;
	return temp;
}

void Key_Tick(void)
{  // 只有切换作用，不做多功能按钮
	static uint16_t Count1;
	static uint8_t PrevState = 0, CurrState = 0;
	Count1 ++;
	if (Count1 >= 20)
	{	
		PrevState = CurrState;
		CurrState = Key_GetState();
		if (CurrState != 0 && PrevState == 0)  // 按下瞬间
		{
			Key_Num = 1;
		}
		else if (CurrState == 0 && PrevState == 0)  // 按住
		{
			
		}
		else if (CurrState == 0 && PrevState != 0)  // 松开瞬间
		{
			Count1 = 0;
		}
	}
}
