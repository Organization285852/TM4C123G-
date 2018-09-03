#include "includes.h"


void Delay_ms(u32 ms)
{
	u32 sf;
	sf =SysCtlClockGet()/6000*ms;
	SysCtlDelay(sf);
}

