#include "AnyID_CS1110_Wight_HL.h"


#define GPB_INT_CHANNEL           USART1_IRQn
const PORT_INF GPB_PORT_TX      = {GPIOA, GPIO_PIN_2};
const PORT_INF GPB_PORT_RX      = {GPIOA, GPIO_PIN_3};
const PORT_INF GPB_PORT_CTRL    = {GPIOA, GPIO_PIN_1};

void GPB_InitInterface(u32 baudrate)
{
/*
	// ����TXΪ���츴��ģʽ
	gpio_init(GPB_PORT_TX.port, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPB_PORT_TX.pin);
	// ����RXΪ��������ģʽ
	gpio_init(GPB_PORT_RX.port, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPB_PORT_RX.pin);
	gpio_init(GPB_PORT_CTRL.port, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPB_PORT_CTRL.pin);

	GPB_EnableRx();
	// ���ô��ڵĹ�������
	usart_deinit(GPB_PORT);
	usart_baudrate_set(GPB_PORT, baudrate);                        // ������
	usart_word_length_set(GPB_PORT, USART_WL_8BIT);               // ֡�����ֳ�
	usart_stop_bit_set(GPB_PORT, USART_STB_1BIT);                 // ֹͣλ
	usart_parity_config(GPB_PORT, USART_PM_NONE);                 // ��żУ��λ
	usart_hardware_flow_rts_config(GPB_PORT, USART_RTS_DISABLE);  // Ӳ��������RTS
	usart_hardware_flow_cts_config(GPB_PORT, USART_CTS_DISABLE);  // Ӳ��������CTS
	usart_receive_config(GPB_PORT, USART_RECEIVE_ENABLE);         // ʹ�ܽ���
	usart_transmit_config(GPB_PORT, USART_TRANSMIT_ENABLE);       // ʹ�ܷ���

	usart_interrupt_enable(GPB_PORT, USART_INT_RBNE);
	nvic_irq_enable(GPB_INT_CHANNEL, 0, 0);
	usart_enable(GPB_PORT);   
	*/
	
	//ʹ���жϣ������ж����ȼ�
	nvic_irq_enable(USART1_IRQn, 0, 2);//�ж�Դ   ��ռ���ȼ���  ��Ӧ���ȼ�
	//����GPIO
	rcu_periph_clock_enable(RCU_GPIOA);
	//ʹ��UARTxʱ��
	rcu_periph_clock_enable(RCU_USART1);
	//UART1 TX(PA2) Init
	gpio_init(GPB_PORT_TX.port, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPB_PORT_TX.pin);
	//UART1 RX(PA3) Init
	gpio_init(GPB_PORT_RX.port, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPB_PORT_RX.pin);
 
	//Reset UARTx
	usart_deinit(USART1);
	usart_baudrate_set(USART1,115200);//���ò�����
	usart_parity_config(USART1,USART_PM_NONE);//����У��
	usart_word_length_set(USART1,USART_WL_8BIT);//�������ݳ���
	usart_stop_bit_set(USART1,USART_STB_1BIT);//����ֹͣλ
	usart_hardware_flow_rts_config(USART1,USART_RTS_DISABLE);//RTS Disable
	usart_hardware_flow_cts_config(USART1,USART_CTS_DISABLE);//CTS Disable
	usart_receive_config(USART1,USART_RECEIVE_ENABLE);//ʹ�ܴ��ڽ��չ���
	usart_transmit_config(USART1,USART_TRANSMIT_ENABLE);//ʹ�ܴ��ڷ��͹���
	usart_enable(USART1);//ʹ�ܴ���
	
	usart_interrupt_enable(USART1, USART_INT_IDLE);//ʹ��RX�����ж�
	usart_interrupt_enable(USART1, USART_INT_RBNE);//ʹ��RX�����ж�
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
    dma_deinit(DMA0, DMA_CH6);//Tx��ӦDMA0 ͨ��6
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


    /* ���� dm0 channel5(USART1 rx) */
    dma_deinit(DMA0, DMA_CH5);//rx��ӦDMA0ͨ��5
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
	
	dma_memory_address_config(GPB_UART_DMA_CHANNEL, GPB_UART_DMA_TX_CHANNEL, (uint32_t)pFrame);//����Ҫ�������ݵ��ڴ��ַ
	dma_transfer_number_config(GPB_UART_DMA_CHANNEL, GPB_UART_DMA_TX_CHANNEL, lenth);//һ�������ٸ�����
	
	dma_channel_enable(GPB_UART_DMA_CHANNEL, GPB_UART_DMA_TX_CHANNEL);
  	usart_dma_transmit_config(GPB_PORT, USART_TRANSMIT_DMA_ENABLE);//ʹ�ܴ���DMA����
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
