#include "height_ctrl.h"
#include "ultrasonic.h"
#include "mymath.h"
#include "filter.h"
#include "ctrl.h"
#include "ms5611.h"
#include "rc.h"
#include "parameter.h"
#include "autofly.h"
_st_height_pid_v wz_speed_pid_v;
_st_height_pid wz_speed_pid;

float baro_speed;

float height_ctrl_out;
float wz_acc;

#define BARO_SPEED_NUM 100
float baro_speed_arr[BARO_SPEED_NUM + 1];
u16 baro_cnt[2];

#define EXP_Z_SPEED  ( 4.0f *my_deathzoom( (thr-500), 50 ) )
void Height_Ctrl(float T,float thr)
{	
	static float wz_speed_t;
	static u8 height_ctrl_start_f;
	static u16 hc_start_delay;
		static u8 hs_ctrl_cnt;
	
	switch( height_ctrl_start_f )
	{
		case 0:
	
		if( mpu6050.Acc.z > 4000 )
		{
			height_ctrl_start_f = 1;
		}	
		else if( ++hc_start_delay > 500 )
		{
			height_ctrl_start_f = 1;
		}
		break;
		
		case 1:
		
		wz_acc += ( 1 / ( 1 + 1 / ( 20 *3.14f *T ) ) ) *( (reference_v.z *mpu6050.Acc.z + reference_v.x *mpu6050.Acc.x + reference_v.y *mpu6050.Acc.y - 4096 ) - wz_acc );
		
		wz_speed_t += ( 1 / ( 1 + 1 / ( 0.5f *3.14f *T ) ) ) *(0.4f*(thr-500) - wz_speed_t);
		
		Moving_Average( (float)( baro_alt_speed *10),baro_speed_arr,BARO_SPEED_NUM, baro_cnt ,&baro_speed ); //单位mm/s
// 		if( baro_alt_speed > 2000 )
// 		{
// 			while(1);
// 		}
		if( height_ctrl_mode == 1)
		{
			//height_speed_ctrl(T,thr,0.8f*(thr-500),wz_speed_t);
			
			
			if(baro_ctrl_start==1)//20ms
			{
				height_speed_ctrl(0.02f,thr,( EXP_Z_SPEED ),baro_speed);//baro_alt_speed *10);///
				baro_ctrl_start = 0;
				Baro_Ctrl(0.02f,thr);
			}		
		}
		


//超声波定高	
		else if( height_ctrl_mode == 2)
		{
			hs_ctrl_cnt++;
			hs_ctrl_cnt = hs_ctrl_cnt%10;  //0 ~ 10;
			if(hs_ctrl_cnt == 0)
			{
				height_speed_ctrl(0.02f,thr,0.4f*ultra_ctrl_out,ultra_speed);  //2*10ms  // 在thr上加减 //内环速度控制
			}

			if( ultra_start_f == 0 )  //超声波获得新数据；
			{	
				
				Ultra_Ctrl(0.1f,thr);//超声波周期100ms 外环高度ＰＩＤ控制，还带有速度的检测。
				ultra_start_f = -1;
			}
		}

		
	////////////////////////////////////////////////////////////	
		if(height_ctrl_mode)
		{		
			height_ctrl_out = wz_speed_pid_v.pid_out;
		}
		else
		{
			height_ctrl_out = thr;
		}
		
		break; //case 1
		
		default: break;
		
	} //switch
}

void WZ_Speed_PID_Init()
{
	wz_speed_pid.kp = 0.3f *pid_setup.groups.hc_sp.kp; 
	wz_speed_pid.kd = 1.4f *pid_setup.groups.hc_sp.kd; 
	wz_speed_pid.ki = 0.12f *pid_setup.groups.hc_sp.ki; 
}

float wz_speed,wz_speed_old;

float wz_acc_mms2;
void height_speed_ctrl(float T,float thr,float exp_z_speed,float h_speed) // 在thr上加减
{
	static float thr_lpf;
	float height_thr;
	static float hc_acc_i,wz_speed_0,wz_speed_1;
	
	height_thr = LIMIT( 2 * thr , 0, 600 );
	
	thr_lpf += ( 1 / ( 1 + 1 / ( 0.5f *3.14f *T ) ) ) *( height_thr - thr_lpf );
	
	wz_acc_mms2 = (wz_acc/4096.0f) *10000 + hc_acc_i;//9800 *T;
	
	
	
	wz_speed_0 += my_deathzoom( (wz_acc_mms2 ) ,100) *T;
	
	hc_acc_i += 0.4f *T *( (wz_speed - wz_speed_old)/T - wz_acc_mms2 );
	hc_acc_i = LIMIT( hc_acc_i, -500, 500 );	
	
	wz_speed_0 += ( 1 / ( 1 + 1 / ( 0.1f *3.14f *T ) ) ) *( h_speed - wz_speed_0  ) ;
	
	wz_speed_1 = wz_speed_0;
	
	if( ABS( wz_speed_1 ) < 50 )
	{
		wz_speed_1 = 0;
	}
	
	wz_speed_old = wz_speed;
	
	wz_speed = wz_speed_1;
	
	wz_speed_pid_v.err = wz_speed_pid.kp *( exp_z_speed - wz_speed );
	wz_speed_pid_v.err_d = 0.002f/T *10*wz_speed_pid.kd * (-wz_acc_mms2) *T;//(wz_speed_pid_v.err - wz_speed_pid_v.err_old);
	
	//wz_speed_pid_v.err_i += wz_speed_pid.ki *wz_speed_pid_v.err *T;
	wz_speed_pid_v.err_i += wz_speed_pid.ki *wz_speed_pid.kp *( exp_z_speed - h_speed ) *T;
	wz_speed_pid_v.err_i = LIMIT(wz_speed_pid_v.err_i,-Thr_Weight *300,Thr_Weight *300);
	
	wz_speed_pid_v.pid_out = thr_lpf + Thr_Weight *LIMIT((wz_speed_pid.kp *exp_z_speed + wz_speed_pid_v.err + wz_speed_pid_v.err_d + wz_speed_pid_v.err_i),-300,300);

	wz_speed_pid_v.err_old = wz_speed_pid_v.err; 
}

u8 baro_ctrl_start;
float baro_height,baro_height_old;
float baro_measure;

void Baro_Ctrl(float T,float thr)
{
	static float start_height;

	/*        
        Q:过程噪声，Q增大，动态响应变快，收敛稳定性变坏
        R:测量噪声，R增大，动态响应变慢，收敛稳定性变好        
  */
    

  
	
	if( (s16)start_height == 0 )
	{
		start_height = baroAlt;
	}		
	baro_measure = 10 *( baroAlt - start_height );
	
	baro_height += T *wz_speed;
	
	baro_height += 0.2f *3.14f *T *(baro_measure - baro_height);
				

        
}

#define ULTRA_SPEED 		 300    // mm/s
#define ULTRA_MAX_HEIGHT 500   // mm
#define ULTRA_INT        300    // 积分幅度
float MAX_HEIGHT=150;
_st_height_pid_v ultra_ctrl;
_st_height_pid ultra_pid;

void Ultra_PID_Init()
{
	ultra_pid.kp = 1.5;
	ultra_pid.kd = 2.5;
	ultra_pid.ki = 0;
}

float exp_height_speed,exp_height;
float ultra_speed;
float ultra_dis_lpf;
float ultra_ctrl_out;

void Ultra_Ctrl(float T,float thr)
{
	float ultra_sp_tmp,ultra_dis_tmp;	
	int dh;
	exp_height_speed = ULTRA_SPEED *my_deathzoom_2(thr - 500,50)/200.0f; //+-ULTRA_SPEEDmm / s
	//这个并不是内环中期望速度的值，而我们想要的一个速度上升。
//	exp_height_speed = ABS(exp_height_speed);
	exp_height_speed = LIMIT(exp_height_speed ,-ULTRA_SPEED,ULTRA_SPEED);//一直是个正值
	
	if( exp_height > MAX_HEIGHT ) //exp_height
	{
		if( exp_height_speed > 0 )
		{
			exp_height_speed = -exp_height_speed;
		}
	}
//	dh = MAX_HEIGHT -  exp_height;
//	if(dh<0)
//		exp_height_speed = -exp_height_speed;
	else if( exp_height < 150 ) 
	{
		if( exp_height_speed < 0 )
		{
			exp_height_speed = 0;
		}
	}
//		if(dh>-50&&dh<50)
//	{
//		exp_height = MAX_HEIGHT;
//		exp_height_speed = 0;
//	}
	exp_height += exp_height_speed *T;
///////////////////	如果期望与实际高度相差比较大，即高度还未调节到位,跟随性不高。强制变化范围100
  if((exp_height-ultra_distance)>100)
		exp_height = ultra_distance + 100;
	if((exp_height-ultra_distance)<-100)
		exp_height = ultra_distance - 100;
//////////////////////////////////////	要求ultra_distance的噪声不能有太大的偏离，否可能引起系统振荡。
	if(exp_height<120)
		exp_height = 120;
//	ChangeZ(exp_height_speed,500,T);
	
	if( thr < 100 )
	{
		exp_height += ( 1 / ( 1 + 1 / ( 0.2f *3.14f *T ) ) ) *( -exp_height);//滤波为零
	}
	
	
//获取测量值	
	ultra_sp_tmp = Moving_Median(0,5,ultra_delta/T); //ultra_delta/T; //上升速度。
	ultra_dis_tmp = Moving_Median(1,5,ultra_distance);
	
//速度低通滤波
	if( ultra_dis_tmp < 2000 )
	{
		if( ABS(ultra_sp_tmp) < 100 )
		{
			ultra_speed += ( 1 / ( 1 + 1 / ( 4 *3.14f *T ) ) ) * ( (float)(ultra_sp_tmp) - ultra_speed );
		}
		else
		{
			ultra_speed += ( 1 / ( 1 + 1 / ( 1.0f *3.14f *T ) ) ) * ( (float)(ultra_sp_tmp) - ultra_speed );
		}
	}
	
///高度低通滤波	
	if( ultra_dis_tmp < 2000 )
	{
		
		if( ABS(ultra_dis_tmp - ultra_dis_lpf) < 100 )
		{
			
			ultra_dis_lpf += ( 1 / ( 1 + 1 / ( 4.0f *3.14f *T ) ) ) *(ultra_dis_tmp - ultra_dis_lpf) ;
		}
		else if( ABS(ultra_dis_tmp - ultra_dis_lpf) < 200 )
		{
			
			ultra_dis_lpf += ( 1 / ( 1 + 1 / ( 2.2f *3.14f *T ) ) ) *(ultra_dis_tmp- ultra_dis_lpf) ;
		}
		else if( ABS(ultra_dis_tmp - ultra_dis_lpf) < 400 )
		{
			ultra_dis_lpf += ( 1 / ( 1 + 1 / ( 1.2f *3.14f *T ) ) ) *(ultra_dis_tmp- ultra_dis_lpf) ;
		}
		else
		{
			ultra_dis_lpf += ( 1 / ( 1 + 1 / ( 0.6f *3.14f *T ) ) ) *(ultra_dis_tmp- ultra_dis_lpf) ;
		}
	}
	else
	{
		
	}
//外环高度ＰＩＤ控制
	ultra_ctrl.err = ( ultra_pid.kp*(exp_height - ultra_dis_lpf));
	
	ultra_ctrl.err_i += ultra_pid.ki *ultra_ctrl.err *T;
	
	ultra_ctrl.err_i = LIMIT(ultra_ctrl.err_i,-Thr_Weight *ULTRA_INT,Thr_Weight *ULTRA_INT);
	
	ultra_ctrl.err_d = ultra_pid.kd *( 0.6f *(-wz_speed*T) + 0.4f *(ultra_ctrl.err - ultra_ctrl.err_old) );
	
	ultra_ctrl.pid_out = ultra_ctrl.err + ultra_ctrl.err_i + ultra_ctrl.err_d;
	
	ultra_ctrl.pid_out = LIMIT(ultra_ctrl.pid_out,-500,500);
		
	ultra_ctrl_out = ultra_ctrl.pid_out;
	
	ultra_ctrl.err_old = ultra_ctrl.err;
}

