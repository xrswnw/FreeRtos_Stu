#ifndef UART2_H_
#define UART2_H_

#include "Uart2_HL.h"
#include "AnyID_Uart_Receive.h"



extern USART_RCVFRAME g_sUsartRcvFrame;
extern USART_RCVFRAME g_sUsartRcvTempFrame; 

void Usart_Init(uint32_t bud);

#endif