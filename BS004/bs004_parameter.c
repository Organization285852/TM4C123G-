/*************************************************************************************************************
Բ�㲩ʿС���������2014������Դ��������:
��Դ��������ο�,Բ�㲩ʿ����Դ�����ṩ�κ���ʽ�ĵ���,Ҳ������ʹ�ø�Դ��������ֵ���ʧ����.
�û�������ѧϰ��Ŀ���޸ĺ�ʹ�ø�Դ����.
���û����޸ĸ�Դ����ʱ,�����Ƴ��ò��ְ�Ȩ��Ϣ�����뱣��ԭ������.

������Ϣ������ʹٷ���վwww.etootle.com, �ٷ�����:http://weibo.com/xiaosizhou
**************************************************************************************************************/
#include "bs004_parameter.h" 
//
unsigned int bs004_load_par_sign=0;
//
void BS004_Load_Fly_Parameter(void)
{
	bs004_load_par_sign=0; 	//Բ�㲩ʿ:װ�ز�����־        
	//
	BS004_Load_Filter_Parameter();			//Բ�㲩ʿ:װ�ز���
	BS004_Load_Pid_Parameter();         //Բ�㲩ʿ:װ�ز���
	BS004_Load_Calibration_Parameter(); //Բ�㲩ʿ:װ�ز���
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
	bs004_filter_high=(float)bs004_filter_par[0]/1000.0f;						//Բ�㲩ʿ:�����˲�����
	bs004_filter_low=(float)bs004_filter_par[1]/1000.0f;						//Բ�㲩ʿ:�����˲�����	
	bs004_filter_time=(float)bs004_filter_par[2]/1000.0f;						//Բ�㲩ʿ:�����˲�����		
	//bs004_sys_timer_period=(unsigned int)bs004_filter_par[3]-1; 		//Բ�㲩ʿ:����������Ƶ��
	bs004_mpu6050_gyro_scale=1.0f/(float)bs004_filter_par[4];				//Բ�㲩ʿ:������������
	bs004_mpu6050_acc_scale=(float)bs004_filter_par[5]/1000.0f;			//Բ�㲩ʿ:���ٶ�������
	bs004_hmc5883l_mag_scale=(float)bs004_filter_par[6]/1000.0f;		//Բ�㲩ʿ:������������
	//bs004_motor_pwm_period=(unsigned int)bs004_filter_par[7]*36;	  //Բ�㲩ʿ:�����������
	//BS004_Motor_Scale=(unsigned int)bs004_filter_par[8];          	//Բ�㲩ʿ:�������������
	bs004_quad_halfT=(float)bs004_filter_par[9]/1000.0f;						//Բ�㲩ʿ:��Ԫ��ʱ��ϵ��
	bs004_quad_Kp=(float)bs004_filter_par[10]/1000.0f;							//Բ�㲩ʿ:��Ԫ������ϵ��
	bs004_quad_Ki=(float)bs004_filter_par[11]/1000.0f;							//Բ�㲩ʿ:��Ԫ������ϵ��	
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
