
#include "includes.h"
#include "HW_TYPES.h"
#include "hw_gpio.h"
#include "pwm.h"


#define INIT_DUTY 5000  //基本脉冲宽度1ms,f=400hz （最大12500）

#define PWM_RADIO 5//(10000 - 5000)/1000.0 //PWM变化范围为5000—10000，归算大0-1000.脉冲范围1ms-2ms

//PWM时钟16分频
void PWMInit(void)
{
//
// Enable the PWM0 peripheral
//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
//
// Wait for the PWM0 module to be ready.
//
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM1))
	{	
	}
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	
  HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;//解锁

  HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;// (GPIOCR 寄存器的内容只有在 GPIOLOCK 寄存器解锁时才能被修改)对PF0进行解锁
    //
    // Configure the GPIO pin muxing to select PWM functions for these pins.
    // This step selects which alternate function is available for these pins.
    // This is necessary if your part supports GPIO pin function muxing.
    // Consult the data sheet to see which functions are allocated per pin.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinConfigure(GPIO_PF0_M1PWM4);
    GPIOPinConfigure(GPIO_PF1_M1PWM5);
    GPIOPinConfigure(GPIO_PF2_M1PWM6);
    GPIOPinConfigure(GPIO_PF3_M1PWM7);
    //
    // Configure the GPIO pad for PWM function on pins PB6 and PB7.  Consult
    // the data sheet to see which functions are allocated per pin.
    // TODO: change this to select the port/pin you are using.
    //
	  GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_0);
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1);
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2);
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_3);
  
	
//
// Configure the PWM generator for count down mode with immediate updates
// to the parameters.
//
	PWMGenConfigure(PWM1_BASE,PWM_GEN_2,PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	PWMGenConfigure(PWM1_BASE,PWM_GEN_3,PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);//PWM_GEN_X须单独设置
//
// Set the period. For a 50 KHz frequency, the period = 1/50,000, or 20
// microseconds. For a 20 MHz clock, this translates to 400 clock ticks.
	
// Use this value to set the period.
	
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2,12500);//400Hz
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3,12500);//400Hz
//
// Set the pulse width of PWM0 for a 25% duty cycle.
//
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_4,5000);//1ms
//
// Set the pulse width of PWM1 for 1ms cycle.
//
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5,5000);
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,5000);
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,5000);
//
// Start the timers in generator 0 ，1.
//
	PWMGenEnable(PWM1_BASE,PWM_GEN_2);
	PWMGenEnable(PWM1_BASE,PWM_GEN_3);
//
// Enable the outputs.
//
	PWMOutputState(PWM1_BASE, (PWM_OUT_4_BIT | PWM_OUT_5_BIT|PWM_OUT_6_BIT | PWM_OUT_7_BIT), true);
}


u8 CH_out_Mapping[MAXMOTORS] = {0,1,2,3};

void CH_out_Mapping_Fun(u16 *out,u16 *mapped )
{
	u8 i;
	for( i = 0 ; i < MAXMOTORS ; i++ )
	{
		*( mapped + i ) = *( out + CH_out_Mapping[i] );
	}
}

void SetPwm(int16_t pwm[MAXMOTORS],s16 min,s16 max)
{
	u8 i;
	s16 pwm_tem[MAXMOTORS];
	for(i=0;i<MAXMOTORS;i++)
	{
			pwm_tem[i] = pwm[i] ;
			pwm_tem[i] = LIMIT(pwm_tem[i],min,max);
	}
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_4,PWM_RADIO *( pwm_tem[CH_out_Mapping[0]] ) + INIT_DUTY);
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5,PWM_RADIO *( pwm_tem[CH_out_Mapping[1]] ) + INIT_DUTY);
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,PWM_RADIO *( pwm_tem[CH_out_Mapping[2]] ) + INIT_DUTY);
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,PWM_RADIO *( pwm_tem[CH_out_Mapping[3]] ) + INIT_DUTY);
}

