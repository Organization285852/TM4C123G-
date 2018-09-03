#ifndef __RPY_H
#define __RPY_H

extern unsigned char mpuflag;
int MPU6050Check(void);
void MPU6050Rfresh(float *pitch,float *roll,float *yaw);
int MPU6050Init(void);
void RPYResiver(unsigned char byte);

#endif




