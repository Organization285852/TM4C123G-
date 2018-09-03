/*************************************************************************************************************
圆点博士小四轴飞行器2014版配套源代码声明:
该源代码仅供参考,圆点博士不对源代码提供任何形式的担保,也不对因使用该源代码而出现的损失负责.
用户可以以学习的目的修改和使用该源代码.
但用户在修改该源代码时,不得移除该部分版权信息，必须保留原版声明.

更多信息，请访问官方网站www.etootle.com, 官方博客:http://weibo.com/xiaosizhou
**************************************************************************************************************/
#include "bs004_parameter.h" 
//
unsigned int bs004_load_par_sign=0;
//
void BS004_Load_Fly_Parameter(void)
{
	bs004_load_par_sign=0; 	//圆点博士:装载参数标志        
	//
	BS004_Load_Filter_Parameter();			//圆点博士:装载参数
	BS004_Load_Pid_Parameter();         //圆点博士:装载参数
	BS004_Load_Calibration_Parameter(); //圆点博士:装载参数
	//
	BS004_COM1_Send_Str_Head();
	BS004_COM1_Send_Str_Body("load user parameter .");
	BS004_COM1_Send_4bits_BCD_Num(bs004_load_par_sign);
	BS004_COM1_Send_Str_Tail();
}

void BS004_Load_Filter_Parameter(void)
{
	int bs004_filter_par[12];
	//
	bs004_filter_par[0]=750;
	bs004_filter_par[1]=250;
	bs004_filter_par[2]=0;
	bs004_filter_par[3]=0;
	bs004_filter_par[4]=8192;
	bs004_filter_par[5]=8725;
	bs004_filter_par[6]=1000;
	bs004_filter_par[7]=1000;
	bs004_filter_par[8]=36;
	bs004_filter_par[9]=1;
	bs004_filter_par[10]=1600;
	bs004_filter_par[11]=1;
	//
	bs004_filter_high=(float)bs004_filter_par[0]/1000.0f;						//圆点博士:互补滤波参数
	bs004_filter_low=(float)bs004_filter_par[1]/1000.0f;						//圆点博士:互补滤波参数	
	bs004_filter_time=(float)bs004_filter_par[2]/1000.0f;						//圆点博士:互补滤波参数		
	//bs004_sys_timer_period=(unsigned int)bs004_filter_par[3]-1; 		//圆点博士:传感器采样频率
	bs004_mpu6050_gyro_scale=1.0f/(float)bs004_filter_par[4];				//圆点博士:陀螺仪灵敏度
	bs004_mpu6050_acc_scale=(float)bs004_filter_par[5]/1000.0f;			//圆点博士:加速度灵敏度
	bs004_hmc5883l_mag_scale=(float)bs004_filter_par[6]/1000.0f;		//圆点博士:磁力计灵敏度
	//bs004_motor_pwm_period=(unsigned int)bs004_filter_par[7]*36;	  //圆点博士:电机控制周期
	//BS004_Motor_Scale=(unsigned int)bs004_filter_par[8];          	//圆点博士:电机控制灵敏度
	bs004_quad_halfT=(float)bs004_filter_par[9]/1000.0f;						//圆点博士:四元数时间系数
	bs004_quad_Kp=(float)bs004_filter_par[10]/1000.0f;							//圆点博士:四元数比例系数
	bs004_quad_Ki=(float)bs004_filter_par[11]/1000.0f;							//圆点博士:四元数积分系数	
}

void BS004_Load_Pid_Parameter(void)
{
	int bs004_pid_par[12];
	unsigned char i;
	//
	for(i=0;i<3;i++) pid_setting_P_value[i]=(unsigned int)bs004_pid_par[i];
	for(i=0;i<3;i++) pid_setting_I_value[i]=(unsigned int)bs004_pid_par[i+3];
	for(i=0;i<3;i++) pid_setting_D_value[i]=(unsigned int)bs004_pid_par[i+6];
	//bs004_fly_gas_scale=(float)bs004_pid_par[9]/1.0f;
	//bs004_fly_pitch_scale=(float)bs004_pid_par[10]/1.0f;
	//bs004_fly_roll_scale=(float)bs004_pid_par[10]/1.0f;
	//bs004_fly_yaw_scale=(float)bs004_pid_par[11]/1.0f;
}

void BS004_Load_Calibration_Parameter(void)
{
	int bs004_cal_par[12];
	int i;
	for(i=0;i<12;i++) bs004_cal_par[i]=0;
	//
	bs004_mpu6050_gyro_pitch_cal=(signed short int)bs004_cal_par[0];
	bs004_mpu6050_gyro_roll_cal=(signed short int)bs004_cal_par[1];
	bs004_mpu6050_gyro_yaw_cal=(signed short int)bs004_cal_par[2];
	bs004_mpu6050_acc_roll_cal=(signed short int)bs004_cal_par[3];
	bs004_mpu6050_acc_pitch_cal=(signed short int)bs004_cal_par[4];
	bs004_mpu6050_acc_yaw_cal=(signed short int)bs004_cal_par[5];
	bs004_hmc5883l_mag_pitch_cal=(signed short int)bs004_cal_par[6];
	bs004_hmc5883l_mag_roll_cal=(signed short int)bs004_cal_par[7];
	bs004_hmc5883l_mag_yaw_cal=(signed short int)bs004_cal_par[8];
}
