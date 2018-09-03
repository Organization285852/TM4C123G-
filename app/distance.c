#include "distance.h"
#include "PX4FLOW.h"
_distance distance;
void GetDistance1(float ms)
{
	ReadPX4Flow();
	distance.LX += (float)(ms*flow.pixel_flow_x_sum); 
	distance.LY += (float)(ms*flow.pixel_flow_y_sum);
}

void GetDistance(void)
{
	static uint16_t lastcount;
	uint16_t ds;
	ReadPX4Flow();
	ds = flow.frame_count -lastcount;
	distance.LX += ds*flow.pixel_flow_x_sum;
	distance.LY += ds*flow.pixel_flow_y_sum;
	lastcount = flow.frame_count;
}

