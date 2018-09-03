/******************** (C) COPYRIGHT 2014 Air Nano Team ***************************
 * 文件名  ：mpu6050.c
 * 描述    ：mpu6050配置         
 * 实验平台：Air Nano四轴飞行器
 * 库版本  ：ST3.5.0
 * 作者    ：Air Nano Team 
 * 淘宝    ：http://byd2.taobao.com
**********************************************************************************/


#include "ak8975.h"
#include "hmc5883.h"
#include "mpu6050.h"
#include "delay.h"
//当前磁场的最大值和最小值
int16_t  HMC58X3_limit[6]={0};
int16_t  *mag_limt = HMC58X3_limit;

/*====================================================================================================*/
/*====================================================================================================*
**函数 : Init_HMC5883L
**功能 : 指南针初始化
**输入 : None
**輸出 : 状态
**备注 : None
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
**函数 : hmc5883lRead
**功能 : 度取地磁数据
**输入 : None
**輸出 : None
**备注 : None
**====================================================================================================*/
/*====================================================================================================*/
void HMC5883lRead(int16_t *magData)
{
	u8 buf[6];
	int16_t mag[3];
	static int32_t An[3] = {0,0,0};
	
	// 读取寄存器数据
	I2CReadToBuf(MAG_ADDRESS,MAG_DATA_REGISTER,buf,6);

	// 十位深度滤波
	An[0] -= An[0]/10;
	An[0] += (int16_t)(buf[0] << 8 | buf[1]);
	mag[0] = An[0]/10;
	
	An[1] -= An[1]/10;
	An[1] += (int16_t)(buf[4] << 8 | buf[5]);
	mag[1] = An[1]/10;
	
	An[2] -= An[2]/10;
	An[2] += (int16_t)(buf[2] << 8 | buf[3]);
	mag[2] = An[2]/10;

	//需要校准
	if(Mag_CALIBRATED) {
		Mag_Calibration(mag);
	}
	// 修正
//	for(cy=0;cy<3;cy++)
//		*(magData+cy) = (float)(mag[cy] -(*(mag_limt+cy+3) + *(mag_limt+cy))/2);
	
	ak8975.Mag_Val.x = (float)(mag[0] -(*(mag_limt+0+3) + *(mag_limt+0))/2);
	ak8975.Mag_Val.z = (float)(mag[1] -(*(mag_limt+1+3) + *(mag_limt+1))/2);
	ak8975.Mag_Val.y = (float)(mag[2] -(*(mag_limt+2+3) + *(mag_limt+2))/2);
}

/*====================================================================================================*/
/*====================================================================================================*
**函数 : Mag_Calibration
**功能 : 地磁校准
**输入 : None
**輸出 : None
**备注 : None
**====================================================================================================*/
/*====================================================================================================*/
void Mag_Calibration(int16_t *array)
{
	static u16 count;
	int cy;
  
	// 开始采集 寻找三个轴的最大和最小值
	for(cy=0;cy<3;cy++){
		if(*(mag_limt+cy)> *(array+cy)) *(mag_limt+cy) = *(array+cy);  //找最小
		else if(*(mag_limt+cy+3)<*(array+cy)) *(mag_limt+cy+3) = *(array+cy);  //找最大
	}
	if(count>=CALIBRATING_MAG_CYCLES)
	{
		Mag_CALIBRATED = 0;
		count = 0;
	}
	count++;
}
