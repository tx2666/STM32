#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "Key.h"
#include "LED.h"
#include <string.h>

uint8_t KeyNum;

int main(void)
{
	OLED_Init();
	Key_Init();
	Serial_Init();
	LED_Init();
	
	OLED_ShowString(1, 1, "TxPacket");
	OLED_ShowString(3, 1, "RxPacket");
	
	Serial_SendPacket();
	
	while (1)
	{
		if (Serial_GetRxFlag() == 1)
		{
			OLED_ShowString(4, 1, "                ");  // 清空
			OLED_ShowString(4, 1, Serial_RxPacket);
			if (strcmp(Serial_RxPacket, "LED_ON") == 0)
			{
				LED1_On();
				OLED_ShowString(2, 1, "                ");
				OLED_ShowString(2, 1, "LED_ON_OK");
				Serial_Printf("LED_ON_OK");
			}
			else if (strcmp(Serial_RxPacket, "LED_OFF") == 0)
			{
				LED1_Off();
				OLED_ShowString(2, 1, "                ");
				OLED_ShowString(2, 1, "LED_OFF_OK");
				Serial_Printf("LED_OFF_OK");
			}
			else
			{
				OLED_ShowString(2, 1, "                ");
				OLED_ShowString(2, 1, "ERROR");
				Serial_Printf("ERROR");
			}
		}
	}
}
