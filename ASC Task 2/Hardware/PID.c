#include "stm32f10x.h"                  // Device header
#include "Motor.h"
#include "Serial.h"

typedef struct {
	float Magnification;  	// 倍数，表示最终结果要乘多少倍
	float Kp;				// 比例项系数
	float Ki;				// 积分项系数
	float Kd;				// 微分项系数
	float Current_Speed;	// 当前速度
	float Target_Speed;		// 目标速度
	float P;
	float I;
	float D;
	float Out;
} PID_Typedef;

void PID_Motor_Control(uint8_t Motor_Num, PID_Typedef *pid)
{
	static float PrevError = 0;
	static float CurrError = 0;
	static float SumError = 0;
	static uint16_t Count1 = 0;
	Count1 ++ ;
	if (Count1 >= 10)
	{
		PrevError = CurrError;
		CurrError = pid->Target_Speed - pid->Current_Speed;
		SumError += CurrError;
		if (SumError > 1000 / pid->Ki)
		{
			SumError = 1000 / pid->Ki;
		}
		// PID计算
		pid->P = pid->Kp * CurrError;
		pid->I = pid->Ki * SumError;
		pid->D = pid->Kd * (CurrError - PrevError) / Count1;
		
		// 输出计算
		pid->Out = pid->P + pid->I + pid->D;
		pid->Out *= pid->Magnification;
		
		if (pid->Out > 1000)
		{
			pid->Out = 1000;
		}
		else if (pid->Out < -1000)
		{
			pid->Out = -1000;
		}
		// 控制输出
		if (Motor_Num == 1)
		{
			Motor1_SetSpeed(pid->Out);
		}
		else if (Motor_Num == 2)
		{
		}
		else
		{
			Serial_Printf("PID Motor Error\r\n");
		}
		// 信息发送

		
		Count1 = 0;
	}
}

void PID_TypedefStructInit(PID_Typedef *PID_Struct)
{
	PID_Struct->Magnification = 1;
	PID_Struct->Kp = 1;
	PID_Struct->Ki = 0.01;
	PID_Struct->Kd = 1;
	PID_Struct->Current_Speed = 0;
	PID_Struct->Target_Speed = 0;
	PID_Struct->P = 0;
	PID_Struct->I = 0;
	PID_Struct->D = 0;
	PID_Struct->Out = 0;
}
