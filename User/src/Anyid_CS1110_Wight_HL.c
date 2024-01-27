#include "AnyID_CS1110_Wight_HL.h"

const PORT_INF USART1_TX      = {GPIOA, GPIO_PIN_2};
const PORT_INF USART1_RX      = {GPIOA, GPIO_PIN_3};
const PORT_INF USART1_CTRL    = {GPIOA, GPIO_PIN_1};

void Wight_Delayms(uint32_t n)             //系统延时n毫秒
{
    n *= 0x6000;
    n++;
    while(n--);
}


void Wight_InitInterface(u32 bud)
{
	gpio_init(USART1_TX.port, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, USART1_TX.pin);
	gpio_init(USART1_RX.port, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, USART1_RX.pin);
	gpio_init(USART1_CTRL.port, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, USART1_CTRL.pin);
		
    usart_deinit(USART1);
    usart_baudrate_set(USART1, bud);
    usart_word_length_set(USART1, USART_WL_8BIT);
    usart_stop_bit_set(USART1, USART_STB_1BIT);
    usart_parity_config(USART1, USART_PM_NONE);
    usart_hardware_flow_rts_config(USART1, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART1, USART_CTS_DISABLE);
    usart_receive_config(USART1, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);
 	usart_enable(USART1);
	
	usart_dma_receive_config(USART1, USART_RECEIVE_DMA_ENABLE);
	usart_dma_transmit_config(USART1, USART_TRANSMIT_DMA_ENABLE);
	
	nvic_priority_group_set(NVIC_PRIGROUP_PRE3_SUB1);
	nvic_irq_enable(USART1_IRQn, 1, 0);
	usart_interrupt_enable(USART1, USART_INT_IDLE);
	usart_interrupt_enable(USART1, USART_INT_RBNE);

}

void Wight_DmaTxInit()
{
	dma_parameter_struct dma_init_struct;

    dma_deinit(DMA0, DMA_CH6);
    dma_struct_para_init(&dma_init_struct);

    dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_addr = NULL;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = NULL;
    dma_init_struct.periph_addr = (u32)&USART_DATA(USART1);
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(DMA0, DMA_CH6, &dma_init_struct);
    
    dma_circulation_disable(DMA0, DMA_CH6);
    dma_memory_to_memory_disable(DMA0, DMA_CH6);
    dma_channel_disable(DMA0, DMA_CH6);
	
	dma_interrupt_enable(DMA0, DMA_CH6,DMA_INT_FTF);
	
	nvic_irq_enable(DMA0_Channel6_IRQn, 2, 1);
	Wight_EnableRx();
	
	

}

void Wight_DmaRxInit(u8 *pFrame, u16 len)
{
	dma_parameter_struct dma_init_struct;
	
    dma_deinit(DMA0, DMA_CH5);
    dma_struct_para_init(&dma_init_struct);

    dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
    dma_init_struct.memory_addr = (uint32_t)pFrame;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;  
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = len; 
    dma_init_struct.periph_addr = (u32)&USART_DATA(USART1);
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.memory_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(DMA0, DMA_CH5, &dma_init_struct);
	
	dma_circulation_disable(DMA0, DMA_CH5);
    dma_memory_to_memory_disable(DMA0, DMA_CH5);
    dma_interrupt_enable(DMA0, DMA_CH5, DMA_INT_FTF);						//打开中断
    dma_channel_enable(DMA0, DMA_CH5);
		

}

void Wight_EnableDmaTx(u8 *pBuffer, u16 lenth)
{
	Wight_EnableTx();
	usart_flag_clear(USART1, USART_FLAG_TC);
	dma_channel_disable(DMA0, DMA_CH6);
	dma_flag_clear(DMA0, DMA_CH6, DMA_FLAG_FTF);
	dma_memory_address_config(DMA0, DMA_CH6, (uint32_t)pBuffer);
	dma_transfer_number_config(DMA0, DMA_CH6, lenth);
	dma_channel_enable(DMA0, DMA_CH6);
	//while (usart_flag_get(USART1, USART_FLAG_TC)!=RESET);
	//Wight_EnableRx();
}


void Write_Delayms(u32 n)             //系统延时n毫秒
{
    n *= 0x6000;
    n++;
    while(n--);
}


void Wight_WriteByte(u8 da)
{
	usart_data_transmit(USART1, da);
    while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));
}

void Wight_WriteBuffer(u8 *pBuffer, u16 len)
{
	u32 i = 0;
	
    Wight_EnableTx();
	Write_Delayms(1);
    for(i = 0; i < len; i++)
    {
        Wight_WriteByte(pBuffer[i]);
    }
	Write_Delayms(4);
    Wight_EnableRx();
	
}