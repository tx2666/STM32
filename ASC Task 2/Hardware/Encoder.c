#include "stm32f10x.h"                  // Device header

int16_t Encoder1_Count;
int16_t Encoder2_Count;

void Encoder_Init(void)
{
	/* 开启时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	// 初始化
	/* 电机1 */ 
	// GPIOA 6 7
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 时基单元 TIM3 分配给电机1 编码器接口
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;  // ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;  // PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	// 输入捕获 初始化
	TIM_ICInitTypeDef TIM_ICInitStructure;                  		// 定义结构体变量
	TIM_ICStructInit(&TIM_ICInitStructure);              			// 赋初始值
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;  				// 选通道
	TIM_ICInitStructure.TIM_ICFilter = 0xF;  						// 滤波器
//	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;			// 不分频
//	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	// 下面这个设置编码器接口
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	// 下方开启时钟，对电机1测速的初始化结束
	TIM_Cmd(TIM3, ENABLE);
	
	/* 电机2 */
	// GPIOB 6 7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// 时基单元 TIM4 分配给电机2 编码器接口
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;  // ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;  // PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
	// 输入捕获 初始化
//	TIM_ICInitTypeDef TIM_ICInitStructure;                  		// 定义结构体变量
	TIM_ICStructInit(&TIM_ICInitStructure);              			// 赋初始值
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;  				// 选通道
	TIM_ICInitStructure.TIM_ICFilter = 0xF;  						// 滤波器
//	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;			// 不分频
//	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	// 下面这个设置编码器接口
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	// 下方开启时钟，对电机2测速的初始化结束
	TIM_Cmd(TIM4, ENABLE);
}

int16_t Encoder1_Get(void)
{
	int16_t temp = TIM_GetCounter(TIM3);
	TIM_SetCounter(TIM3, 0);
	return temp;
}

int16_t Encoder2_Get(void)
{
	int16_t temp = TIM_GetCounter(TIM4);
	TIM_SetCounter(TIM4, 0);
	return temp;
}

void Encoder_Tick(void)
{
	static uint16_t Count1 = 0;
	Count1 ++;
	
	if (Count1 >= 10)
	{
		Encoder1_Count = Encoder1_Get();
		Encoder2_Count = Encoder2_Get();
		
		Count1 = 0;
	}
}
