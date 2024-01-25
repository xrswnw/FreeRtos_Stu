#ifndef UART2_HL_H_
#define UART2_HL_H_

#include "Config.h"
#include "AnyID_Uart_Receive.h"

#define UART2_PORT				USART2
#define UART2_PORTIRQN			USART2_IRQn
#define UART2_BAUD_RATE			115200
#define Uart2_IRQHandler		USART2_IRQHandler



extern const PORT_INF UART2_PORT_CTRL;
//#define Uart2_EnableRx()                     do{GPIO_BC(UART2_PORT_CTRL.port) = UART2_PORT_CTRL.pin;}while(0)
//#define Uart2_EnableTx()                     do{GPIO_BOP(UART2_PORT_CTRL.port) = UART2_PORT_CTRL.pin;}while(0)


#define Uart2_ReadByte()                 	(uint16_t)(GET_BITS(USART_DATA(UART2_PORT), 0U, 8U))


extern const PORT_INF USART_PORT_TX;
extern const PORT_INF USART_PORT_RX;
extern const PORT_INF USART_PORT_CTRL;

#define USART_PORT					USART2	
#define USART_PORT_IQRN				USART2_IRQn
#define USART_PORT_IQRNHANDLER		USART2_IRQHandler
#define USART_BAUD_RATE				115200

#define USART_DMA_CHANNEL			DMA0

#define USART_DMA_CHANNEL_TX		DMA_CH1
#define USART_DMA_CHANNEL_RX		DMA_CH2


#define Usart_PoeEnable()					do{GPIO_BOP(USART_PORT_CTRL.port) = USART_PORT_CTRL.pin;}while(0)


void Uart_InitInterface(u32 bud);
void Uart_DmaTxInit();
void Uart_DmaRxInit(u8 *pFrame, u16 len);
void Uart_EnableTx(u8 *pBuffer, u16 lenth);

//




void usart2_dma_send(uint8_t *buffer,uint16_t len);
void Uart2_HLInit(uint32_t baud);
void Uart2_SendByte(uint8_t ch);
void Uart2_SendBuf(uint8_t *pFrame, uint32_t len);
void Uart2_Delayms(uint32_t n);          
#endif















