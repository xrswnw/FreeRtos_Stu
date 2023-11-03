#ifndef UART2_HL_H_
#define UART2_HL_H_

#include "Config.h"

#define RS485_PORT		UART3

#define RS485_BAUD_RATE						115200



extern const PORT_INF RS485_PORT_CTRL;
#define R485_EnableRx()                     do{GPIO_BC(RS485_PORT_CTRL.port) = RS485_PORT_CTRL.pin;}while(0)
#define R485_EnableTx()                     do{GPIO_BOP(RS485_PORT_CTRL.port) = RS485_PORT_CTRL.pin;}while(0)










void Uart2_HLInit(uint32_t baud);
void Uart_SendByte(uint8_t ch);
void Uart_SendBuf(uint8_t *pFrame, uint32_t len);

void R485_Delayms(uint32_t n);          
#endif















