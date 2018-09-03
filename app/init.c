/******************** (C) COPYRIGHT 2014 ANO Tech ********************************
  * 作者   ：匿名科创
 * 文件名  ：init.c
 * 描述    ：飞控初始化
 * 官网    ：www.anotc.com
 * 淘宝    ：anotc.taobao.com
 * 技术Q群 ：190169595
**********************************************************************************/

#include "includes.h"
#include "hal_pwm.h"
#include "mpu6050.h"
#include "hal_i2c.h"
#include "led.h"
#include "ctrl.h"
#include "ms5611.h"
#include "ak8975.h"
#include "ultrasonic.h"
#include "delay.h"
#include "hal_timer.h"
#include "init.h"
#include "RPY.h"
#include "usart.h"
#include "rc.h"
#include "hal_pwm_in.h"
#include "tracking.h"
u8 All_Init()
{
	SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	
	IntMasterEnable();	//使能中断
	
	I2C0Init();					//初始化模拟I2C
	
//	PWM_IN_Init();						//初始化接收机采集功能
	Delay_ms(400);						//延时
	PWMInit();			//初始化电调输出功能	
	
//	Usb_Hid_Init();						//飞控usb接口的hid初始化
	
  MS5611_Init();						//气压计初始化
	TIM_INIT();
	uart0_init(115200);
	uart2_init(9600);//Bluetooth
	uart3_init(500000);//地面站
	uart7_init(115200);//地面站
	
	MPU6050_Init(20);   			//加速度计、陀螺仪初始化，配置20hz低通
//	MPU6050Init();
	LED_Init();								//LED功能初始化
	
	
	
	Para_Init();							//参数初始化
	
	Delay_ms(100);						//延时
	
	Ultrasonic_Init();   			//超声波初始化
	height_ctrl_mode = 2;
	Cycle_Time_Init();
	TrackCtrlDef_Init();
	
	ak8975_ok = !(ANO_AK8975_Run());
	
	if( !mpu6050_ok )
	{
		LED_MPU_Err();
	}
	else if( !ak8975_ok )
	{
		LED_Mag_Err();
	}
//	else if( !ms5611_ok )
//	{
//		LED_MS5611_Err();
//	}
 	return (1);
}
/******************* (C) COPYRIGHT 2014 ANO TECH *****END OF FILE************/
