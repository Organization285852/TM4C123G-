#ifndef __HAL_TIMER_H
#define __HAL_TIMER_H

#include "includes.h"
void Timer0BInit(u16 us);//不能大于819,应为时钟为80M，而其为16位的

void SysTickInit(void);

void TIM_INIT(void);
void sys_time(void);

u16 Get_Time(u8,u16,u16);

float Get_Cycle_T(u8 );

void Cycle_Time_Init(void);

extern volatile uint32_t sysTickUptime;
extern int time_1h,time_1m,time_1s,time_1ms;

void Delay_us(uint32_t);
void Delay_ms(uint32_t ms);
uint32_t GetSysTime_us(void);

extern u8 Init_Finish;

#endif

