#ifndef _AK8975_H_
#define	_AK8975_H_


#include "stdbool.h"
#include "includes.h"
#include "parameter.h"
#define CALIBRATING_MAG_CYCLES              2000  //У׼ʱ�����20s

#define AK8975_ADDRESS         0x0c	// 0x18

#define AK8975_WIA     0x00
#define AK8975_HXL     0x03
#define AK8975_HXH     0x04
#define AK8975_HYL     0x05
#define AK8975_HYH     0x06
#define AK8975_HZL     0x07
#define AK8975_HZH     0x08
#define AK8975_CNTL    0x0A

typedef struct 
{
	xyz_s16_t Mag_Adc;			//����ֵ
	xyz_f_t   Mag_Offset;		//ƫ��ֵ
	xyz_f_t 	Mag_Gain;			//��������	
  xyz_f_t 	Mag_Val;			//�������ֵ
}ak8975_t;

extern ak8975_t ak8975;

bool ANO_AK8975_Run(void);
void ANO_AK8975_CalOffset_Mag(void);
void ANO_AK8975_Read(void);


extern u8 Mag_CALIBRATED;
extern u8 ak8975_ok;
#endif

