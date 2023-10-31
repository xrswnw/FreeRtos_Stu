#ifndef _ANYID_SM5001_SYSCFG_H
#define _ANYID_SM5001_SYSCFG_H


#include "SysCfg.h"
#include "Uart2.h"


#define SYS_STAT_IDLE                   0x00000001	       



extern const PORT_INF SYS_RUN_LED;
#define Sys_LedOpen()				do{GPIO_BOP(SYS_RUN_LED.port) = SYS_RUN_LED.pin;}while(0)
#define Sys_LedOff()				do{GPIO_BC(SYS_RUN_LED.port) = SYS_RUN_LED.pin;}while(0)






void SysLedTask(void *pParaments);
void Sys_Init(void);
void Sys_CtrlIOInit(void);

#endif
