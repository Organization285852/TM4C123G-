
#include "includes.h"
#include "led.h"
#include "scheduler.h"
#include "hal_timer.h"
#include "led.h"
#include "usart.h"

void Timer0BInit(u16 us)
{  
	   //
    // The Timer0 peripheral must be enabled for use.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	 //
    // The Timer0 peripheral must be enabled for use.
    //
    TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PERIODIC);

    //
    // Set the Timer0B load value to 1ms.
    //
    TimerLoadSet(TIMER0_BASE, TIMER_B,SysCtlClockGet()/1000000*us);//不能大于819,应为时钟为80M，而其为16位的（在计算时不能超过32位大小）
    //
    // Enable processor interrupts.
    //
    // Configure the Timer0B interrupt for timer timeout.
    //
 //   TimerIntEnable(TIMER0_BASE,TIMER_TIMB_TIMEOUT);
    //
    // Enable the Timer0B interrupt on the processor (NVIC).
    //
    IntEnable(INT_TIMER0B);
    //
    // Enable Timer0B.
    //
    TimerEnable(TIMER0_BASE, TIMER_B);
}
//*****************************************************************************
//
// The interrupt handler for the for Timer0B interrupt.
//
//****************************************************************************
void TIMER0B_Handler(void) 
{
    //
    // Clear the timer interrupt flag.
    //
    TimerIntClear(TIMER0_BASE, TIMER_TIMB_TIMEOUT);
    timer_ISR();
}

 void SysTickInit(void)
 {
		SysTickPeriodSet(SysCtlClockGet()/1000);//1ms
	  SysTickEnable();
		SysTickIntEnable();	
 }
 volatile uint32_t sysTickUptime = 0;
 
 void SysTick_Handler(void)
 {
	sysTickUptime++;
	sys_time();
//	 f= ~f;
//	  if(f)
//			LED1_OFF;
//		else
//			LED1_ON;
 }
 
 void TIM_INIT()
{
	Timer0BInit(50);//0.05毫秒中断一次用于打开发送中断
	SysTickInit();
}

#define TICK_PER_SECOND 1000 
#define TICK_US	(1000000/TICK_PER_SECOND)

uint32_t GetSysTime_us(void) 
{
	register uint32_t ms;
	u32 value;
	ms = sysTickUptime;
//	value = ms * TICK_US + (SysTick->LOAD - SysTick->VAL) * TICK_US / SysTick->LOAD;
	value = ms * TICK_US + (SysTickPeriodGet() - SysTickValueGet()) * TICK_US /SysTickPeriodGet() ;
	
	return value;
}

void Delay_us(uint32_t us)
{
    uint32_t now = GetSysTime_us();
    while (GetSysTime_us() - now < us);
}

//void Delay_ms(uint32_t ms)
//{
//    while (ms--)
//        Delay_us(1000);
//}

int time_1h,time_1m,time_1s,time_1ms;

void sys_time(void)
{ 
	if( !Init_Finish )
	{
		return ;
	}
	
  if(time_1ms < 999)
	{
    time_1ms++;

		Loop_check();
	}
	else
	{
		
    time_1ms =0;
	  if(time_1s<59)
	  {
      time_1s++;
		}
		else
		{
			time_1s = 0;
			if(time_1m<59)
			{
				time_1m++;
			}
			else
			{
				time_1m = 0;
				if(time_1h<23)
				{
					time_1h++;
				}
				else
				{
					time_1h = 0;
				}
			}
		}
	}
}

volatile float Cycle_T[GET_TIME_NUM][3];

enum
{
	NOW = 0,
	OLD,
	NEW,
};

float Get_Cycle_T(u8 item)	
{
	Cycle_T[item][OLD] = Cycle_T[item][NOW];	//上一次的时间
	Cycle_T[item][NOW] = GetSysTime_us()/1000000.0f; //本次的时间
	Cycle_T[item][NEW] = ( ( Cycle_T[item][NOW] - Cycle_T[item][OLD] ) );//间隔的时间（周期）
	return Cycle_T[item][NEW];
}

void Cycle_Time_Init()
{
	u8 i;
	for(i=0;i<GET_TIME_NUM;i++)
	{
		Get_Cycle_T(i);
	}

}

 
