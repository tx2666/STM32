#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "Key.h"
#include "Timer.h"
#include <string.h>

uint8_t KeyNum;
uint8_t Mode = 0;

int main(void)
{
	// 初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Timer_Init();
	OLED_Init();
	Key_Init();
	Serial_Init();
	
	while (1)
	{
		KeyNum = Key_GetNum();
		if (KeyNum == 1)
		{
			Mode = !Mode;
		}
		
		if (Mode == 0)
		{
			OLED_ShowChar(1, 16, '0');
			// 编码器测速与电机速度环
		}
		else if (Mode == 1)
		{
			OLED_ShowChar(1, 16, '1');
			// PID运用 电机传动系统
		}
	}
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Key_Tick();
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
