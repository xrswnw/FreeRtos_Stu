#include "AnyID_CS1110_Wight_HL.h"


#define GPB_INT_CHANNEL           USART1_IRQn
const PORT_INF GPB_PORT_TX      = {GPIOA, GPIO_PIN_2};
const PORT_INF GPB_PORT_RX      = {GPIOA, GPIO_PIN_3};
const PORT_INF GPB_PORT_CTRL    = {GPIOA, GPIO_PIN_1};

void GPB_InitInterface(u32 baudrate)
{
/*
	// 配置TX为推挽复用模式
	gpio_init(GPB_PORT_TX.port, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPB_PORT_TX.pin);
	// 配置RX为浮空输入模式
	gpio_init(GPB_PORT_RX.port, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPB_PORT_RX.pin);
	gpio_init(GPB_PORT_CTRL.port, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPB_PORT_CTRL.pin);

	GPB_EnableRx();
	// 配置串口的工作参数
	usart_deinit(GPB_PORT);
	usart_baudrate_set(GPB_PORT, baudrate);                        // 波特率
	usart_word_length_set(GPB_PORT, USART_WL_8BIT);               // 帧数据字长
	usart_stop_bit_set(GPB_PORT, USART_STB_1BIT);                 // 停止位
	usart_parity_config(GPB_PORT, USART_PM_NONE);                 // 奇偶校验位
	usart_hardware_flow_rts_config(GPB_PORT, USART_RTS_DISABLE);  // 硬件流控制RTS
	usart_hardware_flow_cts_config(GPB_PORT, USART_CTS_DISABLE);  // 硬件流控制CTS
	usart_receive_config(GPB_PORT, USART_RECEIVE_ENABLE);         // 使能接收
	usart_transmit_config(GPB_PORT, USART_TRANSMIT_ENABLE);       // 使能发送

	usart_interrupt_enable(GPB_PORT, USART_INT_RBNE);
	nvic_irq_enable(GPB_INT_CHANNEL, 0, 0);
	usart_enable(GPB_PORT);   
	*/
	
	//使能中断，配置中断优先级
	nvic_irq_enable(USART1_IRQn, 0, 2);//中断源   抢占优先级，  响应优先级
	//配置GPIO
	rcu_periph_clock_enable(RCU_GPIOA);
	//使能UARTx时钟
	rcu_periph_clock_enable(RCU_USART1);
	//UART1 TX(PA2) Init
	gpio_init(GPB_PORT_TX.port, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPB_PORT_TX.pin);
	//UART1 RX(PA3) Init
	gpio_init(GPB_PORT_RX.port, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPB_PORT_RX.pin);
 
	//Reset UARTx
	usart_deinit(USART1);
	usart_baudrate_set(USART1,115200);//配置波特率
	usart_parity_config(USART1,USART_PM_NONE);//配置校验
	usart_word_length_set(USART1,USART_WL_8BIT);//配置数据长度
	usart_stop_bit_set(USART1,USART_STB_1BIT);//配置停止位
	usart_hardware_flow_rts_config(USART1,USART_RTS_DISABLE);//RTS Disable
	usart_hardware_flow_cts_config(USART1,USART_CTS_DISABLE);//CTS Disable
	usart_receive_config(USART1,USART_RECEIVE_ENABLE);//使能串口接收功能
	usart_transmit_config(USART1,USART_TRANSMIT_ENABLE);//使能串口发送功能
	usart_enable(USART1);//使能串口
	
	usart_interrupt_enable(USART1, USART_INT_IDLE);//使能RX空闲中断
	usart_interrupt_enable(USART1, USART_INT_RBNE);//使能RX空闲中断
}

void GPB_Delayms(u32 n)
{
    n *= 0x6000;//0x3800;
    n++;
    while(n--);
}


void Gpb_DmaTxInit(u8 *pBuffer, u16 len)
{
	/*
	dma_parameter_struct dma_init_struct = {0};
	
    dma_deinit(GPB_UART_DMA_CHANNEL, GPB_UART_DMA_TX_CHANNEL);
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_addr = NULL;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = 0;
    dma_init_struct.periph_addr = USART_DATA(GPB_PORT);
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(GPB_UART_DMA_CHANNEL, GPB_UART_DMA_TX_CHANNEL, &dma_init_struct);
    
    dma_circulation_disable(GPB_UART_DMA_CHANNEL, GPB_UART_DMA_TX_CHANNEL);
    dma_memory_to_memory_disable(GPB_UART_DMA_CHANNEL, GPB_UART_DMA_TX_CHANNEL);

    usart_dma_transmit_config(USART0, USART_TRANSMIT_DMA_ENABLE);
    
    dma_interrupt_enable(GPB_UART_DMA_CHANNEL, GPB_UART_DMA_TX_CHANNEL, DMA_INT_FTF);
    
    dma_channel_enable(GPB_UART_DMA_CHANNEL, GPB_UART_DMA_TX_CHANNEL);
	nvic_irq_enable(GPB_UART_DMA_TX_CHANNEL_IRQ, 0, 0);
*/
	dma_parameter_struct dma_init_struct;
    /* enable DMA0 */

	rcu_periph_clock_enable(RCU_DMA0);

    /* deinitialize DMA channel6(USART1 tx) */
    dma_deinit(DMA0, DMA_CH6);//Tx对应DMA0 通道6
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_addr = (uint32_t)pBuffer;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.number = sizeof(pBuffer);
    dma_init_struct.periph_addr = (uint32_t)(&USART_DATA(USART1));
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
	
    dma_init(DMA0, DMA_CH6, &dma_init_struct);
	
    dma_interrupt_enable(DMA0, DMA_CH6, DMA_INT_FTF);
    //configure DMA mode 
    dma_circulation_disable(DMA0, DMA_CH6);


	
}

void Gpb_DmaRxInit(u8 *pBuffer, u16 len)
{
	/*
	dma_parameter_struct dma_init_struct = {0};
	
    dma_deinit(GPB_UART_DMA_CHANNEL, GPB_UART_DMA_RX_CHANNEL);
    dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;
    dma_init_struct.memory_addr = (uint32_t)pBuffer;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number = len;
    dma_init_struct.periph_addr = USART_DATA(GPB_PORT);
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(GPB_UART_DMA_CHANNEL, GPB_UART_DMA_RX_CHANNEL, &dma_init_struct);
    
    dma_circulation_disable(GPB_UART_DMA_CHANNEL, GPB_UART_DMA_RX_CHANNEL);
    dma_memory_to_memory_disable(GPB_UART_DMA_CHANNEL, GPB_UART_DMA_RX_CHANNEL);

    usart_dma_receive_config(USART0, USART_TRANSMIT_DMA_ENABLE);
    
    dma_interrupt_enable(GPB_UART_DMA_CHANNEL, GPB_UART_DMA_RX_CHANNEL, DMA_INT_FTF);
    
    dma_channel_enable(GPB_UART_DMA_CHANNEL, GPB_UART_DMA_RX_CHANNEL);
	nvic_irq_enable(GPB_UART_DMA_RX_CHANNEL_IRQ, 0, 0);
	*/
	
	dma_parameter_struct dma_parameter;
    /* enable DMA0 */
	rcu_periph_clock_enable(RCU_DMA0);


    /* 接收 dm0 channel5(USART1 rx) */
    dma_deinit(DMA0, DMA_CH5);//rx对应DMA0通道5
    dma_parameter.direction = DMA_PERIPHERAL_TO_MEMORY;
    dma_parameter.periph_addr = (uint32_t)(&USART_DATA(USART1));
    dma_parameter.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_parameter.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_parameter.memory_addr = (uint32_t)pBuffer;
    dma_parameter.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_parameter.number = sizeof(pBuffer);
    dma_parameter.priority = DMA_PRIORITY_ULTRA_HIGH;

    dma_init(DMA0, DMA_CH5, &dma_parameter);

    /* configure DMA mode */
	
    dma_interrupt_enable(DMA0, DMA_CH5, DMA_INT_FTF);

    dma_channel_enable(DMA0, DMA_CH5);



}

void Gpb_EnableTxDma(u8 *pFrame, u16 lenth)
{
	/*
	dma_channel_disable(GPB_UART_DMA_CHANNEL, GPB_UART_DMA_TX_CHANNEL);
	
	dma_memory_address_config(GPB_UART_DMA_CHANNEL, GPB_UART_DMA_TX_CHANNEL, (uint32_t)pFrame);//设置要发送数据的内存地址
	dma_transfer_number_config(GPB_UART_DMA_CHANNEL, GPB_UART_DMA_TX_CHANNEL, lenth);//一共发多少个数据
	
	dma_channel_enable(GPB_UART_DMA_CHANNEL, GPB_UART_DMA_TX_CHANNEL);
  	usart_dma_transmit_config(GPB_PORT, USART_TRANSMIT_DMA_ENABLE);//使能串口DMA发送
	while(RESET == dma_flag_get(GPB_UART_DMA_CHANNEL, GPB_UART_DMA_TX_CHANNEL, DMA_FLAG_FTF));
	*/
	usart_flag_clear(USART1, USART_FLAG_TC);

	dma_channel_disable(DMA0, DMA_CH6);
	dma_flag_clear(DMA0, DMA_CH6, DMA_FLAG_FTF);
	dma_memory_address_config(DMA0, DMA_CH6, (uint32_t)pFrame);
	dma_transfer_number_config(DMA0, DMA_CH6, lenth);
	dma_channel_enable(DMA0, DMA_CH6);
	while (usart_flag_get(USART1, USART_FLAG_TC)!=RESET);

}

int32_t uart_dma_stop(void)
{
    usart_disable(USART1);
    return 0;
}
