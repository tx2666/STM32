#ifndef __PID_H
#define __PID_H

/* 变量类型部分 */
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
	float PrevError;
	float CurrError;
	float SumError;
	uint16_t Count1;
} PID_Typedef;


/* 函数部分 */
void PID_TypedefStructInit(PID_Typedef *PID_Struct);
void PID_Motor_Control(uint8_t Motor_Num, PID_Typedef *pid);

#endif
