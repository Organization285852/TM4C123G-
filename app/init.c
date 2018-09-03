/******************** (C) COPYRIGHT 2014 ANO Tech ********************************
  * ����   �������ƴ�
 * �ļ���  ��init.c
 * ����    ���ɿس�ʼ��
 * ����    ��www.anotc.com
 * �Ա�    ��anotc.taobao.com
 * ����QȺ ��190169595
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
	
	IntMasterEnable();	//ʹ���ж�
	
	I2C0Init();					//��ʼ��ģ��I2C
	
//	PWM_IN_Init();						//��ʼ�����ջ��ɼ�����
	Delay_ms(400);						//��ʱ
	PWMInit();			//��ʼ������������	
	
//	Usb_Hid_Init();						//�ɿ�usb�ӿڵ�hid��ʼ��
	
  MS5611_Init();						//��ѹ�Ƴ�ʼ��
	TIM_INIT();
	uart0_init(115200);
	uart2_init(9600);//Bluetooth
	uart3_init(500000);//����վ
	uart7_init(115200);//����վ
	
	MPU6050_Init(20);   			//���ٶȼơ������ǳ�ʼ��������20hz��ͨ
//	MPU6050Init();
	LED_Init();								//LED���ܳ�ʼ��
	
	
	
	Para_Init();							//������ʼ��
	
	Delay_ms(100);						//��ʱ
	
	Ultrasonic_Init();   			//��������ʼ��
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
