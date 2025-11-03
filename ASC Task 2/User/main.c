#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "Key.h"
#include "Timer.h"
#include "Encoder.h"
#include <string.h>
#include "Motor.h"
#include "PID.h"

PID_Typedef PID_Motor1;
PID_Typedef PID_Motor2;

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
	
	Motor_Init();
	
	Motor1_SetSpeed(0);
	
	PID_TypedefStructInit(&PID_Motor1);
	PID_Motor1.Magnification = 1;
	PID_Motor1.Kp = 1.13;
	PID_Motor1.Ki = 0.41;
	PID_Motor1.Kd = -3.5;
	PID_Motor1.Target_Speed = 0;
	// 极速为500左右
	PID_TypedefStructInit(&PID_Motor2);
	
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
			// 编码器测速与电机速度
			int16_t count1, count2;
			count1 = Encoder1_Count;  // 在Encoder.c
			count2 = Encoder2_Count;
			OLED_ShowString(1, 1, "CNT1:");
			OLED_ShowSignedNum(1, 6, count1, 5);
			OLED_ShowString(2, 1, "CNT2:");
			OLED_ShowSignedNum(2, 6, count2, 5);
			PID_Motor1.Current_Speed = count1;
		}
		else if (Mode == 1)
		{
			OLED_ShowChar(1, 16, '1');
			// PID运用 电机传动系统
			OLED_ShowString(1, 1, "PID");
		}
	}
}

void Send_Data()
{
	static uint16_t Count1 = 0;
	Count1 ++;
	if (Count1 >= 10)
	{
		Serial_Printf("Data:%f, %f, %f, %f, %f\r\n", (float)Encoder1_Count, 
			PID_Motor1.P, PID_Motor1.I, PID_Motor1.D, PID_Motor1.Out);
		
		Count1 = 0;
	}
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Key_Tick();
		Encoder_Tick();
		PID_Motor_Control(1, &PID_Motor1);
		Send_Data();
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
