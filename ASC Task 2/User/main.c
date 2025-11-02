#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "Key.h"
#include "Timer.h"
#include "Encoder.h"
#include <string.h>

uint8_t KeyNum;
uint8_t Mode = 0;

int main(void)
{
	// 初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	// 好像初始化的顺序对程序有影响
	Key_Init();
	Timer_Init();
	Serial_Init();
	OLED_Init();
	Encoder_Init();
	
	while (1)
	{
		KeyNum = Key_GetNum();
		if (KeyNum == 1)  // 模式切换
		{
			Mode = !Mode;
			// 避免切换时OLED产生闪烁
			if (Mode == 0)
			{
				OLED_ShowString(1, 1, "   ");  // 3个空格，去掉PID
			}
			else if (Mode == 1)
			{
				OLED_ShowString(1, 4, "        ");  // 7个空格
				OLED_ShowString(2, 1, "           ");  // 10个空格				
			}
		}
		
		if (Mode == 0)
		{
			OLED_ShowChar(1, 16, '0');
			// 编码器测速与电机速度环
			int16_t count1, count2;
			count1 = Encoder1_Count;  // 在Encoder.c
			OLED_ShowString(1, 1, "CNT1:");
			OLED_ShowSignedNum(1, 6, count1, 5);
			OLED_ShowString(2, 1, "CNT2:");
			OLED_ShowSignedNum(2, 6, count2, 5);
		}
		else if (Mode == 1)
		{
			OLED_ShowChar(1, 16, '1');
			// PID运用 电机传动系统
			OLED_ShowString(1, 1, "PID");
		}
	}
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Key_Tick();
		Encoder_Tick();
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
