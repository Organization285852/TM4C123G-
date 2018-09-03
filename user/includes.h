#ifndef __INCLUDES_H
#define __INCLUDES_H

//������ͷ�ļ�
#include "stdio.h"
#include "stdint.h"

//�⺯��ͷ�ļ�
#include "tm4c123gh6pm.h"
#include "hw_memmap.h"
#include "hw_types.h"
#include "gpio.h"
#include "sysctl.h"
#include "interrupt.h"
#include "uart.h"
#include "pin_map.h"
#include "i2c.h"
#include "hw_i2c.h"
#include "pwm.h"
#include "timer.h"
#include "systick.h"

//Ӧ��ͷ�ļ�
#include "usart.h"
#include "hal_i2c.h"
//#include "hal_pwm.h"
//#include "led.h"
//#include "mpu6050.h"
#include "mymath.h"
//#include "parameter.h"



#define u8			uint8_t
#define u16     uint16_t
#define u32     uint32_t

#define s32     int32_t
#define s16     int16_t
#define s8      int8_t 

//typedef   signed          char s8;
//typedef   signed short     int s16;
//typedef   signed           int s32;

//typedef unsigned          char u8;
//typedef unsigned short     int u16;
//typedef unsigned           int u32;

#define  vs16       volatile int16_t  
#define  vs32       volatile int32_t  


//================ϵͳ===================
#define CTRL_HEIGHT 1       //0ʧ�ܣ�1ʹ��

#define USE_US100           //ʹ��us100�ͺų����� 
//#define USE_KS103					//ʹ��ks103�ͺų�����

#define MAXMOTORS 		(4)		//�������
#define GET_TIME_NUM 	(5)		//���û�ȡʱ�����������
#define CH_NUM 				(8) 	//���ջ�ͨ������

#define USE_TOE_IN_UNLOCK 0 // 0��Ĭ�Ͻ�����ʽ��1����˽�����ʽ
#define ANO_DT_USE_USART2 	//��������2��������

#define FLOWADDR    0x16   //66

//#define ANO_DT_USE_USB_HID	//�����ɿ�USBHID������λ������
//=======================================
/***************�ж����ȼ�******************/
//#define NVIC_GROUP NVIC_PriorityGroup_3		//�жϷ���ѡ��
//#define NVIC_PWMIN_P			1		//���ջ��ɼ�
//#define NVIC_PWMIN_S			1
//#define NVIC_TIME_P       2		//��δʹ��
//#define NVIC_TIME_S       0
//#define NVIC_UART_P				5		//��δʹ��
//#define NVIC_UART_S				1
//#define NVIC_UART2_P			3		//����2�ж�
//#define NVIC_UART2_S			1
/***********************************************/

//================������===================
//#define ACC_ADJ_EN 									//�Ƿ�����У׼���ٶȼ�,(����������)

#define OFFSET_AV_NUM 	50					//У׼ƫ����ʱ��ƽ��������
#define FILTER_NUM 			10					//����ƽ���˲���ֵ����

#define TO_ANGLE 				0.06103f 		//0.061036 //   4000/65536  +-2000   ???

#define FIX_GYRO_Y 			1.02f				//������Y����в���
#define FIX_GYRO_X 			1.02f				//������X����в���

#define TO_M_S2 				0.23926f   	//   980cm/s2    +-8g   980/4096
#define ANGLE_TO_RADIAN 0.01745329f //*0.01745 = /57.3	�Ƕ�ת����

#define MAX_ACC  4096.0f						//+-8G		���ٶȼ�����
#define TO_DEG_S 500.0f      				//T = 2ms  Ĭ��Ϊ2ms ����ֵ����1/T

enum
{
 A_X = 0,
 A_Y ,
 A_Z ,
 G_Y ,
 G_X ,
 G_Z ,
 TEM ,
 ITEMS ,
};

// CH_filter[],0�����1������2���ţ�3����		
enum
{
 ROL= 0,
 PIT ,
 THR ,
 YAW ,
 AUX1 ,
 AUX2 ,
 AUX3 ,
 AUX4 ,
};
//=========================================

//================����=====================
#define MAX_CTRL_ANGLE			25.0f										//ң���ܴﵽ�����Ƕ�
#define ANGLE_TO_MAX_AS 		30.0f										//�Ƕ����Nʱ���������ٶȴﵽ��󣨿���ͨ������CTRL_2��Pֵ������
#define CTRL_2_INT_LIMIT 		0.5f *MAX_CTRL_ANGLE		//�⻷���ַ���

#define MAX_CTRL_ASPEED 	 	300.0f									//ROL,PIT����������ƽ��ٶ�
#define MAX_CTRL_YAW_SPEED 	150.0f									//YAW����������ƽ��ٶ�
#define CTRL_1_INT_LIMIT 		0.5f *MAX_CTRL_ASPEED		//�ڻ����ַ���


#define MAX_PWM				100			///%	���PWM���Ϊ100%����
#define MAX_THR       80 			///%	����ͨ�����ռ��80%����20%��������
#define READY_SPEED   20			///%	��������ת��20%����
//=========================================



#endif

