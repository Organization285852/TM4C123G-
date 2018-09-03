#ifndef _LED_H_
#define	_LED_H_

#include "includes.h"

#define LED2_ON   		 GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4,0x00)
#define LED2_OFF       GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4,0x10)
#define LED3_ON        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5,0x00)
#define LED3_OFF       GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5,0x20)
#define LED1_ON        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6,0x00)
#define LED1_OFF       GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6,0x40)

//#define LED4_OFF         ANO_GPIO_LED->BSRRL = ANO_Pin_LED4
//#define LED4_ON          ANO_GPIO_LED->BSRRH = ANO_Pin_LED4


void LED_Init(void);
void LED_Display( u8 []);
void LED_Duty(void);

void LED_MPU_Err(void);
void LED_Mag_Err(void);
void LED_MS5611_Err(void);

extern u8 LED_Brightness[4];
#endif

