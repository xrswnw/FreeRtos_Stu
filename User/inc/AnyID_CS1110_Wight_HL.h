#ifndef _ANYID_CS1110_WIGHT_HL_H
#define _ANYID_CS1110_WIGHT_HL_H

#include "Config.h"

#define WIGHT_BAUD_RATE				115200

#define WIGHT_PORT					USART1	
#define WIGHT_PORT_IQRN				USART1_IRQn
#define WIGHT_PORT_IQRNHANDLER		USART1_IRQHandler

#define WIGHT_DMA_CHANNEL					DMA0
#define WIGHT_DMA_CHANNEL_TX				DMA_CH6
#define WIGHT_DMA_CHANNEL_TXIQRN			DMA0_Channel6_IRQn
#define WIGHT_DMA_CHANNEL_RX				DMA_CH5


extern const PORT_INF USART1_CTRL;
#define Wight_EnableTx()                     do{GPIO_BOP(USART1_CTRL.port) = USART1_CTRL.pin;}while(0)
#define Wight_EnableRx()                     do{GPIO_BC(USART1_CTRL.port) = USART1_CTRL.pin;}while(0)

#define Wight_ReadByte()                 	(uint16_t)(GET_BITS(USART_DATA(WIGHT_PORT), 0U, 8U))

                                            

void Wight_Delayms(uint32_t n); 
void Wight_InitInterface(u32 bud);
void Wight_DmaTxInit();
void Wight_DmaRxInit(u8 *pFrame, u16 len);
void Wight_EnableDmaTx(u8 *pBuffer, u16 lenth);
void Wight_WriteByte(u8 da);
void Wight_WriteBuffer(u8 *pBuffer, u16 len);
#endif