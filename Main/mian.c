#include "SysCfg.h"

uint8_t g_aQueneStatic[128];
StaticQueue_t g_sQueneStatic = {0};
int main()
{
	Sys_Init();
	

	/*
	xQueue1 = xQueueCreate( QUEUE_LENGTH, sizeof( QUENE_INFO ) );			//创建动态队列，可删除从而释放内存
	binarySemaphore = xSemaphoreCreateBinary();								//创建信号量
	
	//xQueue2 = xQueueCreateStatic(QUEUE_LENGTH, sizeof(QUENE_INFO), g_aQueneStatic, &g_sQueneStatic);				//静态任务留有bug
	
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
*/
	



	//业务
	xTaskCreate(Sys_Uart2Task, "SysTaskUart2", configTIMER_TASK_STACK_DEPTH, NULL, 0, &SysTaskUart2);
	xTaskCreate(Sys_IsrTask, "SysIsrTask", configTIMER_TASK_STACK_DEPTH, NULL, 0, &SysIsrTask);
	xTaskCreate(Sys_WightTask, "SysWightTask", configTIMER_TASK_STACK_DEPTH, NULL, 0, &SysWightTask);
	
	/*调度器启动*/
	vTaskStartScheduler(); 
	
	/* 如果程序运行到了这里就表示出错了, 一般是内存不足 */
	return 0;
} 