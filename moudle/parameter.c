/******************** (C) COPYRIGHT 2014 ANO Tech ********************************
  * ����   �������ƴ�
 * �ļ���  ��parameter.c
 * ����    ���������õ�
 * ����    ��www.anotc.com
 * �Ա�    ��anotc.taobao.com
 * ����QȺ ��190169595
**********************************************************************************/

#include "includes.h"
#include "mpu6050.h"
#include "ak8975.h"
#include "ctrl.h"
#include "string.h"
#include "ff.h"
#include "height_ctrl.h"
#include "rc.h"

#define SENSOR_SETUP_FILE      "sensor.bin"
#define PID_SETUP_FILE         "pid.bin"
u8 flash_init_error;
sensor_setup_t sensor_setup;
pid_setup_t pid_setup;

/* �ļ���ض��� */
static FATFS fs;
static 	FIL file;
static 	DIR DirInf;
	
static int32_t Para_ReadSettingFromFile(void)
{
	FRESULT result;
	uint32_t bw;

 	/* �����ļ�ϵͳ */
	result = f_mount(&fs, "0:", 1);			/* Mount a logical drive */
	if (result != FR_OK)
	{
		/* ������ز��ɹ������и�ʽ�� */
		result = f_mkfs("0:",0,0);
		if (result != FR_OK)
		{
			 return -1;     //flash�����⣬�޷���ʽ��
		}
		else
		{
			/* ���½��й��� */
			result = f_mount(&fs, "0:", 0);			/* Mount a logical drive */
	   if (result != FR_OK)
      {
			 	/* ж���ļ�ϵͳ */
	      f_mount(NULL, "0:", 0);
			 return -2 ;
			}
		}
	}

	/* �򿪸��ļ��� */
	result = f_opendir(&DirInf, "/"); 
	if (result != FR_OK)
	{
		/* ж���ļ�ϵͳ */
	  f_mount(NULL, "0:", 0);
		return -3;
	}

	/* ���ļ� */
	result = f_open(&file, SENSOR_SETUP_FILE, FA_OPEN_EXISTING | FA_READ);
	if (result !=  FR_OK)
	{
	  /* ж���ļ�ϵͳ */
	  f_mount(NULL, "0:", 0);
   /* �ļ������� */
		return -4;
	}

	/* ��ȡSensor�����ļ� */
	result = f_read(&file, &sensor_setup.raw_data, sizeof(sensor_setup), &bw);
	if (bw > 0)
	{
		/* �ر��ļ�*/
	 f_close(&file);
		/* ���ļ� */
	 result = f_open(&file, PID_SETUP_FILE, FA_OPEN_EXISTING | FA_READ);
	  if (result !=  FR_OK)
	 {
		/* ж���ļ�ϵͳ */
	  f_mount(NULL, "0:", 0);
		return -4;
	 }
		/* ��ȡPID�����ļ� */
	 result = f_read(&file, &pid_setup.raw_data, sizeof(pid_setup), &bw);
    if(bw > 0)
		{
		 /* �ر��ļ�*/
	   f_close(&file);
		 	/* ж���ļ�ϵͳ */
	   f_mount(NULL, "0:", 0);
			return 1;
		}else
		{
		 /* �ر��ļ�*/
	   f_close(&file);
		 	/* ж���ļ�ϵͳ */
	    f_mount(NULL, "0:", 0);
			return -4;
		}
	}else
  {
	 /* �ر��ļ�*/
	 f_close(&file);
	 	/* ж���ļ�ϵͳ */
	 f_mount(NULL, "0:", 0);
	 return -5;
	}

}

static int32_t Para_WriteSettingToFile(void)
{
	FRESULT result;
	uint32_t bw;

 	/* �����ļ�ϵͳ */
	result = f_mount(&fs, "0:", 0);			/* Mount a logical drive */
	if (result != FR_OK)
	{
		/* ������ز��ɹ������и�ʽ�� */
		result = f_mkfs("0:",0,0);
		if (result != FR_OK)
		{
			 return -1;     //flash�����⣬�޷���ʽ��
		}
		else
		{
			/* ���½��й��� */
			result = f_mount(&fs, "0:", 0);			/* Mount a logical drive */
	   if (result != FR_OK)
      {
			 	/* ж���ļ�ϵͳ */
	      f_mount(NULL, "0:", 0);
			 return -2 ;
			}
		}
	}

	/* �򿪸��ļ��� */
	result = f_opendir(&DirInf, "/"); 
	if (result != FR_OK)
	{
		/* ж���ļ�ϵͳ */
	  f_mount(NULL, "0:", 0);
		return -3;
	}

	/* ���ļ� */
	result = f_open(&file, SENSOR_SETUP_FILE, FA_CREATE_ALWAYS | FA_WRITE);
	if (result !=  FR_OK)
	{
	  /* ж���ļ�ϵͳ */
	  f_mount(NULL, "0:", 0);
		return -4;
	}

	/* д��Sensor�����ļ� */
	result = f_write(&file, &sensor_setup.raw_data, sizeof(sensor_setup), &bw);
	if (result == FR_OK)
	{
		/* �ر��ļ�*/
	 f_close(&file);
		/* ���ļ� */
	 result = f_open(&file, PID_SETUP_FILE, FA_CREATE_ALWAYS | FA_WRITE);
	  if (result !=  FR_OK)
	 {
		/* ж���ļ�ϵͳ */
	  f_mount(NULL, "0:", 0);
		return -4;
	 }
		/* д��PID�����ļ� */
	 result = f_write(&file, &pid_setup.raw_data, sizeof(pid_setup), &bw);
    if(result == FR_OK)
		{
	 		/* �ر��ļ�*/
	    f_close(&file);
		 	/* ж���ļ�ϵͳ */
	   f_mount(NULL, "0:", 0);
			return 1;
		}else
		{
		  /* �ر��ļ�*/
	   f_close(&file);
		 	/* ж���ļ�ϵͳ */
	    f_mount(NULL, "0:", 0);
			return -4;
		}
	}else
  {
	  /* �ر��ļ�*/
	  f_close(&file);
		/* ж���ļ�ϵͳ */
	  f_mount(NULL, "0:", 0);
	 return -5;
	}

}


static void  Param_SetSettingToFC(void) 
{
	memcpy(&mpu6050.Acc_Offset,&sensor_setup.Offset.Accel,sizeof(xyz_f_t));
	memcpy(&mpu6050.Gyro_Offset,&sensor_setup.Offset.Gyro,sizeof(xyz_f_t));
	memcpy(&ak8975.Mag_Offset,&sensor_setup.Offset.Mag,sizeof(xyz_f_t));
	memcpy(&mpu6050.vec_3d_cali,&sensor_setup.Offset.vec_3d_cali,sizeof(xyz_f_t));
	
	mpu6050.Acc_Temprea_Offset = sensor_setup.Offset.Acc_Temperature;
	mpu6050.Gyro_Temprea_Offset = sensor_setup.Offset.Gyro_Temperature;
  
  memcpy(&ctrl_1.PID[PIDROLL],&pid_setup.groups.ctrl1.roll,sizeof(pid_t));
	memcpy(&ctrl_1.PID[PIDPITCH],&pid_setup.groups.ctrl1.pitch,sizeof(pid_t));
	memcpy(&ctrl_1.PID[PIDYAW],&pid_setup.groups.ctrl1.yaw,sizeof(pid_t));
	
	
	memcpy(&ctrl_2.PID[PIDROLL],&pid_setup.groups.ctrl2.roll,sizeof(pid_t));
	memcpy(&ctrl_2.PID[PIDPITCH],&pid_setup.groups.ctrl2.pitch,sizeof(pid_t));
	memcpy(&ctrl_2.PID[PIDYAW],&pid_setup.groups.ctrl2.yaw,sizeof(pid_t));

}

void Para_ResetToFactorySetup(void)
{
	/* ������ز��ɹ������и�ʽ�� */
		f_mkfs("0:",1,0);
	
// 	/* ���ټ�Ĭ��У׼ֵ */
// 	sensor_setup.Offset.Accel.x = 0;
// 	sensor_setup.Offset.Accel.y = 0;
// 	sensor_setup.Offset.Accel.z = 0;
// 	/* ������Ĭ��У׼ֵ */
// 	sensor_setup.Offset.Gyro.x = 0;
// 	sensor_setup.Offset.Gyro.y = 0;
// 	sensor_setup.Offset.Gyro.z = 0;
// 	/* ����Ĭ��У׼ֵ */
// 	sensor_setup.Offset.Mag.x = 0;		
// 	sensor_setup.Offset.Mag.y = 0;		
// 	sensor_setup.Offset.Mag.z = 0;	
// 	/* ��ѹ��Ĭ��У׼ֵ */	
// 	sensor_setup.Offset.Baro = 0;
//    /* �¶�Ĭ��У׼ֵ */	
// 	sensor_setup.Offset.Acc_Temperature = 0;
// 	sensor_setup.Offset.Gyro_Temperature = 0;
	
  /* PID Ĭ��ֵ */
	pid_setup.groups.ctrl1.pitch.kp = 0.8;
	pid_setup.groups.ctrl1.roll.kp  = 0.8;	
	pid_setup.groups.ctrl1.yaw.kp   = 1.2;	
	
	pid_setup.groups.ctrl1.pitch.ki = 0.1;
	pid_setup.groups.ctrl1.roll.ki  = 0.1;	
	pid_setup.groups.ctrl1.yaw.ki   = 1.0;	
	
	
	pid_setup.groups.ctrl1.pitch.kd = 2.0;
	pid_setup.groups.ctrl1.roll.kd  = 2.0;	
	pid_setup.groups.ctrl1.yaw.kd   = 1.0;	
	
	pid_setup.groups.ctrl1.pitch.kdamp = 1;
	pid_setup.groups.ctrl1.roll.kdamp  = 1;	
	pid_setup.groups.ctrl1.yaw.kdamp   = 1;

  pid_setup.groups.ctrl2.pitch.kp = 0.5;
  pid_setup.groups.ctrl2.roll.kp  = 0.5;	
	pid_setup.groups.ctrl2.yaw.kp   = 0.2;	
	
	pid_setup.groups.ctrl2.pitch.ki = 0.05;
	pid_setup.groups.ctrl2.roll.ki  = 0.05;	
	pid_setup.groups.ctrl2.yaw.ki   = 0.05;	
	
	pid_setup.groups.ctrl2.pitch.kd = 0.3;
	pid_setup.groups.ctrl2.roll.kd  = 0.3;
  pid_setup.groups.ctrl2.yaw.kd   = 0.1;
	
	pid_setup.groups.ctrl3.kp = 1.0f;
	pid_setup.groups.ctrl3.ki = 1.0f;
	pid_setup.groups.ctrl3.kd = 1.0f;
	
	pid_setup.groups.ctrl4.kp = 1.0f;
	pid_setup.groups.ctrl4.ki = 1.0f;
	pid_setup.groups.ctrl4.kd = 1.0;
	
	pid_setup.groups.hc_sp.kp = 1.0f;
	pid_setup.groups.hc_sp.ki = 1.0f;
	pid_setup.groups.hc_sp.kd = 1.0f;
	
	pid_setup.groups.hc_height.kp = 1.0f;
	pid_setup.groups.hc_height.ki = 1.0f;
	pid_setup.groups.hc_height.kd = 1.0f;	
	
  Para_WriteSettingToFile();
	Param_SetSettingToFC();
	PID_Para_Init();
}

void PID_Para_Init()
{
	Ctrl_Para_Init();
//	WZ_Acc_PID_Init();
	WZ_Speed_PID_Init();
	Ultra_PID_Init();
//	Baro_PID_Init();

}

void Para_Init()
{
	int32_t result = Para_ReadSettingFromFile();
  if(result < 0)
  {
	 Para_ResetToFactorySetup();
	 flash_init_error = 1;
	}
	Param_SetSettingToFC();
	
	PID_Para_Init();
}

void Param_SaveAccelOffset(xyz_f_t *offset)
{
 memcpy(&mpu6050.Acc_Offset,offset,sizeof(xyz_f_t));
 memcpy(&sensor_setup.Offset.Accel, offset,sizeof(xyz_f_t));
	
 sensor_setup.Offset.Acc_Temperature = mpu6050.Acc_Temprea_Offset ;
	
 Para_WriteSettingToFile();
}

void Param_SaveGyroOffset(xyz_f_t *offset)
{
 memcpy(&mpu6050.Gyro_Offset,offset,sizeof(xyz_f_t));
 memcpy(&sensor_setup.Offset.Gyro, offset,sizeof(xyz_f_t));
	
 sensor_setup.Offset.Gyro_Temperature = mpu6050.Gyro_Temprea_Offset ;
	
 Para_WriteSettingToFile();
}

void Param_SaveMagOffset(xyz_f_t *offset)
{
 memcpy(&ak8975.Mag_Offset,offset,sizeof(xyz_f_t));
 memcpy(&sensor_setup.Offset.Mag, offset,sizeof(xyz_f_t));
 Para_WriteSettingToFile();
}

void Param_Save_3d_offset(xyz_f_t *offset)
{
 memcpy(&mpu6050.vec_3d_cali,offset,sizeof(xyz_f_t));
 memcpy(&sensor_setup.Offset.vec_3d_cali, offset,sizeof(xyz_f_t));
	
 Para_WriteSettingToFile();
}

void Param_SavePID(void)
{
 memcpy(&pid_setup.groups.ctrl1.roll,&ctrl_1.PID[PIDROLL],sizeof(pid_t));
 memcpy(&pid_setup.groups.ctrl1.pitch,&ctrl_1.PID[PIDPITCH],sizeof(pid_t));
 memcpy(&pid_setup.groups.ctrl1.yaw,&ctrl_1.PID[PIDYAW],sizeof(pid_t));
  
 memcpy(&pid_setup.groups.ctrl2.roll,&ctrl_2.PID[PIDROLL],sizeof(pid_t));
 memcpy(&pid_setup.groups.ctrl2.pitch,&ctrl_2.PID[PIDPITCH],sizeof(pid_t));
 memcpy(&pid_setup.groups.ctrl2.yaw,&ctrl_2.PID[PIDYAW],sizeof(pid_t));
 Para_WriteSettingToFile();
}
extern u16 flash_save_en_cnt;

void Parameter_Save()
{
  if( flash_save_en_cnt !=0 )
	{
		flash_save_en_cnt++;
	}

	if( flash_save_en_cnt > 60 ) // 20 *60 = 1200ms
	{
		flash_save_en_cnt = 0;
		if( !fly_ready )
		{
			Param_SavePID();
		}
	}
}


/******************* (C) COPYRIGHT 2014 ANO TECH *****END OF FILE************/
