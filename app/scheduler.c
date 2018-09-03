/******************** (C) COPYRIGHT 2014 ANO Tech ********************************
  * ����   �������ƴ�
 * �ļ���  ��scheduler.c
 * ����    ���������
 * ����    ��www.anotc.com
 * �Ա�    ��anotc.taobao.com
 * ����QȺ ��190169595
**********************************************************************************/
#include "scheduler.h"
#include "includes.h"
#include "hal_timer.h"
#include "mpu6050.h"
#include "ak8975.h"
#include "led.h"
#include "rc.h"
#include "imu.h"
#include "hal_pwm.h"
#include "hal_pwm_in.h"
#include "ctrl.h"
#include "ms5611.h"
#include "parameter.h"
#include "ultrasonic.h"
#include "height_ctrl.h"
#include "RPY.h"
#include "data_transfer.h"
#include "stdio.h"
#include "autofly.h"
#include "tracking.h"
s16 loop_cnt;


loop_t loop;

void Loop_check()  //TIME INTTERRUPT 1msִ��һ��
{
	loop.time++; //u16
	loop.cnt_2ms++;
	loop.cnt_5ms++;
	loop.cnt_10ms++;
	loop.cnt_20ms++;
	loop.cnt_50ms++;

	if( loop.check_flag == 1)
	{
		loop.err_flag ++;     //ÿ�ۼ�һ�Σ�֤��������Ԥ��������û�����ꡣ
	}
	else
	{	
		loop.check_flag = 1;	//�ñ�־λ��ѭ�����������
	}
}
float h;
void Duty_1ms(void)
{
	LED_Display( LED_Brightness );								//20��led������ʾ
	ANO_DT_Data_Exchange();												//����ͨ�Ŷ�ʱ����
}

float test[5];
void Duty_2ms(void)
{
	float inner_loop_time;
	
	inner_loop_time = Get_Cycle_T(0); 						//��ȡ�ڻ�׼ȷ��ִ������
	
	test[0] = GetSysTime_us()/1000000.0f;
	TrackingDuty();
	MPU6050_Data_Prepare( inner_loop_time );			//mpu6�ᴫ�������ݴ���

	CTRL_1( inner_loop_time ); 										//�ڻ����ٶȿ��ơ����룺ִ�����ڣ��������ٶȣ��������ٶȣ��Ƕ�ǰ������������PWMռ�ձȡ�<����δ��װ>
	
	RC_Duty( inner_loop_time ,Rc_Pwm_In );				// ң����ͨ�����ݴ��� �����룺ִ�����ڣ����ջ�pwm��������ݡ�
	
	test[1] = GetSysTime_us()/1000000.0f;
}

void Duty_5ms(void)
{
	float outer_loop_time;
	
	outer_loop_time = Get_Cycle_T(2);								//��ȡ�⻷׼ȷ��ִ������
	
	
	
	/*IMU������̬�����룺���ִ�����ڣ��������������ݣ�ת������ÿ�룩��������ٶȼ����ݣ�4096--1G���������ROLPITYAW��̬��*/
 IMUupdate(0.5f *outer_loop_time,mpu6050.Gyro_deg.x, mpu6050.Gyro_deg.y, mpu6050.Gyro_deg.z, mpu6050.Acc.x, mpu6050.Acc.y, mpu6050.Acc.z,&Roll,&Pitch,&Yaw);
 //MPU6050Rfresh(&Pitch,&Roll,&Yaw);
 	CTRL_2( outer_loop_time ); 											// �⻷�Ƕȿ��ơ����룺ִ�����ڣ������Ƕȣ�ҡ����������̬�Ƕȣ�������������ٶȡ�<����δ��װ>
}


void Duty_10ms(void)
{
 		if( MS5611_Update() ) 				//����ms5611��ѹ������
		{	
			baro_ctrl_start = 1;  //20ms
		}	
		Get_Cycle_T(3);
	test[2] = GetSysTime_us()/1000000.0f;
	  ANO_AK8975_Read();			//��ȡ������������	
	test[3] = GetSysTime_us()/1000000.0f;	
	h = test[3] - test[2];
}

void Duty_20ms(void)
{
	
}

void Duty_50ms(void)
{
	LED_Duty();								//LED����
	Ultra_Duty();
	Get_Cycle_T(4);
	AutoFlyDuty();
}


void Duty_Loop(void)   					//�����������Ϊ1ms���ܵĴ���ִ��ʱ����ҪС��1ms��
{

	if( loop.check_flag == 1 )
	{
		loop_cnt = time_1ms;
		
		Duty_1ms();							//����1ms������
		
		if( loop.cnt_2ms >= 2 )
		{
			loop.cnt_2ms = 0;
			Duty_2ms();						//����2ms������
		}
		if( loop.cnt_5ms >= 5 )
		{
			loop.cnt_5ms = 0;
			Duty_5ms();						//����5ms������
		}
		if( loop.cnt_10ms >= 10 )
		{
			loop.cnt_10ms = 0;
			Duty_10ms();					//����10ms������
		}
		if( loop.cnt_20ms >= 20 )
		{
			loop.cnt_20ms = 0;
			Duty_20ms();					//����20ms������
		}
		if( loop.cnt_50ms >= 50 )
		{
			loop.cnt_50ms = 0;
			Duty_50ms();					//����50ms������
		}	
		loop.check_flag = 0;		//ѭ��������ϱ�־
	}
}




	/******************* (C) COPYRIGHT 2014 ANO TECH *****END OF FILE************/
	

