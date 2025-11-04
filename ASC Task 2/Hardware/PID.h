#ifndef __PID_H
#define __PID_H

/* 变量类型部分 */
typedef struct {
	float Magnification;  	// 倍数，表示最终结果要乘多少倍
	float Kp;				// 比例项系数
	float Ki;				// 积分项系数
	float Kd;				// 微分项系数
	float Current;			// 当前速度
	float Target;			// 目标速度
	float P;				// 比例项结果
	float I;				// 积分项结果
	float D;				// 微分项结果
	float Out;				// 输出
	float PrevError;		// 上次误差
	float PrevPrevError;	// 上上次误差
	float CurrError;		// 当前误差
	float SumError;			// 误差积分
	uint16_t Count1;
	uint16_t Count2;
} PID_Typedef;

typedef enum {
	POSTION = 0,
	ADDITION = 1
} PID_Mode;

/* 函数部分 */
void PID_TypedefStructInit(PID_Typedef *PID_Struct);							// 结构体初始化赋值
void PID_Motor_Control(uint8_t Motor_Num, PID_Typedef *pid, PID_Mode Mode);
void PID_TypedefStructReset(PID_Typedef *PID_Struct);							// 除参数外全部清零

#endif
