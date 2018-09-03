/******************** (C) COPYRIGHT 2014 Air Nano Team ***************************
 * �ļ���  ��mpu6050.c
 * ����    ��mpu6050����         
 * ʵ��ƽ̨��Air Nano���������
 * ��汾  ��ST3.5.0
 * ����    ��Air Nano Team 
 * �Ա�    ��http://byd2.taobao.com
**********************************************************************************/


#include "ak8975.h"
#include "hmc5883.h"
#include "mpu6050.h"
#include "delay.h"
//��ǰ�ų������ֵ����Сֵ
int16_t  HMC58X3_limit[6]={0};
int16_t  *mag_limt = HMC58X3_limit;

/*====================================================================================================*/
/*====================================================================================================*
**���� : Init_HMC5883L
**���� : ָ�����ʼ��
**���� : None
**ݔ�� : ״̬
**��ע : None
**====================================================================================================*/
/*====================================================================================================*/

u8 Init_HMC5883L(void)
{ 
	u8 b;
	u32 ack;
	ack = I2CReadToBuf(MAG_ADDRESS,0x0b,&b,1);
	if(ack)
		return 1;
// leave test mode
//	Single_Write(MAG_ADDRESS, HMC58X3_R_CONFA, 0x70);   // Configuration Register A  -- 0 11 100 00  num samples: 8 ; output rate: 15Hz ; normal measurement mode
//	Single_Write(MAG_ADDRESS, HMC58X3_R_CONFB, 0x20);   // Configuration Register B  -- 001 00000    configuration gain 1.3Ga
//	Single_Write(MAG_ADDRESS, HMC58X3_R_MODE, 0x00);    // Mode register             -- 000000 00    continuous Conversion Mode
  b=0x70;
	I2CWriteToReg(MAG_ADDRESS, HMC58X3_R_CONFA, &b,1);
	b=0x20;
	I2CWriteToReg(MAG_ADDRESS, HMC58X3_R_CONFB, &b,1);
	b=0x00;
  I2CWriteToReg(MAG_ADDRESS, HMC58X3_R_MODE, &b,1);
	Delay_ms(100);
//	delay(100);
	return 0;	 
}
	
/*====================================================================================================*/
/*====================================================================================================*
**���� : hmc5883lRead
**���� : ��ȡ�ش�����
**���� : None
**ݔ�� : None
**��ע : None
**====================================================================================================*/
/*====================================================================================================*/
void HMC5883lRead(int16_t *magData)
{
	u8 buf[6];
	int16_t mag[3];
	static int32_t An[3] = {0,0,0};
	
	// ��ȡ�Ĵ�������
	I2CReadToBuf(MAG_ADDRESS,MAG_DATA_REGISTER,buf,6);

	// ʮλ����˲�
	An[0] -= An[0]/10;
	An[0] += (int16_t)(buf[0] << 8 | buf[1]);
	mag[0] = An[0]/10;
	
	An[1] -= An[1]/10;
	An[1] += (int16_t)(buf[4] << 8 | buf[5]);
	mag[1] = An[1]/10;
	
	An[2] -= An[2]/10;
	An[2] += (int16_t)(buf[2] << 8 | buf[3]);
	mag[2] = An[2]/10;

	//��ҪУ׼
	if(Mag_CALIBRATED) {
		Mag_Calibration(mag);
	}
	// ����
//	for(cy=0;cy<3;cy++)
//		*(magData+cy) = (float)(mag[cy] -(*(mag_limt+cy+3) + *(mag_limt+cy))/2);
	
	ak8975.Mag_Val.x = (float)(mag[0] -(*(mag_limt+0+3) + *(mag_limt+0))/2);
	ak8975.Mag_Val.z = (float)(mag[1] -(*(mag_limt+1+3) + *(mag_limt+1))/2);
	ak8975.Mag_Val.y = (float)(mag[2] -(*(mag_limt+2+3) + *(mag_limt+2))/2);
}

/*====================================================================================================*/
/*====================================================================================================*
**���� : Mag_Calibration
**���� : �ش�У׼
**���� : None
**ݔ�� : None
**��ע : None
**====================================================================================================*/
/*====================================================================================================*/
void Mag_Calibration(int16_t *array)
{
	static u16 count;
	int cy;
  
	// ��ʼ�ɼ� Ѱ���������������Сֵ
	for(cy=0;cy<3;cy++){
		if(*(mag_limt+cy)> *(array+cy)) *(mag_limt+cy) = *(array+cy);  //����С
		else if(*(mag_limt+cy+3)<*(array+cy)) *(mag_limt+cy+3) = *(array+cy);  //�����
	}
	if(count>=CALIBRATING_MAG_CYCLES)
	{
		Mag_CALIBRATED = 0;
		count = 0;
	}
	count++;
}
