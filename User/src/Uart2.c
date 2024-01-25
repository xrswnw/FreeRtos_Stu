#include "Uart2.h"


UART_RCVFRAME g_sUartRcvFrame = {0};
UART_RCVFRAME g_sUartRcvTempFrame = {0}; 

USART_RCVFRAME g_sUsartRcvFrame = {0};
USART_RCVFRAME g_sUsartRcvTempFrame = {0};
void Uart_Init(uint32_t bud)
{
/*	Uart2_HLInit(115200);*/
	
	Uart_InitInterface(bud);
	Uart_DmaTxInit();
	Uart_DmaRxInit(g_sUsartRcvFrame.buffer, UART_BUFFER_MAX_LEN);
}