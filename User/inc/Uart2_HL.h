#ifndef UART2_HL_H_
#define UART2_HL_H_

#include "Config.h"
#include "AnyID_Uart_Receive.h"

#define USART_BAUD_RATE				115200

#define USART_PORT					USART2	
#define USART_PORT_IQRN				USART2_IRQn
#define USART_PORT_IQRNHANDLER		USART2_IRQHandler

#define USART_DMA_CHANNEL			DMA0
#define USART_DMA_CHANNEL_TX		DMA_CH1
#define USART_DMA_CHANNEL_RX		DMA_CH2

extern const PORT_INF UART2_PORT_CTRL;
#define Usart_PoeEnable()					do{GPIO_BOP(USART_PORT_CTRL.port) = USART_PORT_CTRL.pin;}while(0)

#define Usart_ReadByte()                 	(uint16_t)(GET_BITS(USART_DATA(USART_PORT), 0U, 8U))

void Usart_Delayms(uint32_t n); 
void Usart_InitInterface(u32 bud);
void Usart_DmaTxInit();
void Usart_DmaRxInit(u8 *pFrame, u16 len);
void Usart_EnableTx(u8 *pBuffer, u16 lenth);

#endif















