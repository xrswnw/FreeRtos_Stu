#include "Uart2.h"

USART_RCVFRAME g_sUsartRcvFrame = {0};
USART_RCVFRAME g_sUsartRcvTempFrame = {0};

void Usart_Init(uint32_t bud)
{
	Usart_InitInterface(bud);
	Usart_DmaTxInit();
	Usart_DmaRxInit(g_sUsartRcvFrame.buffer, UART_BUFFER_MAX_LEN);
}