#include "includes.h"
#include "rom.h"

u16 Rc_Pwm_In[8];
u32 Pwm_In[8];
void PWMInInit(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER2); 
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER3); 
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlDelay(200);//配置完系统寄存器后要延时一段时间 
	//GPIO config  
	GPIOPinConfigure(GPIO_PD0_WT2CCP0);  
	GPIOPinConfigure(GPIO_PD1_WT2CCP1);  
	GPIOPinConfigure(GPIO_PD2_WT3CCP0);  
	GPIOPinConfigure(GPIO_PD3_WT3CCP1);
	
  GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_0);  
  GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_1); 
	GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_2);  
  GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_3); 
  //Timer config  
  TimerConfigure(WTIMER2_BASE,TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_CAP_TIME_UP);  
	TimerConfigure(WTIMER2_BASE,TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_TIME_UP); 
	TimerConfigure(WTIMER3_BASE,TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_CAP_TIME_UP);  
	TimerConfigure(WTIMER3_BASE,TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_TIME_UP);
	
  TimerControlEvent(WTIMER2_BASE,TIMER_A,TIMER_EVENT_BOTH_EDGES);  
	TimerControlEvent(WTIMER2_BASE,TIMER_B,TIMER_EVENT_BOTH_EDGES);
	TimerControlEvent(WTIMER3_BASE,TIMER_A,TIMER_EVENT_BOTH_EDGES);
	TimerControlEvent(WTIMER3_BASE,TIMER_B,TIMER_EVENT_BOTH_EDGES);
      
  //Int config  
  IntEnable(INT_WTIMER2B);  
	IntEnable(INT_WTIMER2A); 
	
  TimerIntEnable(WTIMER2_BASE, TIMER_CAPB_EVENT);  
	TimerIntEnable(WTIMER2_BASE, TIMER_CAPA_EVENT); 
	TimerIntEnable(WTIMER3_BASE, TIMER_CAPB_EVENT);  
	TimerIntEnable(WTIMER3_BASE, TIMER_CAPA_EVENT);
	
  TimerEnable(WTIMER2_BASE,TIMER_A);  
	TimerEnable(WTIMER2_BASE,TIMER_B);  
	TimerEnable(WTIMER3_BASE,TIMER_A);  
	TimerEnable(WTIMER3_BASE,TIMER_B);  
}
void WTIMER2A_Handler(void)
{
	static u32 last;
	u32 temp;
	TimerIntClear(WTIMER2_BASE,TIMER_CAPA_EVENT); 
	if(GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_0))
		last = ROM_TimerValueGet(WTIMER2_BASE,TIMER_A);
	else
	{
		temp = ROM_TimerValueGet(WTIMER2_BASE,TIMER_A);
		if(temp>last)
			Pwm_In[0] = temp - last;
		else
			Pwm_In[0] =0xFFFFFFFF - last + temp;
		Rc_Pwm_In[0] = (u16)(Pwm_In[0]/80);
	}
}	
void WTIMER2B_Handler(void)
{
	static u32 last;
	u32 temp;
	TimerIntClear(WTIMER2_BASE,TIMER_CAPB_EVENT); 
	if(GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_1))
		last = ROM_TimerValueGet(WTIMER2_BASE,TIMER_B);
	else
	{
		temp = ROM_TimerValueGet(WTIMER2_BASE,TIMER_B);
		if(temp>last)
			Pwm_In[0] = temp - last;
		else
			Pwm_In[0] =0xFFFFFFFF - last + temp;
		Rc_Pwm_In[0] = (u16)(Pwm_In[0]/80);
	}
}	
void WTIMER3A_Handler(void)
{
	static u32 last;
	u32 temp;
	TimerIntClear(WTIMER3_BASE,TIMER_CAPA_EVENT); 
	if(GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_2))
		last = ROM_TimerValueGet(WTIMER3_BASE,TIMER_A);
	else
	{
		temp = ROM_TimerValueGet(WTIMER3_BASE,TIMER_A);
		if(temp>last)
			Pwm_In[2] = temp - last;
		else
			Pwm_In[2] =0xFFFFFFFF - last + temp;
		Rc_Pwm_In[2] = (u16)(Pwm_In[2]/80);
	}
}	
void WTIMER3B_Handler(void)
{
	static u32 last;
	u32 temp;
	TimerIntClear(WTIMER3_BASE,TIMER_CAPB_EVENT); 
	if(GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_3))
		last = ROM_TimerValueGet(WTIMER3_BASE,TIMER_B);
	else
	{
		temp = ROM_TimerValueGet(WTIMER3_BASE,TIMER_B);
		if(temp>last)
			Pwm_In[3] = temp - last;
		else
			Pwm_In[3] =0xFFFFFFFF - last + temp;
		Rc_Pwm_In[3] = (u16)(Pwm_In[3]/80);
	}
}	

