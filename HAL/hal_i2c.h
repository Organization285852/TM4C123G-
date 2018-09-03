#ifndef __HAL_I2C_H
#define __HAL_I2C_H

#include <stdint.h>

void I2C0Init(void);
uint32_t I2CReadToBuf(uint8_t adress,uint8_t reg,uint8_t *buf,uint8_t len);
uint32_t I2CWriteToReg(uint8_t adress,uint8_t reg,uint8_t *buf,uint8_t len);

#endif 



