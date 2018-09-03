
#include "PX4FLOW.h"
#include "hal_I2c.h"
#include "includes.h"

i2c_frame flow;
u8 a;
void ReadPX4Flow(void)
{
//  uint8_t temp;
	uint8_t buf[22];
//	while(i<=255)
//	{
//		temp = I2CReadToBuf(i,0x00,buf,22);
//		
//		if(temp==0)
//		{
//	    printf("     i=%d",i);
//			a=i;
//			break;
//		}
//		i++;
//	  if(i==0x68||i==0x77||i==0x9E||i==0xe8||0xf7)
//			i++;			
//		
//	}
	I2CReadToBuf(FLOWADDR,0x00,buf,22);
	flow.frame_count = buf[1]<<8 | buf[0];
	flow.pixel_flow_x_sum = buf[3]<<8 | buf[2];
	flow.pixel_flow_y_sum = buf[5]<<8 | buf[4];
	flow.flow_comp_m_x = buf[7]<<8 | buf[6];
	flow.flow_comp_m_y = buf[9]<<8 | buf[8];
	flow.qual = buf[11]<<8 | buf[10];
	flow.gyro_x_rate = buf[13]<<8 | buf[12];
	flow.gyro_y_rate = buf[15]<<8 | buf[14];
	flow.gyro_z_rate = buf[17]<<8 | buf[16];
	flow.gyro_range = buf[18];
	flow.sonar_timestamp = buf[19];
	flow.ground_distance = buf[21]<<8 | buf[20];
}

