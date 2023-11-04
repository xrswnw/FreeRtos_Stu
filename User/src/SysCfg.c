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
    rcu_periph_clock_enable(RCU_UART3);
    /* enable ADC1 clock */
    rcu_periph_clock_enable(RCU_ADC1);  
    /* enable DMA0 clock */
    //rcu_periph_clock_enable(RCU_DMA0);  
    /* enable timer1 clock */
    rcu_periph_clock_enable(RCU_TIMER1);
    /* config ADC clock */
	rcu_periph_clock_enable( RCU_AF);
    //rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV4);
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
const PORT_INF IO_LED_CTL = {GPIOB, GPIO_PIN_3};

void Sys_CtrlIOInit(void)
{
	gpio_init(SYS_RUN_LED.port, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, SYS_RUN_LED.pin);
	gpio_init(IO_LED_CTL.port, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, IO_LED_CTL.pin);


}
void Sys_Init(void)
{
	rcu_config();
	Sys_CtrlIOInit();
	Uart_Init(RS485_BAUD_RATE);
	rcu_clock_freq_get(CK_SYS);
}

const TickType_t xDelay500ms = pdMS_TO_TICKS( 500UL );
const TickType_t xDelay50ms = pdMS_TO_TICKS( 50UL );




TaskHandle_t handSysLedTask1;
void SysLedTask1(void *pParaments)
{
	static uint8_t i = 0;

	UBaseType_t uxPriority;
	
	
	while(1)
	{
		uxPriority = uxTaskPriorityGet( NULL );
		Sys_LedOpen();
		//Uart_SendByte(i++);
		i++;
		if(i > (0xFF - 1))
		{
			vTaskDelete(NULL);
		}
		else if(i == 0xF0)
		{
			//vTaskSuspend(NULL);
		}
		
		if(i % 2 == 0)
		{
			Sys_RunLedOpen();
		}
		else
		{
			Sys_RunLedOff();
		}
		//vTaskDelay(xDelay50ms);
		vTaskPrioritySet(handSysLedTask2, (uxPriority + 1));
	}
}
TaskHandle_t handSysLedTask2;

	TickType_t xLastWakeTime;
void SysLedTask2(void *pParaments)
{
	TickType_t uxPriority;
	static uint8_t i = 1;
	uxPriority = uxTaskPriorityGet( NULL );


	
	
	xLastWakeTime = xTaskGetTickCount();//获取当前时间
	for(;;)
	{
		Sys_LedOff();
		i++;
		//Uart_SendByte(i+=2);
		//vApplicationIdleHook();
		if(i % 2 == 0)
		{
			Sys_RunLedOff();
		}
		else
		{
			Sys_RunLedOpen();
		}
		vTaskDelayUntil(&xLastWakeTime, xDelay50ms);					//xLastWakeTime延时完成后会更新到延时时间
		//vTaskPrioritySet(handSysLedTask2, (uxTaskPriorityGet( handSysLedTask1 ) - 1));;
	}
}


QueueHandle_t xQueue1;

uint8_t i = 0;
QUENE_INFO g_sTempQueneInfo = {0};


TaskHandle_t sysQueneTaskHandel1 ;
TaskHandle_t sysQueneTaskHandel2 ;


void SysWriteQueueTask(void *pParments)
{
	xLastWakeTime = xTaskGetTickCount();//获取当前时间
	while(1)
	{
		g_sTempQueneInfo.num = uxQueueMessagesWaiting(xQueue1);		//获取队列当前长度
		uint8_t j = 0;		
		for(uint8_t i = 0; i < ITEM_SIZE; i++)
		{
			g_sTempQueneInfo.buffer[i] = j++;
		}
		xQueueSendToBack(xQueue1, &g_sTempQueneInfo, xDelay50ms);
		if(g_sTempQueneInfo.num < QUEUE_LENGTH)
		{
			Uart_SendBuf(&g_sTempQueneInfo.num, 1);
			Uart_SendBuf(g_sTempQueneInfo.buffer, ITEM_SIZE);
			vTaskSuspend(sysQueneTaskHandel2);
			vTaskDelayUntil(&xLastWakeTime, xDelay500ms * 2);
			
		}
		else
		{
			/*	典型，可做时序参考
			vTaskResume(sysQueneTaskHandel2);	//释放 读队列函数
			vTaskPrioritySet(sysQueneTaskHandel2, (uxTaskPriorityGet( sysQueneTaskHandel2 ) + 1));	//读队列函数优先级加高
			vTaskSuspend(NULL);				//suspend当前函数，后续代码不执行
			*/
			vTaskResume(sysQueneTaskHandel2);	//释放 读队列函数
			vTaskPrioritySet(sysQueneTaskHandel2, 1);	//读队列函数优先级加高
			vTaskSuspend(NULL);
			//vTaskSuspend(NULL);				//suspend当前函数，后续代码不执行
		}
	}
}

void SysReadQueueTask(void *pParments)
{
	UBaseType_t num = 0;
	xLastWakeTime = xTaskGetTickCount();//获取当前时间
	while(1)
	{
		num = uxQueueMessagesWaiting(xQueue1);
		xQueueReceive(xQueue1, &g_sTempQueneInfo, xDelay50ms);
		vTaskDelayUntil(&xLastWakeTime, xDelay500ms);
		if(num > 0)
		{
			Uart_SendBuf((uint8_t *)num, 1);
			vTaskSuspend(sysQueneTaskHandel1);
			Uart_SendBuf(g_sTempQueneInfo.buffer, ITEM_SIZE);
			vTaskDelayUntil(&xLastWakeTime, xDelay500ms * 2);
		}
		else
		{
			/*	典型，可做时序参考
			vTaskResume(sysQueneTaskHandel1);
			vTaskPrioritySet(sysQueneTaskHandel2, (uxTaskPriorityGet( sysQueneTaskHandel2 ) - 1));
			vTaskSuspend(NULL);			//suspend当前函数，后续代码不执行
			*/
			
			vTaskResume(sysQueneTaskHandel1);
			vTaskSuspend(NULL);
			//vTaskPrioritySet(sysQueneTaskHandel2, (uxTaskPriorityGet( sysQueneTaskHandel2 ) - 1));
			//vTaskSuspend(NULL);			//suspend当前函数，后续代码不执行

		}
	}

}


/* 二进制信号量句柄 */
SemaphoreHandle_t binarySemaphore;


void SysSemaphoreTask(void *pParments)
{
	



}