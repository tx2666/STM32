#include "stm32f10x.h"                  // Device header
#include "Motor.h"
#include "Serial.h"

typedef struct {
	float Kp;
	float Ki;
	float Kd;
} PID_Parameter;

void PID_Motor_Control(float CurrSpeed, float TargetSpeed, PID_Parameter pid)
{
	static float PrevError = 0;
	static float CurrError = 0;
	static float SumError = 0;
	static uint16_t Count1 = 0;
	Count1 ++ ;
	if (Count1 >= 10)
	{
		PrevError = CurrError;
		CurrError = TargetSpeed - CurrSpeed;
		SumError += CurrError;
		float Num_P = pid.Kp * CurrError;
		float Num_I = pid.Ki * SumError;
		float Num_D = pid.Kd * (CurrError - PrevError) / Count1;
		float Out = Num_P + Num_I + Num_D;
		if (Out > 900)
		{
			Out = 900;
		}
		else if (Out < 255)
		{
			Out = 255;
		}
		Serial_Printf("PID:%f, %f ,%f", Num_P, Num_I, Num_D);
		Serial_Printf("Out: %f", Out);
		Count1 = 0;
	}
}
