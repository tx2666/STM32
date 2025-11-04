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

	Key_Init();
	
	Timer_Init();
	
	Serial_Init();
	
	OLED_Init();
	
	Encoder_Init();
	
	Motor_Init();
	
	PID_TypedefStructInit(&PID_Motor1);
	PID_Motor1.Magnification = 1;
	PID_Motor1.Kp = 0.3;
	PID_Motor1.Ki = 0.42;
	PID_Motor1.Kd = -0.35;
	PID_Motor1.Target = 0;
	// 极速为480左右
	PID_TypedefStructInit(&PID_Motor2);
	PID_Motor2.Magnification = 1;
	PID_Motor2.Kp = 1.13;
	PID_Motor2.Ki = 0.41;
	PID_Motor2.Kd = -3.5;
	PID_Motor2.Target = 0;
	// 极速为520左右
	
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
			OLED_ShowString(1, 1, "CNT1:");
			OLED_ShowString(2, 1, "CNT2:");
			if (Serial_GetRxFlag() == 1)  // 串口数据包解包
			{
				char Command[100];
				
				uint8_t i;
				uint8_t flag1 = 1;
				uint8_t Num_flag = 1;  // 1表示正数，0表示负数
				int16_t Num = 0;
				for (i = 0; Serial_RxPacket[i] != '\0'; i++)
				{
					char c = Serial_RxPacket[i];
					if (c == '%')
					{
						flag1 = 0;
					}
					else if (flag1)
					{
						Command[i] = c;
					}
					else if (!flag1)
					{
						if (c == '-')
						{
							Num_flag = 0;
						}
						else if (c == '+')
						{
							// 啥也不干
						}
						else 
						{
							Num *= 10;
							Num += c - '0';
						}
					}
				}
				if (strcmp(Command, "speed") == 0)
				{
					Num = (Num_flag == 0) ? -Num : Num;  // 确认正负
					PID_Motor1.Target = Num;
				}
				else 
				{
					Serial_Printf("Command Error");
				}
			}
			int16_t count1, count2;
			count1 = Encoder1_Count;  // 在Encoder.c
			count2 = Encoder2_Count;
			OLED_ShowSignedNum(1, 6, count1, 5);
			OLED_ShowSignedNum(2, 6, count2, 5);
			PID_Motor1.Current = count1;
			PID_Motor2.Current = count2;
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
		if (Mode == 0)
		{
			Serial_Printf("Data:%.2f, %.2f, %.2f, %.2f, %.2f, %.2f\r\n", (float)Encoder1_Count, 
				PID_Motor1.P, PID_Motor1.I, PID_Motor1.D, PID_Motor1.Out, PID_Motor1.Target);
		}
		else if (Mode == 1)
		{
			Serial_Printf("Data:%.2f, %.2f, %.2f, %.2f, %.2f, %.2f\r\n", (float)Encoder2_Count, 
				PID_Motor2.P, PID_Motor2.I, PID_Motor2.D, PID_Motor2.Out, PID_Motor2.Target);
		}
		
		Count1 = 0;
	}
}

void TIM1_UP_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
	{
		Key_Tick();
		Encoder_Tick();
		if (Mode == 0)  			// 验收题1
		{
			Motor2_SetSpeed(0);
			PID_TypedefStructReset(&PID_Motor2);
			PID_Motor_Control(1, &PID_Motor1);
		}
		else if (Mode == 1)  		// 验收题2
		{
			Motor1_SetSpeed(0);
			PID_TypedefStructReset(&PID_Motor1);
			PID_Motor_Control(2, &PID_Motor2);
		}
		Send_Data();
		
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}
}
