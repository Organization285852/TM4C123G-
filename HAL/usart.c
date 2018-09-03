
#include "includes.h"
#include "ultrasonic.h"
#include "RPY.h"
#include "usart.h"
#include "bluetooth.h"
#include "data_transfer.h"
#include "timer.h"
#include "tracking.h"
#include "hw_gpio.h"
#include "hw_types.h"
//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,UART0支持  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
//		UARTCharPut(UART2_BASE,ch);
//uint8_t byte=ch;
//	UART3SendBuf(&byte,1);
//	UART2Send(&byte,1);
	return ch;
}
#endif 

void uart0_init(uint32_t clk)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
//		while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0))
//		{
//		}
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Configure the GPIO pin muxing for the UART function.
    // This is only necessary if your part supports GPIO pin function muxing.
    // Study the data sheet to see which functions are allocated per pin.
    // TODO: change this to select the port/pin you are using
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);

    //
    // Since GPIO A0 and A1 are used for the UART function, they must be
    // configured for use as a peripheral function (instead of GPIO).
    // TODO: change this to match the port/pin you are using
    //
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
//		GPIOPadConfigSet(GPIO_PORTA_BASE,GPIO_PIN_0 | GPIO_PIN_1,GPIO_STRENGTH_10MA,GPIO_PIN_TYPE_STD_WPU);
//    HWREG(GPIO_PORTA_BASE + GPIO_O_PUR) |= 0x03;
    //
    // Configure the UART for 115,200, 8-N-1 operation.
    // This function uses SysCtlClockGet() to get the system clock
    // frequency.  This could be also be a variable or hard coded value
    // instead of a function call.
    //
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), clk,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                         UART_CONFIG_PAR_NONE));
												 
						 
		//使能串口中断
    IntEnable(INT_UART0);
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);								 
												 
}
void UART0_Handler(void)
{
	 uint32_t ulStatus,temp;
    //获取中断状态
   ulStatus = UARTIntStatus(UART0_BASE, true);
    //清除中断标志
   UARTIntClear(UART0_BASE, ulStatus);
	   if(UARTCharsAvail(UART0_BASE))
	 {
			temp = UARTCharGet(UART0_BASE);
	//	 RPYResiver(temp);
		 ResiveCameraData(temp);
	 }


}
//串口发送函数
void UART0Send(uint8_t *pucBuffer, uint32_t ulCount)
{
    while(ulCount--)
    {
       //将要发送的字符写进UART
				UARTCharPutNonBlocking(UART0_BASE, *pucBuffer++);
				//UARTCharPut(UART0_BASE, *pucBuffer++);
    }
}

void uart1_init(uint32_t clk)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
//		while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0))
//		{
//		}
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    //
    // Configure the GPIO pin muxing for the UART function.
    // This is only necessary if your part supports GPIO pin function muxing.
    // Study the data sheet to see which functions are allocated per pin.
    // TODO: change this to select the port/pin you are using
    //
    GPIOPinConfigure(GPIO_PC4_U1RX);
    GPIOPinConfigure(GPIO_PC5_U1TX);

    //
    // Since GPIO A0 and A1 are used for the UART function, they must be
    // configured for use as a peripheral function (instead of GPIO).
    // TODO: change this to match the port/pin you are using
    //
    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    //
    // Configure the UART for 115,200, 8-N-1 operation.
    // This function uses SysCtlClockGet() to get the system clock
    // frequency.  This could be also be a variable or hard coded value
    // instead of a function call.
    //
    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), clk,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                         UART_CONFIG_PAR_NONE));
												 
						 
		//使能串口中断
    IntEnable(INT_UART1);
    UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT);								 
												 
}
void UART1_Handler(void)
{
	 uint32_t ulStatus;
	u8 temp;
    //获取中断状态
   ulStatus = UARTIntStatus(UART1_BASE, true);
    //清除中断标志
   UARTIntClear(UART1_BASE, ulStatus);
	 temp = UARTCharGet(UART1_BASE);
	 Ultra_Get(temp);
//UARTCharPut(UART1_BASE, temp);
}
//串口发送函数
void UART1Send(uint8_t *pucBuffer, uint32_t ulCount)
{
    while(ulCount--)
    {
       //将要发送的字符写进UART
				UARTCharPutNonBlocking(UART1_BASE, *pucBuffer++);
				//UARTCharPut(UART0_BASE, *pucBuffer++);
			 
    }
}

void uart2_init(uint32_t clk)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
		while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART2))
		{
		}
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    //
    // Configure the GPIO pin muxing for the UART function.
    // This is only necessary if your part supports GPIO pin function muxing.
    // Study the data sheet to see which functions are allocated per pin.
    // TODO: change this to select the port/pin you are using
    //
    GPIOPinConfigure(GPIO_PD6_U2RX);
    GPIOPinConfigure(GPIO_PD7_U2TX);

    //
    // Since GPIO A0 and A1 are used for the UART function, they must be
    // configured for use as a peripheral function (instead of GPIO).
    // TODO: change this to match the port/pin you are using
    //
    GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);

    //
    // Configure the UART for 115,200, 8-N-1 operation.
    // This function uses SysCtlClockGet() to get the system clock
    // frequency.  This could be also be a variable or hard coded value
    // instead of a function call.
    //
    UARTConfigSetExpClk(UART2_BASE, SysCtlClockGet(), clk,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                         UART_CONFIG_PAR_NONE));
		UARTFIFOLevelSet(UART2_BASE,UART_FIFO_TX4_8,UART_FIFO_RX7_8);												 				 
		//使能串口中断
    IntEnable(INT_UART2);
    UARTIntEnable(UART2_BASE, UART_INT_RX | UART_INT_RT);								 
												 
}
u32 l;
void UART2_Handler(void)
{
	 uint32_t ulStatus;
	 u8 temp;
    //获取中断状态
  l= ulStatus = UARTIntStatus(UART2_BASE, true);
    //清除中断标志
   UARTIntClear(UART2_BASE, ulStatus);
	 temp = UARTCharGet(UART2_BASE);
	 BluetoothResive(temp);
}
//串口发送函数
void UART2Send(uint8_t *pucBuffer, uint32_t ulCount)
{
    while(ulCount--)
    {
       //将要发送的字符写进UART
				UARTCharPutNonBlocking(UART2_BASE, *pucBuffer++);
				//UARTCharPut(UART0_BASE, *pucBuffer++);
			 
    }
}
void uart3_init(uint32_t clk)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);
		while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART3))
		{
		}
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    //
    // Configure the GPIO pin muxing for the UART function.
    // This is only necessary if your part supports GPIO pin function muxing.
    // Study the data sheet to see which functions are allocated per pin.
    // TODO: change this to select the port/pin you are using
    //
    GPIOPinConfigure(GPIO_PC6_U3RX);
    GPIOPinConfigure(GPIO_PC7_U3TX);

    //
    // Since GPIO A0 and A1 are used for the UART function, they must be
    // configured for use as a peripheral function (instead of GPIO).
    // TODO: change this to match the port/pin you are using
    //
    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);

    //
    // Configure the UART for 115,200, 8-N-1 operation.
    // This function uses SysCtlClockGet() to get the system clock
    // frequency.  This could be also be a variable or hard coded value
    // instead of a function call.
    //
    UARTConfigSetExpClk(UART3_BASE, SysCtlClockGet(), clk,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                         UART_CONFIG_PAR_NONE));
		UARTFIFOLevelSet(UART3_BASE,UART_FIFO_TX4_8,UART_FIFO_RX7_8);									 
											 				 
		//使能串口中断
    IntEnable(INT_UART3);
    UARTIntEnable(UART3_BASE, UART_INT_RX | UART_INT_RT );			
}

u8 TxBuffer[256];
u8 TxCounter=0;
u8 count=0; 
u8 TI=0;//用于保存上次中断开关状态
u32 k;
void UART3_Handler(void)
{
	 uint32_t ulStatus;
	u8 temp;
    //获取中断状态
   k = ulStatus = UARTIntStatus(UART3_BASE, true);
    //清除中断标志
	UARTIntClear(UART3_BASE, ulStatus);
   if(UARTCharsAvail(UART3_BASE))
	 {
			temp = UARTCharGet(UART3_BASE);
			ANO_DT_Data_Receive_Prepare(temp);
	 }
}
//串口发送函
void UART3SendBuf(uint8_t *pucBuffer, uint32_t ulCount)
{
  u8 i;
	for(i=0;i<ulCount;i++)
	{
		TxBuffer[count++] = *(pucBuffer+i);
	}
	if(TI==0) //
	{
//	  UARTIntEnable(UART3_BASE,UART_INT_TX);
		TimerIntEnable(TIMER0_BASE,TIMER_TIMB_TIMEOUT);
		TI=1;
	}
}

void timer_ISR(void)
{

	if(UARTSpaceAvail(UART3_BASE))
	{
		UARTCharPutNonBlocking(UART3_BASE,TxBuffer[TxCounter++]);       
		if(TxCounter == count)
		{
			TI = 0;
//			UARTIntDisable(UART3_BASE,UART_INT_TX);		//关闭TXE（发送中断）中断
			TimerIntDisable(TIMER0_BASE,TIMER_TIMB_TIMEOUT);
		}
	}

}


void uart7_init(uint32_t clk)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART7);
		while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART7))
		{
		}
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    //
    // Configure the GPIO pin muxing for the UART function.
    // This is only necessary if your part supports GPIO pin function muxing.
    // Study the data sheet to see which functions are allocated per pin.
    // TODO: change this to select the port/pin you are using
    //
    GPIOPinConfigure(GPIO_PE0_U7RX);
    GPIOPinConfigure(GPIO_PE1_U7TX);

    //
    // Since GPIO A0 and A1 are used for the UART function, they must be
    // configured for use as a peripheral function (instead of GPIO).
    // TODO: change this to match the port/pin you are using
    //
    GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Configure the UART for 115,200, 8-N-1 operation.
    // This function uses SysCtlClockGet() to get the system clock
    // frequency.  This could be also be a variable or hard coded value
    // instead of a function call.
    //
    UARTConfigSetExpClk(UART7_BASE, SysCtlClockGet(), clk,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                         UART_CONFIG_PAR_NONE));
		UARTFIFOLevelSet(UART7_BASE,UART_FIFO_TX4_8,UART_FIFO_RX7_8);												 				 
		//使能串口中断
    IntEnable(INT_UART7);
    UARTIntEnable(UART7_BASE, UART_INT_RX | UART_INT_RT);								 
												 
}
void UART7_Handler(void)
{
	 uint32_t ulStatus;
	 u8 temp;
    //获取中断状态
   ulStatus = UARTIntStatus(UART7_BASE, true);
    //清除中断标志
   UARTIntClear(UART7_BASE, ulStatus);
	 temp = UARTCharGet(UART7_BASE);
		
}
//串口发送函数
void UART7Send(uint8_t *pucBuffer, uint32_t ulCount)
{
    while(ulCount--)
    {
       //将要发送的字符写进UART
				UARTCharPutNonBlocking(UART7_BASE, *pucBuffer++);
				//UARTCharPut(UART0_BASE, *pucBuffer++);
			 
    }
}
