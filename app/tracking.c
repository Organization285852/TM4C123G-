#include "tracking.h"
#include "hal_pwm_in.h"
#include "ultrasonic.h"
#include "rc.h"
#include "hal_timer.h"
#define _roll	Rc_Pwm_In[0] 
TrackCtrlDef TrackCtrl;
u8 CameraOk = 0;
void TrackCtrlDef_Init(void)
{
	TrackCtrl.TrackPID.Kp = 0.5f;	//越大就越快靠近平衡，但越是趋近平衡，趋近速度就越慢，相隔即为静差
	TrackCtrl.TrackPID.Ki = 0.002f;	//消除静差，但也产生了超调，减小Ki可抑制超调，但消除静差的时间会被拉长 
	TrackCtrl.TrackPID.Kd = 1.1f;	//抑制超调，让系统更稳定，过大会加长调节时间
	TrackCtrl.err = 0;
	TrackCtrl.err_i = 0;
	TrackCtrl.err_d = 0;
	TrackCtrl.ExpXCoorAve = 128;
	TrackCtrl.CurXCoorAve = 128;
	TrackCtrl.PreXCoorAve = 128;
}
void TrackingPIDCtrl(float T,u8 Receive)
{
	if(Receive != 0)	//如果是0,即飞出黑线,无需再调节,如果是511圆盘标志码,即不调节.
	{
		TrackCtrl.CurXCoorAve = Receive;
		
		TrackCtrl.err = TrackCtrl.ExpXCoorAve - TrackCtrl.CurXCoorAve;
		if((TrackCtrl.err < 105) && (TrackCtrl.err > -105))
		{
			TrackCtrl.err_i += TrackCtrl.err*T;
		}
		else
		{
			TrackCtrl.err_i = 0;
		}
		if(TrackCtrl.err_i >  10000) TrackCtrl.err_i =  10000;
		if(TrackCtrl.err_i < -10000) TrackCtrl.err_i = -10000;	
		
		TrackCtrl.err_d = (TrackCtrl.PreXCoorAve - TrackCtrl.CurXCoorAve)*(0.035/T);
		TrackCtrl.PreXCoorAve = TrackCtrl.CurXCoorAve;
		
//		if(TrackCtrl.err_d != 0)
//		{		
		TrackCtrl.TrackPID.Out =   TrackCtrl.TrackPID.Kp * TrackCtrl.err
								 + TrackCtrl.TrackPID.Ki * TrackCtrl.err_i
								 + TrackCtrl.TrackPID.Kd * TrackCtrl.err_d;

		_roll = (u16)((1500 - TrackCtrl.TrackPID.Out) + 0.5f);
//		CH_filter[0] = -TrackCtrl.TrackPID.Out;
//		}
//		else _roll = a;
	}
	//else _roll = 1500;
//	CH_filter[0] = 0;
}

u8 CircleFlag = 0;
u8 Rx1Buffer = 128;
u8 Rx1BufferOld = 128;
u8 CameraCnt;
void TrackingDuty(void)
{
	CameraCnt++;
	if(CameraCnt>200)
	{
		CameraOk = 0;
	}
}

void ResiveCameraData(u8 byte)//放入串口接收中断
{
	float t;
//		if(cnt > 90000)
//	{
//		CircleFlag = 1;
//	}
	CameraOk = 1;
	CameraCnt = 0;//喂狗
//	if(byte == 0x123 )
//	{
//		CircleFlag = 1;
//	}
//	else 
//	{
		t = Get_Cycle_T(1);
		Rx1Buffer += 4*3.14f*t*(byte -Rx1Buffer);//低通滤波
//	}
	if(ultra_distance > 400)//高度大于四百开始循迹
	{
	//	TrackingPIDCtrl(t,Rx1Buffer);		
	}
}
