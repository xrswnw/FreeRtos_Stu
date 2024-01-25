#ifndef UART2_H_
#define UART2_H_

#include "Uart2_HL.h"
#include "AnyID_Uart_Receive.h"











extern UART_RCVFRAME g_sUartRcvFrame;

extern UART_RCVFRAME g_sUartRcvTempFrame;

extern USART_RCVFRAME g_sUsartRcvFrame;
extern USART_RCVFRAME g_sUsartRcvTempFrame; 
void Uart_Init(uint32_t bud);
#endif