#include "usart.h"
#include "Delay.h"
unsigned char Re_buf[11],counter;
unsigned char ucStra[6],ucStrw[6],ucStrAngle[6],sum;
float pitch_offset,roll_offset;
float pitch,roll,yaw;
unsigned char mpuflag;
int MPU6050Check(void)
{
	unsigned char b;
		b =0xff;
	UART0Send(&b,1);
		b =0xAA;
	UART0Send(&b,1);
	
		b =0x52;
	UART0Send(&b,1);
	
  Delay_ms(100);
//  roll = ((short)(ucStrAngle[1]<<8| ucStrAngle[0]))/32768.0*180;
//  pitch = ((short)(ucStrAngle[3]<<8| ucStrAngle[2]))/32768.0*180;
  yaw = ((short)(ucStrAngle[5]<<8| ucStrAngle[4]))/32768.0*180;
  if(yaw==0)
    return 1;
  else
    return 0;
}
int MPU6050Init(void)
{
  unsigned char i=0;
  float temp,temr;
  while((MPU6050Check()==0)&&i<200)
    i++;
  if(i>=200)
    return 1;//检测错误
	
  i = 0;
  while(i<200)
  {
    roll = ((short)(ucStrAngle[1]<<8| ucStrAngle[0]))/32768.0*180;
    pitch = ((short)(ucStrAngle[3]<<8| ucStrAngle[2]))/32768.0*180;
    Delay_ms(10);
    if((temp==pitch)&&(temr==roll))
      break;
    temp = pitch;
    temr = roll; 
    i++;
  }
  if(i>=200)
    return 2;//检测错误
  else
  {
    pitch_offset = pitch;
    roll_offset = roll;
    return 0;
  }
}
void MPU6050Rfresh(float *pitch,float *roll,float *yaw)
{
  if(mpuflag)
  {
    mpuflag = 0;
    *pitch = -(((short)(ucStrAngle[3]<<8| ucStrAngle[2]))/32768.0*180-pitch_offset);
    *roll  = ((short)(ucStrAngle[1]<<8| ucStrAngle[0]))/32768.0*180-roll_offset;
    *yaw   = -(((short)(ucStrAngle[5]<<8| ucStrAngle[4]))/32768.0*180);
  }
}

void RPYResiver(unsigned char byte)
{
    Re_buf[counter]=byte;
    if(counter<10)
      sum += Re_buf[counter];
  //  USART1SendData(Re_buf[counter]);
    if(counter==0&&Re_buf[0]!=0x55) return;      //第0号数据不是帧头         
      counter++; 
    if(counter==11)             //接收到11个数据
    {    
      if(Re_buf [1]==0x53)
      {
        if(sum==Re_buf[10])
        {
          mpuflag = 1;
        }
      }
      sum = 0;
      counter=0;               //重新赋值，准备下一帧数据的接收        
      switch(Re_buf [1])
      {
        case 0x51:
	ucStra[0]=Re_buf[2];
	ucStra[1]=Re_buf[3];
        ucStra[2]=Re_buf[4];
	ucStra[3]=Re_buf[5];
	ucStra[4]=Re_buf[6];
	ucStra[5]=Re_buf[7];
	break;
	case 0x52:	 
	ucStrw[0]=Re_buf[2];
	ucStrw[1]=Re_buf[3];
	ucStrw[2]=Re_buf[4];
	ucStrw[3]=Re_buf[5];
	ucStrw[4]=Re_buf[6];
	ucStrw[5]=Re_buf[7];
	break;
	case 0x53: 
	ucStrAngle[0]=Re_buf[2];
	ucStrAngle[1]=Re_buf[3];
	ucStrAngle[2]=Re_buf[4];
	ucStrAngle[3]=Re_buf[5];
	ucStrAngle[4]=Re_buf[6];
	ucStrAngle[5]=Re_buf[7];	
	break;
       } 
      }
}
