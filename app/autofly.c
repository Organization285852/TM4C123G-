#include "height_ctrl.h"
#include "ultrasonic.h"
#include "includes.h"
#include "autofly.h"
#include "rc.h"
#include "hal_pwm_in.h"
#include "tracking.h"
#define MAX_thr 1550
#define MIN_thr 1200
#define MIN_heigth 80
struct _auto fly_state;
void ChangeZ(u16 speed,u16 height,float T)
{
	 static float DH;
	 DH = height - ultra_distance;
	  if(DH>20)//有一厘米的误差。
			MAX_HEIGHT = ultra_distance + (ultra_speed*0.5 + speed *0.5)*T;//想要的速度和实际速度各占一半。
		else
			exp_height = height;
		if(DH<-50)
			MAX_HEIGHT = ultra_distance - (ultra_speed*0.5 + speed *0.5)*T;  
		else
			MAX_HEIGHT = height;
}
u8 auto_time;
void AutoFlyDuty(void)  //放入50ms任务，100ms执行一次
{
	auto_time++;
	auto_time = auto_time%2;
	if(auto_time==0)
	{
		if(fly_state.down)
			Down();
		if(fly_state.rise)
			Rise(800);
	}
//	if(CameraOk==0)
//	{
//		fly_state.down = 1;
//	}
}
void Down(void)
{
	fly_state.rise = 0;
	MAX_HEIGHT = MIN_heigth + 20;
	Rc_Pwm_In[2] -= 4;
	if(Rc_Pwm_In[2]<MIN_thr)
		Rc_Pwm_In[2] = MIN_thr;
	if(ultra_distance<MIN_heigth + 10)
	{
		Rc_Pwm_In[2] = MIN_thr;
		fly_state.down = 0;
	}
}
void Rise(u16 heigth) //100ms
{
	MAX_HEIGHT = heigth;
	Rc_Pwm_In[2] += 20;
	if(Rc_Pwm_In[2]>MAX_thr)
		Rc_Pwm_In[2] = MAX_thr;
	if((ABS(ultra_distance-MAX_HEIGHT)<50)&&Rc_Pwm_In[2] == MAX_thr)
	{
		fly_state.rise =0;
	}
}

