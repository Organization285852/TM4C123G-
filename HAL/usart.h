#ifndef __USART_H
#define __USART_H



#include"stdint.h"



void uart0_init(uint32_t clk);
void UART0Send(uint8_t *pucBuffer, uint32_t ulCount);

void uart1_init(uint32_t clk);
void UART1Send(uint8_t *pucBuffer, uint32_t ulCount);

void uart2_init(uint32_t clk);
void UART2Send(uint8_t *pucBuffer, uint32_t ulCount);

void uart3_init(uint32_t clk);
void UART3SendBuf(uint8_t *pucBuffer, uint32_t ulCount);

void uart7_init(uint32_t clk);
void UART7SendBuf(uint8_t *pucBuffer, uint32_t ulCount);

void timer_ISR(void);

#endif
