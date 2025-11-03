#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
#include "Encoder.h"

uint8_t Serial_TxPacket[4];
char Serial_RxPacket[100];
uint8_t Serial_RxFlag;

void Serial_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;  // 波特率
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  // 硬件流控制
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;  // 是否添加校验位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;  // 停止位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);
	
	// 中断方法要开NVIC
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1, ENABLE);
}


void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

void Serial_SendArray(uint8_t *Array, uint8_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i++)
	{
		Serial_SendByte(Array[i]);
	}
}

void Serial_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		Serial_SendByte(String[i]);
	}
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while(Y--)
	{
		Result *= X;
	}
	return Result;
}

void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
	}
}

void Serial_SendPacket(void)
{
	Serial_SendByte(0xFF);
	Serial_SendArray(Serial_TxPacket, 4);
	Serial_SendByte(0xFE);
}
void Serial_Printf(char *format, ...)  // C语言可变参数写法
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(String);
}

uint8_t Serial_GetRxFlag(void)
{
	if (Serial_RxFlag == 1)
	{
		Serial_RxFlag = 0;
		return 1;
	}
	return 0;
}


void USART1_IRQHandler(void)
{
	static uint8_t RxState = 0;
	static uint8_t pRxData;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		char RxData = USART_ReceiveData(USART1);
		
		if (RxState == 0)
		{
			// 等待包头
			if (RxData == '@' && Serial_RxFlag == 0)
			{
				RxState = 1;
				pRxData = 0;
			}
		}
		else if (RxState == 1)
		{
			// 接收数据
			if (RxData == '\r')
			{
				RxState = 2;
			}
			else 
			{
				Serial_RxPacket[pRxData] = RxData;
				pRxData ++;
			}
		}
		else if (RxState == 2)
		{
			// 等待包尾
			if (RxData == '\n')
			{
				RxState = 0;
				Serial_RxFlag = 1;
				Serial_RxPacket[pRxData] = '\0';
			}
		}
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}

void Serial_Tick(void)
{
	static uint16_t Count1 = 0;
	Count1 ++;
	if (Count1 >= 10)
	{
		float ECount1 = (float)Encoder1_Count;
		float ECount2 = (float)Encoder2_Count;
		Serial_Printf("Data:%f, %f\r\n", ECount1, ECount2);
		
		Count1 = 0;
	}
}
