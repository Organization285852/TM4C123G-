#ifndef _ULTRASONIC_H
#define _ULTRASONIC_H

#include "includes.h"

void Ultrasonic_Init(void);
void Ultra_Duty(void);
void Ultra_Get(u8);

extern s8 ultra_start_f;
extern u16 ultra_distance;
extern s16 ultra_delta; //100ms 的高度变化量。
extern u8 ultra_ok;
#endif


