/*!
    \file    gd32f30x_it.c
    \brief   interrupt service routines

    \version 2017-02-10, V1.0.0, firmware for GD32F30x
    \version 2018-10-10, V1.1.0, firmware for GD32F30x
    \version 2018-12-25, V2.0.0, firmware for GD32F30x
    \version 2020-09-30, V2.1.0, firmware for GD32F30x 
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "gd32f30x_it.h"

/*!
    \brief      this function handles NMI exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void NMI_Handler(void)
{
}

/*!
    \brief      this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardFault_Handler(void)
{
    /* if Hard Fault exception occurs, go to infinite loop */
    while (1){
    }
}

/*!
    \brief      this function handles MemManage exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void MemManage_Handler(void)
{
    /* if Memory Manage exception occurs, go to infinite loop */
    while (1){
    }
}

/*!
    \brief      this function handles BusFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void BusFault_Handler(void)
{
    /* if Bus Fault exception occurs, go to infinite loop */
    while (1){
    }
}

/*!
    \brief      this function handles UsageFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void UsageFault_Handler(void)
{
    /* if Usage Fault exception occurs, go to infinite loop */
    while (1){
    }
}

/*!
    \brief      this function handles SVC exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
/*
void SVC_Handler(void)
{
}
*/
/*!
    \brief      this function handles DebugMon exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DebugMon_Handler(void)
{
}

/*!
    \brief      this function handles PendSV exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
/*
void PendSV_Handler(void)
{
}*/

/*!
    \brief      this function handles SysTick exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
/*
void SysTick_Handler(void)
{

    delay_decrement();
}*/

/*
void Uart2_IRQHandler(void)
{
	if(usart_interrupt_flag_get(UART2_PORT, USART_INT_FLAG_RBNE) != RESET)
    {
        u8 byte = 0;
		g_sUartRcvFrame.idleTime = 0;
        usart_interrupt_flag_clear(UART2_PORT, USART_INT_FLAG_RBNE);
		byte = Uart2_ReadByte();										
		//g_sUartRcvFrame.buffer[g_sUartRcvFrame.length ++] = byte;
		Uart_ReceiveFrame(byte, &g_sUartRcvFrame);
    }
    else 
    {
        Uart2_ReadByte();
    }
}*/



void USART_PORT_IQRNHANDLER()
{
	if(RESET != usart_interrupt_flag_get(USART_PORT, USART_INT_FLAG_IDLE))
	{
		usart_data_receive(USART_PORT);	
		dma_channel_disable(USART_DMA_CHANNEL, USART_DMA_CHANNEL_RX);

		g_sUsartRcvFrame.length = UART_BUFFER_MAX_LEN - dma_transfer_number_get(USART_DMA_CHANNEL, USART_DMA_CHANNEL_RX);
		g_sUsartRcvFrame.state |= UART_STAT_END;
		dma_transfer_number_config(USART_DMA_CHANNEL, USART_DMA_CHANNEL_RX, UART_BUFFER_MAX_LEN);

		dma_channel_enable(USART_DMA_CHANNEL, USART_DMA_CHANNEL_RX);
	}
}



void WIGHT_PORT_IQRNHANDLER(void)
{	//接入电脑，数据收发正常，但接入传感器设备无数据反馈，会返回FF，暂不清楚原始，DMA收发？？？？？？？？？？？？？？？？？？？？？
	if(RESET != usart_interrupt_flag_get(WIGHT_PORT, USART_INT_FLAG_IDLE))
	{
		usart_data_receive(WIGHT_PORT);	
		dma_channel_disable(WIGHT_DMA_CHANNEL, WIGHT_DMA_CHANNEL_RX);

		g_sWightFrame.rxLen = WIGHT_BUFFER_MAX_LEN - dma_transfer_number_get(WIGHT_DMA_CHANNEL, WIGHT_DMA_CHANNEL_RX);
		if(g_sWightFrame.rxLen == 1)
		{
			u8 in = 0;
			in ++;
		}
		g_sWightFrame.state |= WIGHT_STAT_END;
		dma_transfer_number_config(WIGHT_DMA_CHANNEL, WIGHT_DMA_CHANNEL_RX, WIGHT_BUFFER_MAX_LEN);
		dma_channel_enable(WIGHT_DMA_CHANNEL, WIGHT_DMA_CHANNEL_RX);
	}

}


void DMA0_Channel6_IRQHandler(void)
{
	 if(dma_interrupt_flag_get(DMA0, DMA_CH6, DMA_INT_FLAG_FTF)!=RESET)
	 {
		dma_interrupt_flag_clear(DMA0, DMA_CH6, DMA_INT_FLAG_G);
		Wight_Delayms(4);//必须加延时2-4ms，不加延时报文后面少两个字节，且最后一个字节永远是0xff
		Wight_EnableRx();	 
	 }
}


