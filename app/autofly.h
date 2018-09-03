#ifndef __AUTOFLY_H
#define __AUTOFLY_H
#include "includes.h"


struct _auto{
	u8 down;
	u8 rise;
	u16 heigth;
};

void AutoFlyDuty(void);
void Rise(u16 heigth);
void Down(void);





void ChangeZ(u16 speed,u16 height,float T);

extern struct _auto fly_state;

#endif

