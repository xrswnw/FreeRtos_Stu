#include "Uart2_HL.h"

const PORT_INF USART_PORT_TX = {GPIOB, GPIO_PIN_10};
const PORT_INF USART_PORT_RX = {GPIOB, GPIO_PIN_11};
const PORT_INF USART_PORT_CTRL = {GPIOC, GPIO_PIN_5};

void Usart_Delayms(uint32_t n)             //系统延时n毫秒
{
    n *= 0x6000;
    n++;
    while(n--);
}


void Usart_InitInterface(u32 bud)
{
	gpio_init(USART_PORT_TX.port, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, USART_PORT_TX.pin);
	gpio_init(USART_PORT_RX.port, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, USART_PORT_RX.pin);
	gpio_init(USART_PORT_CTRL.port, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, USART_PORT_CTRL.pin);
		
	Usart_PoeEnable();
    usart_deinit(USART_PORT);
    usart_baudrate_set(USART_PORT, bud);
    usart_word_length_set(USART_PORT, USART_WL_8BIT);
    usart_stop_bit_set(USART_PORT, USART_STB_1BIT);
    usart_parity_config(USART_PORT, USART_PM_NONE);
    usart_hardware_flow_rts_config(USART_PORT, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART_PORT, USART_CTS_DISABLE);
    usart_receive_config(USART_PORT, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART_PORT, USART_TRANSMIT_ENABLE);
   
	usart_dma_receive_config(USART_PORT, USART_RECEIVE_DMA_ENABLE);
    usart_dma_transmit_config(USART_PORT, USART_TRANSMIT_DMA_ENABLE);
 	usart_enable(USART_PORT);
	
	nvic_priority_group_set(NVIC_PRIGROUP_PRE3_SUB1);
	nvic_irq_enable(USART_PORT_IQRN, 1, 0);
	usart_interrupt_enable(USART_PORT, USART_INT_IDLE);
}

void Usart_DmaTxInit()
{
	dma_parameter_struct dma_init_struct;

    dma_deinit(USART_DMA_CHANNEL, USART_DMA_CHANNEL_TX);
    dma_struct_para_init(&dma_init_struct);

    dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_addr = NULL;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = NULL;
    dma_init_struct.periph_addr = (u32)&USART_DATA(USART_PORT);
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(USART_DMA_CHANNEL, USART_DMA_CHANNEL_TX, &dma_init_struct);
    
    dma_circulation_disable(USART_DMA_CHANNEL, USART_DMA_CHANNEL_TX);
    dma_memory_to_memory_disable(USART_DMA_CHANNEL, USART_DMA_CHANNEL_TX);
    dma_channel_disable(USART_DMA_CHANNEL, USART_DMA_CHANNEL_TX);
	
}

void Usart_DmaRxInit(u8 *pFrame, u16 len)
{
	dma_parameter_struct dma_init_struct;
	
    dma_deinit(USART_DMA_CHANNEL, USART_DMA_CHANNEL_RX);
    dma_struct_para_init(&dma_init_struct);

    dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
    dma_init_struct.memory_addr = (uint32_t)pFrame;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;  
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = len; 
    dma_init_struct.periph_addr = (u32)&USART_DATA(USART_PORT);
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.memory_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(USART_DMA_CHANNEL, USART_DMA_CHANNEL_RX, &dma_init_struct);
	
	dma_circulation_disable(USART_DMA_CHANNEL, USART_DMA_CHANNEL_RX);
    dma_memory_to_memory_disable(USART_DMA_CHANNEL, USART_DMA_CHANNEL_RX);
    dma_interrupt_enable(USART_DMA_CHANNEL, USART_DMA_CHANNEL_RX, DMA_INT_FTF);						//打开中断
    dma_channel_enable(USART_DMA_CHANNEL, USART_DMA_CHANNEL_RX);
		
}

void Usart_EnableTx(u8 *pBuffer, u16 lenth)
{
	usart_flag_clear(USART_PORT, USART_FLAG_TC);
	dma_channel_disable(USART_DMA_CHANNEL, USART_DMA_CHANNEL_TX);
	dma_flag_clear(USART_DMA_CHANNEL, USART_DMA_CHANNEL_TX, DMA_FLAG_FTF);
	dma_memory_address_config(USART_DMA_CHANNEL, USART_DMA_CHANNEL_TX, (uint32_t)pBuffer);
	dma_transfer_number_config(USART_DMA_CHANNEL, USART_DMA_CHANNEL_TX, lenth);
	dma_channel_enable(USART_DMA_CHANNEL, USART_DMA_CHANNEL_TX);
	while (usart_flag_get(USART_PORT, USART_FLAG_TC)!=RESET);
}