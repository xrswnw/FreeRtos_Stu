#include "SysCfg.h"


int main()
{
	Sys_Init();

	xTaskCreate(SysLedTask1, "Task 1", configTIMER_TASK_STACK_DEPTH, NULL, 2, &handSysLedTask1);
	vTaskStartScheduler(); 
	return 0;
} 