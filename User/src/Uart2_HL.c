#include "Uart2_HL.h"

const PORT_INF RS485_PORT_TX = {GPIOC, GPIO_PIN_10};
const PORT_INF RS485_PORT_RX = {GPIOC, GPIO_PIN_11};
const PORT_INF RS485_PORT_CTRL = {GPIOB, GPIO_PIN_8};

void Uart2_HLInit(uint32_t baud)
{
	// 配置TX为推挽复用模式
	gpio_init(RS485_PORT_TX.port, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, RS485_PORT_TX.pin);
	// 配置RX为浮空输入模式
	gpio_init(RS485_PORT_RX.port, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, RS485_PORT_RX.pin);
	gpio_init(RS485_PORT_CTRL.port, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, RS485_PORT_CTRL.pin);
		
	// 配置串口的工作参数
	usart_deinit(RS485_PORT);
	usart_baudrate_set(RS485_PORT, baud);                        // 波特率
	usart_word_length_set(RS485_PORT, USART_WL_8BIT);               // 帧数据字长
	usart_stop_bit_set(RS485_PORT, USART_STB_1BIT);                 // 停止位
	usart_parity_config(RS485_PORT, USART_PM_NONE);                 // 奇偶校验位
	usart_hardware_flow_rts_config(RS485_PORT, USART_RTS_DISABLE);  // 硬件流控制RTS
	usart_hardware_flow_cts_config(RS485_PORT, USART_CTS_DISABLE);  // 硬件流控制CTS
	usart_receive_config(RS485_PORT, USART_RECEIVE_ENABLE);         // 使能接收
	usart_transmit_config(RS485_PORT, USART_TRANSMIT_ENABLE);       // 使能发送
	usart_enable(RS485_PORT);                                       // 使能串口  
}

void R485_Delayms(uint32_t n)             //系统延时n毫秒
{
    n *= 0x6000;
    n++;
    while(n--);
}


void Uart_SendByte(uint8_t ch)
{
	usart_data_transmit(RS485_PORT, (uint8_t)ch);
    while(RESET == usart_flag_get(RS485_PORT, USART_FLAG_TBE));
}

void Uart_SendBuf(uint8_t *pFrame, uint32_t len)
{
	uint32_t i = 0;
	
	R485_EnableTx();
	R485_Delayms(1);
	for(i = 0; i <= len; i++)
	{
		Uart_SendByte(pFrame[i]);
	}
	R485_Delayms(1);
	R485_EnableRx();
}