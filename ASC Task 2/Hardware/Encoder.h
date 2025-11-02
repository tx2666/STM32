#ifndef __ENCODER_H
#define __ENCODER_H

extern int16_t Encoder1_Count;
extern int16_t Encoder2_Count;

void Encoder_Init(void);
int16_t Encoder1_Get(void);
int16_t Encoder2_Get(void);
void Encoder_Tick(void);

#endif
