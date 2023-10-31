#include "SysCfg.h"

int main()
{
	Sys_Init();

	xTaskCreate(SysLedTask, "Task 1", configTIMER_TASK_STACK_DEPTH, NULL, 2, NULL);
	vTaskStartScheduler(); 
	return 0;
}