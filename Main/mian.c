#include "SysCfg.h"

uint8_t g_aQueneStatic[128];
StaticQueue_t g_sQueneStatic = {0};
int main()
{
	Sys_Init();
	

	/*
	xQueue1 = xQueueCreate( QUEUE_LENGTH, sizeof( QUENE_INFO ) );			//������̬���У���ɾ���Ӷ��ͷ��ڴ�
	binarySemaphore = xSemaphoreCreateBinary();								//�����ź���
	
	//xQueue2 = xQueueCreateStatic(QUEUE_LENGTH, sizeof(QUENE_INFO), g_aQueneStatic, &g_sQueneStatic);				//��̬��������bug
	
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
	



	//ҵ��
	xTaskCreate(Sys_Uart2Task, "SysTaskUart2", configTIMER_TASK_STACK_DEPTH, NULL, 0, &SysTaskUart2);
	xTaskCreate(Sys_IsrTask, "SysIsrTask", configTIMER_TASK_STACK_DEPTH, NULL, 0, &SysIsrTask);
	xTaskCreate(Sys_WightTask, "SysWightTask", configTIMER_TASK_STACK_DEPTH, NULL, 0, &SysWightTask);
	
	/*����������*/
	vTaskStartScheduler(); 
	
	/* ����������е�������ͱ�ʾ������, һ�����ڴ治�� */
	return 0;
} 