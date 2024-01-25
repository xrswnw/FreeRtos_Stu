#ifndef _ANYID_CS1110_WIGHT_HL_H
#define _ANYID_CS1110_WIGHT_HL_H

#include "Config.h"

#define GPB_PORT                       USART1
#define GPB_IRQHandler                 USART1_IRQHandler
#define GPB_ChkTxOver()                while(((GPB_PORT)->SR & USART_FLAG_TC) == (u16)RESET)
#define GPB_ReadByte()                 	(uint16_t)(GET_BITS(USART_DATA(GPB_PORT), 0U, 8U))                                          
#define GPB_BAUDRARE                   115200

extern const PORT_INF GPB_PORT_CTRL;
#define GPB_EnableRx()                     do{GPIO_BOP(GPB_PORT_CTRL.port) = GPB_PORT_CTRL.pin;}while(0)
#define GPB_EnableTx()                     do{GPIO_BC(GPB_PORT_CTRL.port) = GPB_PORT_CTRL.pin;}while(0)

#define GPB_UART_DMA_CHANNEL			DMA0
#define GPB_UART_DMA_TX_CHANNEL			DMA_CH6
#define GPB_UART_DMA_TX_CHANNEL_IRQ		DMA0_Channel6_IRQn
#define GPB_UART_DMA_TX_CHANNEL_IRQHAND	DMA0_Channel6_IRQHandler

#define GPB_UART_DMA_RX_CHANNEL			DMA_CH5
#define GPB_UART_DMA_RX_CHANNEL_IRQ		DMA0_Channel5_IRQn
#define GPB_UART_DMA_RX_CHANNEL_IRQHAND	DMA0_Channel5_IRQHandler



#define GPB_GetRxLen()                     (GPB_BUFFER_MAX_LEN - (GPB_RXDMA_CH)->CNDTR)                                               

void GPB_InitInterface(u32 baudrate);
void Gpb_DmaTxInit(u8 *pBuffer, u16 len);
void Gpb_DmaRxInit(u8 *pBuffer, u16 len);
void GPB_Stop();
void GPB_WriteByte(u8 ch);
void GPB_Delayms(u32 n);
void GPB_WriteBuffer(u8 *pFrame, u16 len);
void Gpb_EnableTxDma(u8 *pFrame, u16 lenth);


#endif