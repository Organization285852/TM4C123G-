#include "includes.h"
#include "ultrasonic.h"
#include "usart.h"


void Ultrasonic_Init()
{
	u8 temp = 0x55;
  uart1_init(9600);			//����1��ʼ������������Ϊ������
	UART1Send(&temp ,1);
}

s8 ultra_start_f;
u8 ultra_time;
u8 ultra_ok = 0;
void Ultra_Duty()
{
	u8 temp[3];

	ultra_time++;
	ultra_time = ultra_time%2;
	
	
	if( ultra_time == 0 )  //100ms//���÷����жϣ���ʡʱ�䡣
	{
/*//////////////////////////////////////////////
		UART5->DR = 0xe8;   //ks103��ַ�������ã�
		while( (UART5->SR & USART_FLAG_TXE) == 0 );
		
		UART5->DR = 0x02;   //++++
		while( (UART5->SR & USART_FLAG_TXE) == 0 );

		UART5->DR = 0xbc;  //70ms,���¶Ȳ���
		while( (UART5->SR & USART_FLAG_TXE) == 0 );
		
//////////////////////////////////////////////
*/	
	#if defined(USE_KS103)
		temp[0] = 0xe8;
		temp[1] = 0x02;
		temp[2] = 0xbc;
		UART1Send(temp ,3);
	#elif defined(USE_US100)
		temp[0] = 0x55;
		UART1Send(temp ,1);
	#endif
///////////////////////////////////////////////
		ultra_start_f = 1;
	}
}

u16 ultra_distance,ultra_distance_old;
s16 ultra_delta;
void Ultra_Get(u8 com_data)
{
	static u8 ultra_tmp;
	
	if( ultra_start_f == 1 )
	{
		ultra_tmp = com_data;
		ultra_start_f = 2;
	}
	else if( ultra_start_f == 2 )
	{
		ultra_distance = (ultra_tmp<<8) + com_data;
		ultra_start_f = 0;
		ultra_ok = 1;
	}
	
	ultra_delta = ultra_distance - ultra_distance_old;
	
	ultra_distance_old = ultra_distance;
	
}


