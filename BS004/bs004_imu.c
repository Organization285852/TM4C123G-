/*************************************************************************************************************
圆点博士小四轴飞行器2014版配套源代码声明:
该源代码仅供参考,圆点博士不对源代码提供任何形式的担保,也不对因使用该源代码而出现的损失负责.
用户可以以学习的目的修改和使用该源代码.
但用户在修改该源代码时,不得移除该部分版权信息，必须保留原版声明.

更多信息，请访问官方网站www.etootle.com, 官方博客:http://weibo.com/xiaosizhou
**************************************************************************************************************/
#include "bs004_imu.h" 
#include "mpu6050.h"
//
float bs004_mpu6050_gyro_scale=0,bs004_mpu6050_acc_scale=0,bs004_gyro_to_acc_scale=0,bs004_hmc5883l_mag_scale=0;
extern float bs004_mpu6050_gyro_angel_pitch_ave,bs004_mpu6050_gyro_angel_roll_ave,bs004_mpu6050_gyro_angel_yaw_ave;
extern float bs004_mpu6050_acc_angel_pitch_ave,bs004_mpu6050_acc_angel_roll_ave,bs004_mpu6050_acc_angel_yaw_ave;
//
float q0=1,q1=0,q2=0,q3=0;	
float exInt=0,eyInt=0,ezInt=0;	
float bs004_imu_pitch=0,bs004_imu_roll=0,bs004_imu_yaw=0;
float bs004_quad_Kp=0,bs004_quad_Ki=0,bs004_quad_halfT=0;			
//
void BS004_Quad_Calculation(void)
{
	float ax=0,ay=0,az=0,gx=0,gy=0,gz=0;
	//
	bs004_gyro_to_acc_scale=bs004_mpu6050_acc_scale*bs004_mpu6050_gyro_scale;
	//	
	gx=bs004_mpu6050_gyro_angel_pitch_ave*bs004_gyro_to_acc_scale;
	gy=bs004_mpu6050_gyro_angel_roll_ave*bs004_gyro_to_acc_scale;
	gz=bs004_mpu6050_gyro_angel_yaw_ave*bs004_gyro_to_acc_scale;		
	//
	ax=bs004_mpu6050_acc_angel_roll_ave;
	ay=bs004_mpu6050_acc_angel_pitch_ave;	
	az=bs004_mpu6050_acc_angel_yaw_ave;		
	//
	BS004_IMU_Update(ax,ay,az,gx,gy,gz);
}
//
unsigned char BS004_IMU_Update(float ax,float ay,float az,float gx,float gy,float gz) 
{
	float norm;
	float vx, vy, vz;
	float ex, ey, ez;  
  //	
	//圆点博士:四元数乘法运算
	float q0q0 = q0 * q0;							
	float q0q1 = q0 * q1;
	float q0q2 = q0 * q2;
	float q1q1 = q1 * q1;
	float q1q3 = q1 * q3;
	float q2q2 = q2 * q2;
	float q2q3 = q2 * q3;
	float q3q3 = q3 * q3;
	//	
	//圆点博士:归一化处理
	norm = sqrt(ax*ax + ay*ay + az*az);     
	if(norm==0) return 0;	
	ax = ax / norm;
	ay = ay / norm;
	az = az / norm;   
  //	
	//圆点博士:建立小四轴坐标系	
	vx = 2*(q1q3 - q0q2);								
	vy = 2*(q0q1 + q2q3);
	vz = q0q0 - q1q1 - q2q2 + q3q3;
	//
	//圆点博士:坐标系和重力叉积运算
	ex = (ay*vz - az*vy);								
	ey = (az*vx - ax*vz);
	ez = (ax*vy - ay*vx);
	//
	//圆点博士:比例运算
	exInt = exInt + ex*bs004_quad_Ki;
	eyInt = eyInt + ey*bs004_quad_Ki;
	ezInt = ezInt + ez*bs004_quad_Ki;
	//
	//圆点博士:陀螺仪融合
	gx = gx + bs004_quad_Kp*ex + exInt;
	gy = gy + bs004_quad_Kp*ey + eyInt;
	gz = gz + bs004_quad_Kp*ez + ezInt;
	//
	//圆点博士:整合四元数率
	q0 = q0 + (-q1*gx - q2*gy - q3*gz)*bs004_quad_halfT;
	q1 = q1 + (q0*gx + q2*gz - q3*gy)*bs004_quad_halfT;
	q2 = q2 + (q0*gy - q1*gz + q3*gx)*bs004_quad_halfT;
	q3 = q3 + (q0*gz + q1*gy - q2*gx)*bs004_quad_halfT;  
	//
	//圆点博士:归一化处理
	norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
	if(norm==0) return 0;	
	q0 = q0 / norm;
	q1 = q1 / norm;
	q2 = q2 / norm;
	q3 = q3 / norm;
	//
	//圆点博士:欧拉角转换
	bs004_imu_roll=asin(-2*q1q3 + 2*q0q2)*57.30f;
  bs004_imu_pitch=atan2(2*q2q3 + 2*q0q1, -2*q1q1-2*q2q2 + 1)*57.30f; 
  bs004_imu_yaw=bs004_imu_yaw-gz*bs004_mpu6050_gyro_scale;
	//
	return 1;	
}


void ANBT_SEND_DMP_EULER_DATA(void)
{
	float bs004_display_pitch=0,bs004_display_roll=0,bs004_display_yaw=0;
	unsigned char data_type,checksum=0,euler_data_sign=0,i=0;
	unsigned int bs004_mpu6050_euler_data[3];
	unsigned char bs004_mpu6050_euler_data_buffer[6];
	//
	bs004_display_pitch=bs004_imu_pitch*100;
	bs004_display_roll =bs004_imu_roll*100;
	bs004_display_yaw  =bs004_imu_yaw*100;
	//	
	if(bs004_display_pitch<0) 
	{
		euler_data_sign|=4;
		bs004_display_pitch+=18000;
	}
	if(bs004_display_roll<0) 
	{
		euler_data_sign|=2;	
		bs004_display_roll+=18000;
	}
	if(bs004_display_yaw<0) 
	{
		euler_data_sign|=1;		
		bs004_display_yaw+=18000;
	}
	//
	bs004_mpu6050_euler_data[0]=(unsigned int)bs004_display_pitch;
	bs004_mpu6050_euler_data[1]=(unsigned int)bs004_display_roll;	
	bs004_mpu6050_euler_data[2]=(unsigned int)bs004_display_yaw;	
	//
  for(i=0;i<3;i++) 
	{
		bs004_mpu6050_euler_data_buffer[i*2]=(bs004_mpu6050_euler_data[i]>>8)&0xff;
		bs004_mpu6050_euler_data_buffer[i*2+1]=bs004_mpu6050_euler_data[i]&0xff;
	}
	//
	data_type=0xB0| euler_data_sign;
	checksum=data_type;
	for(i=0;i<6;i++) checksum+=bs004_mpu6050_euler_data_buffer[i];
	checksum&=0xff;
	checksum=~checksum;
	checksum++;
	//	
//	BS004_COM1_Send_Char(':');
//	BS004_COM1_Send_Num(data_type);
//	for(i=0;i<6;i++) BS004_COM1_Send_Num(bs004_mpu6050_euler_data_buffer[i]);			
//	BS004_COM1_Send_Num(checksum);
//	BS004_COM1_Send_Char('/');
//	BS004_COM1_Send_Char('\n');
}
