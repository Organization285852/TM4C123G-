

#include "includes.h"
#include "PX4FLOW.h"
#include "mpu6050.h"
#include "led.h"
#include "hmc5883.h"
#include "ak8975.h"
#include "imu.h"
#include "hal_timer.h"
#include "ms5611.h"
#include "scheduler.h"
#include "ultrasonic.h"
#include "init.h"
#include "hal_pwm_in.h"
#include "rc.h"
#include "distance.h"
#include "delay.h"
u8 Init_Finish = 0;
int main(void)
{
//	CH_filter[0] = 0;  //R
//	CH_filter[1] = 0; //p
	CH_filter[2] = -500;  //thr =100时，已飞升
//  CH_filter[3] = 0; //Yaw
	
	Rc_Pwm_In[0] = 1500;////R
	Rc_Pwm_In[1] = 1500;//p
	Rc_Pwm_In[2] = 1000;//thr 1000-2000
	Rc_Pwm_In[3] = 1500; //Yaw
	Init_Finish = All_Init();		

	 height_ctrl_mode = 2;
////	//陀螺仪校正
//	mpu6050.Gyro_CALIBRATE=1;
//	while(mpu6050.Gyro_CALIBRATE)
//	{
//	 MPU6050_Data_Prepare(0.01);
//		Delay_ms(10);
//	}
////	//磁力计校正
//	Mag_CALIBRATED=1;
//	while(Mag_CALIBRATED)
//	{
//	 ANO_AK8975_Read();
//	}
	while(1)
	{
		Duty_Loop(); 
	}
}

