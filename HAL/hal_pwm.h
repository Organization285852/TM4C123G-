#ifndef __HAL_PWM_H
#define __HAL_PWM_H

#define MAXMOTORS 		(4)		//电机数量

void PWMInit(void);
void SetPwm(signed short pwm[MAXMOTORS],short min,short max);
void CH_out_Mapping_Fun(unsigned short *out,unsigned short *mapped );
#endif



