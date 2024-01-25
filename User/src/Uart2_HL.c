#include "Uart2_HL.h"

//GD32下为UART3

const PORT_INF UART2_PORT_TX = {GPIOB, GPIO_PIN_10};
const PORT_INF UART2_PORT_RX = {GPIOB, GPIO_PIN_11};
const PORT_INF UART2_PORT_CTRL = {GPIOC, GPIO_PIN_5};



void Uart2_Delayms(uint32_t n)             //系统延时n毫秒
{
    n *= 0x6000;
    n++;
    while(n--);
}


void Uart2_SendByte(uint8_t ch)
{
	usart_data_transmit(UART2_PORT, ch);
    while(RESET == usart_flag_get(UART2_PORT, USART_FLAG_TBE));
}

void Uart2_SendBuf(uint8_t *pFrame, uint32_t len)
{
	uint32_t i = 0;

	for(i = 0; i < len; i++)
	{
		Uart2_SendByte(pFrame[i]);
	}

}

//------------

uint8_t rxbuffer[84];
uint8_t txbuffer[84] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};

#define BAUDRATE	115200U 
#define ARRAYNUM(arr_name)     (uint32_t)(sizeof(arr_name) / sizeof(*(arr_name)))
	 
void bsp_usart_init(void);
void usart2_dma_send(uint8_t *buffer,uint16_t len);

#define USART0_DATA_ADDRESS    ((uint32_t)&USART_DATA(USART0))
#define USART2_DATA_ADDRESS    ((uint32_t)&USART_DATA(USART2))
											 
static void usart_config(uint32_t baudval)
{
	//USART0
	rcu_periph_clock_enable(RCU_GPIOA);	//enable GPIO clock, PA9/PA10
	rcu_periph_clock_enable(RCU_AF);
    rcu_periph_clock_enable(RCU_USART0);	//enable USART clock
	
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);	//PA9--TX0
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);	//PA10--RX0

    usart_deinit(USART0);
    usart_baudrate_set(USART0, baudval);
    usart_word_length_set(USART0, USART_WL_8BIT);
    usart_stop_bit_set(USART0, USART_STB_1BIT);
    usart_parity_config(USART0, USART_PM_NONE);
    usart_hardware_flow_rts_config(USART0, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART0, USART_CTS_DISABLE);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_enable(USART0);
	
    //USART2
    rcu_periph_clock_enable(RCU_GPIOB);	//enable GPIO clock, PB10/PA11
    rcu_periph_clock_enable(RCU_AF);
    rcu_periph_clock_enable(RCU_USART2);	//enable USART2 clock
		
	gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);//USART2_TX
	gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_11);//USART2_RX
		
    usart_deinit(USART2);
    usart_baudrate_set(USART2, baudval);
    usart_word_length_set(USART2, USART_WL_8BIT);
    usart_stop_bit_set(USART2, USART_STB_1BIT);
    usart_parity_config(USART2, USART_PM_NONE);
    usart_hardware_flow_rts_config(USART2, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART2, USART_CTS_DISABLE);
    usart_receive_config(USART2, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART2, USART_TRANSMIT_ENABLE);
    usart_enable(USART2);
}

static void usart_dma_config(void)
{
    dma_parameter_struct dma_init_struct;
    /* enable DMA0 clock */
    rcu_periph_clock_enable(RCU_DMA0);
    
    /* deinitialize DMA channel1(USART2 tx) */
    dma_deinit(DMA0, DMA_CH1);
    dma_struct_para_init(&dma_init_struct);

    
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_addr = NULL;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = ARRAYNUM(txbuffer);
    dma_init_struct.periph_addr = USART2_DATA_ADDRESS;
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(DMA0, DMA_CH1, &dma_init_struct);
    
    /* deinitialize DMA channel2 (USART2 rx) */
    dma_deinit(DMA0, DMA_CH2);
    dma_struct_para_init(&dma_init_struct);

    dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
    dma_init_struct.memory_addr = (uint32_t)rxbuffer;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;  
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = ARRAYNUM(rxbuffer); 
    dma_init_struct.periph_addr = USART2_DATA_ADDRESS;
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.memory_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(DMA0, DMA_CH2, &dma_init_struct);
  
    /* configure DMA mode */
    dma_circulation_disable(DMA0, DMA_CH1);
    dma_memory_to_memory_disable(DMA0, DMA_CH1);
//    dma_circulation_enable(DMA0, DMA_CH2);      // 使用接收空闲中断，是否循环没有关系
    dma_circulation_disable(DMA0, DMA_CH2);
    dma_memory_to_memory_disable(DMA0, DMA_CH2);
    
    /* USART DMA0 enable for reception */
    usart_dma_receive_config(USART2, USART_RECEIVE_DMA_ENABLE);
    /* enable DMA0 channel2 transfer complete interrupt */
    dma_interrupt_enable(DMA0, DMA_CH2, DMA_INT_FTF);
    /* enable DMA0 channel2 */
    dma_channel_enable(DMA0, DMA_CH2);
		
    /* USART DMA0 enable for transmission */
    usart_dma_transmit_config(USART2, USART_TRANSMIT_DMA_ENABLE);
    /* enable DMA0 channel1 transfer complete interrupt */
//    dma_interrupt_enable(DMA0, DMA_CH3, DMA_INT_FTF);
//    /* disable DMA0 channel1 */
    dma_channel_disable(DMA0, DMA_CH1);
}

static void nvic_config(void)
{
		nvic_priority_group_set(NVIC_PRIGROUP_PRE3_SUB1);
//		nvic_irq_enable(DMA0_Channel1_IRQn, 0, 0);
		nvic_irq_enable(USART2_IRQn,1,0);
		usart_interrupt_enable(USART2, USART_INT_IDLE);
//		nvic_irq_enable(DMA0_Channel2_IRQn, 0, 1);
}

void bsp_usart_init() 
{   
	  memset(rxbuffer,0xff,sizeof(rxbuffer));
	  usart_config(BAUDRATE);
	  nvic_config();
	  usart_dma_config();		
}

void usart2_dma_send(uint8_t *buffer,uint16_t len)
{
	usart_flag_clear(USART2, USART_FLAG_TC);

	dma_channel_disable(DMA0, DMA_CH1);
	dma_flag_clear(DMA0, DMA_CH1, DMA_FLAG_FTF);
	dma_memory_address_config(DMA0, DMA_CH1, (uint32_t)buffer);
	dma_transfer_number_config(DMA0, DMA_CH1, len);
	dma_channel_enable(DMA0, DMA_CH1);
	while (usart_flag_get(USART2, USART_FLAG_TC)!=RESET);
}


void Uart2_HLInit(uint32_t baud)
{
	usart_config(115200);
	usart_dma_config();
	nvic_config();
}


//------

const PORT_INF USART_PORT_TX = {GPIOB, GPIO_PIN_10};
const PORT_INF USART_PORT_RX = {GPIOB, GPIO_PIN_11};
const PORT_INF USART_PORT_CTRL = {GPIOC, GPIO_PIN_5};


void Uart_InitInterface(u32 bud)
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

void Uart_DmaTxInit()
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

void Uart_DmaRxInit(u8 *pFrame, u16 len)
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

void Uart_EnableTx(u8 *pBuffer, u16 lenth)
{
	usart_flag_clear(USART2, USART_FLAG_TC);
	dma_channel_disable(USART_DMA_CHANNEL, USART_DMA_CHANNEL_TX);
	dma_flag_clear(USART_DMA_CHANNEL, USART_DMA_CHANNEL_TX, DMA_FLAG_FTF);
	dma_memory_address_config(USART_DMA_CHANNEL, USART_DMA_CHANNEL_TX, (uint32_t)pBuffer);
	dma_transfer_number_config(USART_DMA_CHANNEL, USART_DMA_CHANNEL_TX, lenth);
	dma_channel_enable(USART_DMA_CHANNEL, USART_DMA_CHANNEL_TX);
	while (usart_flag_get(USART2, USART_FLAG_TC)!=RESET);
}