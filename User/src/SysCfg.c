#include "SysCfg.h"

#if SYS_ENABLE_WDT
    #define SYS_ENABLE_TEST         0
#else
    #define SYS_ENABLE_TEST         1
#endif

void Sys_Delayms(int n)
{
    //72MHZ
    n *= 0x6000;
    n++;
    while(n--);
}


void rcu_config(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOD);
    /* enable ADC0 clock */
    rcu_periph_clock_enable(RCU_UART4);
    /* enable ADC1 clock */
    rcu_periph_clock_enable(RCU_ADC1);  
    /* enable DMA0 clock */
    rcu_periph_clock_enable(RCU_DMA0);  
    /* enable timer1 clock */
    rcu_periph_clock_enable(RCU_TIMER1);
    /* config ADC clock */
    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV4);
}


/*

void Sys_CfgNVIC(void)
{
    //NVIC_InitTypeDef NVIC_InitStructure;
#ifdef  VECT_TAB_RAM
    //Set the Vector Table base location at 0x20000000
    NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  //VECT_TAB_FLASH
    //Set the Vector Table base location at 0x08000000
#ifdef _ANYID_BOOT_GD32_
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x4000);
#else
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);
#endif
#endif

    //Configure the Priority Group to 2 bits
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}
*/
const PORT_INF SYS_RUN_LED = {GPIOB, GPIO_PIN_1};

void Sys_CtrlIOInit(void)
{
	gpio_init(SYS_RUN_LED.port, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, SYS_RUN_LED.pin);

}
void Sys_Init(void)
{
	rcu_config();
	Sys_CtrlIOInit();
	Uart2_HLInit(115200);
}	

void SysLedTask(void *pParaments)
{
	while(1)
	{
		Sys_LedOpen();
		Sys_Delayms(1000);
		Sys_LedOff();
		Sys_Delayms(1000);
		Uart_SendByte(0x55);
	}
}

