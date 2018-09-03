
#include "rc.h"
#include "height_ctrl.h"
#include "ultrasonic.h"
#include "hal_pwm_in.h"
#include "autofly.h"
#include "imu.h"
void BluetoothResive(u8 byte)
{
	switch(byte)
	{
		case 0x15:fly_ready = ~fly_ready;break;
//		case 0x10:CH_filter[2] += 5; if(CH_filter[2]>500)CH_filter[2]=500;break;
//		case 0x9:CH_filter[2] -= 5; if(CH_filter[2]<-500)CH_filter[2]=-500;break; //thr
//		case 0x10:Rc_Pwm_In[0] += 5; if(Rc_Pwm_In[0]>2000)Rc_Pwm_In[0]=2000;break;
//		case 0x9:Rc_Pwm_In[0] -= 5; if(Rc_Pwm_In[0]<1000)Rc_Pwm_In[0]=1000;break; //thr		
		case 0x10:Rc_Pwm_In[1] += 1; if(Rc_Pwm_In[1]>2000)Rc_Pwm_In[1]=2000;break;
		case 0x9:Rc_Pwm_In[1] -= 1; if(Rc_Pwm_In[1]<1000)Rc_Pwm_In[1]=1000;break; //thr				
		case 0x17:Rc_Pwm_In[1] = 1500;break;
		case 0x02: height_ctrl_mode = 2;break;//定高；
		case 0x00: height_ctrl_mode = 0;break;//解除定高；
		case 0x12: 
		MAX_HEIGHT += 50;
		  if(MAX_HEIGHT>1200) 
				MAX_HEIGHT =1200;
			break;
		case 0x13:
			MAX_HEIGHT -= 50;
			if(MAX_HEIGHT<100) 
				MAX_HEIGHT =100;
			break;
		case 0x14: fly_state.down=1;break;
		case 0x16: fly_state.rise=1;break;
		case 0x18: RPYCALIBRATE = 1;break;
		default :break;
	}
}	
