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
	float P;				// 比例项结果
	float I;				// 积分项结果
	float D;				// 微分项结果
	float Out;				// 输出
	float PrevError;		// 上次误差
	float PrevPrevError;	// 上上次误差
	float CurrError;		// 当前误差
	float SumError;			// 误差积分
	uint16_t Count1;
} PID_Typedef;

void PID_Motor_Control(uint8_t Motor_Num, PID_Typedef *pid)
{
	pid->Count1 ++ ;
	if (pid->Count1 >= 10)
	{
		pid->PrevPrevError = pid->PrevError;
		pid->PrevError = pid->CurrError;
		pid->CurrError = pid->Target_Speed - pid->Current_Speed;
		pid->SumError += pid->CurrError;
		// PID计算
		pid->P = (pid->Kp) * (pid->CurrError - pid->PrevError);
		pid->I = (pid->Ki) * (pid->CurrError);
		pid->D = (pid->Kd) * (pid->CurrError - 2 * pid->PrevError + pid->PrevPrevError);
		
		// 输出计算
		pid->Out += pid->P + pid->I + pid->D;
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
			Motor2_SetSpeed(pid->Out);
		}
		else
		{
			Serial_Printf("PID Motor Error\r\n");
		}
		// 信息发送

		
		pid->Count1 = 0;
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
	PID_Struct->PrevError = 0;
	PID_Struct->PrevPrevError = 0;	
	PID_Struct->CurrError = 0;
	PID_Struct->SumError = 0;
	PID_Struct->Count1 = 0;
}
