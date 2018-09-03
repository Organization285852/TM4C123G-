#ifndef __TRACKING_H
#define __TRACKING_H

#include "includes.h"

/******************
*  Ñ­¼£PID½á¹¹Ìå  *
*******************/
typedef struct
{
	float Kp;
	float Ki;
	float Kd;
	float Out;

}TrackPIDDef;


typedef struct
{
	TrackPIDDef TrackPID;
	float err;
	float err_i;
	float err_d;
	u16 ExpXCoorAve;
	u16 CurXCoorAve;
	u16 PreXCoorAve;

}TrackCtrlDef;

void TrackingPIDCtrl(float T,u8 Receive);
void TrackingDuty(void);
void ResiveCameraData(u8 byte);
void TrackCtrlDef_Init(void);
extern u8 CameraOk;
extern u8 Rx1Buffer;
extern TrackCtrlDef TrackCtrl;

#endif


