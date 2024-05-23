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

    rcu_periph_clock_enable(RCU_USART2);
	
	rcu_periph_clock_enable(RCU_USART1);
	
    rcu_periph_clock_enable(RCU_ADC1);  

    rcu_periph_clock_enable(RCU_DMA0);  

    rcu_periph_clock_enable(RCU_TIMER1);

	rcu_periph_clock_enable( RCU_AF);
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
const PORT_INF SYS_LED_RED   = {GPIOC, GPIO_PIN_0};
const PORT_INF SYS_LED_BLUE  = {GPIOC, GPIO_PIN_1};
const PORT_INF SYS_LED_GREEN = {GPIOC, GPIO_PIN_2};
void Sys_CtrlIOInit(void)
{
	gpio_init(SYS_RUN_LED.port, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, SYS_RUN_LED.pin);
	gpio_init(IO_LED_CTL.port, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, IO_LED_CTL.pin);
	gpio_init(SYS_LED_RED.port, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, SYS_LED_RED.pin);
	gpio_init(SYS_LED_BLUE.port, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, SYS_LED_BLUE.pin);
	gpio_init(SYS_LED_GREEN.port, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, SYS_LED_GREEN.pin);
}

u32 g_nClock = 0;
void Sys_Init(void)
{
	rcu_config();
	Sys_CtrlIOInit();
	Usart_Init(USART_BAUD_RATE);
	Wight_Init();
	
	//systick_config();
	g_nClock = rcu_clock_freq_get(CK_SYS);
	g_nClock = rcu_clock_freq_get(CK_AHB);
	g_nClock = rcu_clock_freq_get(CK_APB1);
	g_nClock = rcu_clock_freq_get(CK_APB2);
	
	
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
QueueHandle_t xQueue2;
uint8_t i = 0;
QUENE_INFO g_sTempQueneInfo = {0};


TaskHandle_t sysQueneTaskHandel1 ;
TaskHandle_t sysQueneTaskHandel2 ;

TaskHandle_t sysQueneWriteTask;
TaskHandle_t sysQueneReadTask;
UBaseType_t g_nQueneNum = 0;
void SysWriteQueueTask(void *pParments)
{
	xLastWakeTime = xTaskGetTickCount();//获取当前时间
	while(1)
	{
		g_sTempQueneInfo.num = uxQueueMessagesWaiting(xQueue1);		//获取队列当前长度
		
		static uint8_t j = 0;		
		for(uint8_t i = 0; i < ITEM_SIZE; i++)
		{
			g_sTempQueneInfo.buffer[i] = j++;
		}
		xQueueSendToBack(xQueue1, &g_sTempQueneInfo, xDelay50ms);					//先像队列写入数据，写满后再读出，测试队列量
		g_nQueneNum = uxQueueMessagesWaiting(xQueue1);
		if(g_sTempQueneInfo.num < QUEUE_LENGTH)
		{
			//Uart2_SendBuf(&g_sTempQueneInfo.num, 1);
			//Uart2_SendBuf(g_sTempQueneInfo.buffer, ITEM_SIZE);
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
	xLastWakeTime = xTaskGetTickCount();//获取当前时间
	while(1)
	{
		g_nQueneNum = uxQueueMessagesWaiting(xQueue1);
		xQueueReceive(xQueue1, &g_sTempQueneInfo, xDelay50ms);
		vTaskDelayUntil(&xLastWakeTime, xDelay500ms);

		if(g_nQueneNum > 0)
		{
			//Uart2_SendBuf((uint8_t *)g_nQueneNum, 1);
			vTaskSuspend(sysQueneTaskHandel1);
			//Uart2_SendBuf(g_sTempQueneInfo.buffer, ITEM_SIZE);
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
			vTaskSuspend(NULL);							//堵塞当前任务，暂停
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











//------------------------------------------------------------------------------------------------//
TaskHandle_t SysTaskUart2 ;
void Sys_Uart2Task()
{
	for(;;)
	{
		if(Uart_IsRcvFrame(g_sUsartRcvFrame))
		{
			u16 crc1 = 0, crc2 = 0;
			u16 txLen = 0;
			memcpy(&g_sUsartRcvTempFrame, &g_sUsartRcvFrame, sizeof(USART_RCVFRAME));
			memset(&g_sUsartRcvFrame, 0, sizeof(USART_RCVFRAME));
			  
			if((g_sUsartRcvTempFrame.length >= UART_FRAME_MIN_LEN) && (g_sUsartRcvTempFrame.length <= UART_BUFFER_MAX_LEN))
			{
				crc1 = Uart_GetFrameCrc(g_sUsartRcvTempFrame.buffer, g_sUsartRcvTempFrame.length);
				crc2 = a_GetCrc(g_sUsartRcvTempFrame.buffer + UART_FRAME_POS_LEN, g_sUsartRcvTempFrame.length - 4);
				if(crc1 == crc2)
				{
					txLen = Reader_ProcessUartFrames(g_sUsartRcvTempFrame.buffer, g_sUsartRcvTempFrame.length);
					if(txLen)
					{
						Usart_EnableTx(g_sDeviceRspFrame.buffer, g_sDeviceRspFrame.len);
						if(g_sDeviceRspFrame.cmd == READER_CMD_RESET)
						{
							vTaskDelay(pdMS_TO_TICKS( 5UL ));
							Sys_SoftReset();
						}
					}
				}
			}
			Uart_ResetFrame(&g_sUsartRcvTempFrame);
		}
	}
}

//系统级别中断任务，每5ms进入判断一次
TaskHandle_t SysIsrTask;
const TickType_t xDelay5ms = pdMS_TO_TICKS( 5UL );
u32 g_nSysTick = 0;
SYS_INFO g_sSysInfo = {0};
void Sys_IsrTask()
{
	for(;;)
	{
		g_nSysTick ++;
		Uart_IncIdleTime(30, g_sUsartRcvFrame);
		
		if((g_nSysTick % 21) == 0)
		{
			//Uart2_SendByte((u8)g_nSysTick);
			/*
			if(g_sSysInfo.ledState == 0)
			{
				g_sSysInfo.ledState |= 1;
				Sys_LedRedOn();Sys_LedGreenOn();Sys_LedBlueOn();
			}
			else
			{
				g_sSysInfo.ledState &= ~1;
				Sys_LedRedOff();Sys_LedGreenOff();Sys_LedBlueOff();
			}*/
		}
		
		vTaskDelay(xDelay5ms);
	}

}
	

/*
#define WIGHT_STAT_IDLE					0x00
#define WIGHT_STAT_DATA					0x01
#define WIGHT_STAT_TO					0x02
#define WIGHT_STAT_END					0x04
#define WIGHT_STAT_RCV					0x08
#define WIGHT_STAT_TX					0x10
#define WIGHT_STAT_WAIT					0x20
#define WIGHT_STAT_STEP					0x40
*/
TaskHandle_t SysWightTask;

u8 g_aGetWigValue[8] = {0xFF, 0x03, 0x00, 0x01, 0x00, 0x02, 0x80, 0x15};
void Sys_WightTask()
{
	for(;;)
	{
		if(Wight_ChkRcv(&g_sWightFrame))
		{
			if(Wight_ChkRcvFrame(&g_sWightFrame))
			{
				vTaskDelay(pdMS_TO_TICKS( 500UL ));
				Wight_ResetFrame(&g_sWightFrame);
				g_sWightFrame.state |= WIGHT_STAT_TX;
			}
		}
		
		if(g_sWightFrame.state & WIGHT_STAT_TX)
		{
			Wight_EnableDmaTx(g_aGetWigValue, 8 );
			g_sWightFrame.state &= ~WIGHT_STAT_TX;
		}
	}
}