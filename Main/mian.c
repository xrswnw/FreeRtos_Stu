#include "SysCfg.h"


int main()
{
	Sys_Init();
	

	
	xQueue1 = xQueueCreate( QUEUE_LENGTH, sizeof( QUENE_INFO ) );			//创建队列
	binarySemaphore = xSemaphoreCreateBinary();								//创建信号量
	
	if(binarySemaphore != NULL)
	{
		xTaskCreate(SysLedTask1, "Task 1", configTIMER_TASK_STACK_DEPTH, NULL, 0, &handSysLedTask1);
		xTaskCreate(SysLedTask2, "Task 1", configTIMER_TASK_STACK_DEPTH, NULL, 1, &handSysLedTask2);
		
		xTaskCreate(SysWriteQueueTask, "Task 3", configTIMER_TASK_STACK_DEPTH, NULL, 1, &sysQueneTaskHandel1);
		xTaskCreate(SysReadQueueTask, "Task 4", configTIMER_TASK_STACK_DEPTH, NULL, 0, &sysQueneTaskHandel2);
		vTaskStartScheduler(); 
	}
	else
	{
		return 0;
	}
	
} 