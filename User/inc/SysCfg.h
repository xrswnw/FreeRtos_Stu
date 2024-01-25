#ifndef _ANYID_SM5001_SYSCFG_H
#define _ANYID_SM5001_SYSCFG_H


#include "SysCfg.h"
#include "AnyID_CS1110_Reader.h"

#define SYS_STAT_IDLE                   0x00000001	       



extern const PORT_INF SYS_RUN_LED;
#define Sys_LedOpen()				do{GPIO_BOP(SYS_RUN_LED.port) = SYS_RUN_LED.pin;}while(0)				//set
#define Sys_LedOff()				do{GPIO_BC(SYS_RUN_LED.port) = SYS_RUN_LED.pin;}while(0)				//reset


extern const PORT_INF IO_LED_CTL;
#define Sys_RunLedOpen()			do{GPIO_BOP(IO_LED_CTL.port) = IO_LED_CTL.pin;}while(0)				//set
#define Sys_RunLedOff()				do{GPIO_BC(IO_LED_CTL.port) = IO_LED_CTL.pin;}while(0)				//reset




extern const PORT_INF SYS_LED_RED;
#define Sys_LedRedOff()					do{GPIO_BOP(SYS_LED_RED.port) = SYS_RUN_LED.pin;}while(0)				//set
#define Sys_LedRedOn()                  do{GPIO_BC(SYS_LED_RED.port) = SYS_RUN_LED.pin;}while(0)				//reset
extern const PORT_INF SYS_LED_GREEN;
#define Sys_LedGreenOff()				do{GPIO_BOP(SYS_LED_GREEN.port) = SYS_RUN_LED.pin;}while(0)				//set
#define Sys_LedGreenOn()                 do{GPIO_BC(SYS_LED_GREEN.port) = SYS_RUN_LED.pin;}while(0)				//reset

extern const PORT_INF SYS_LED_BLUE;
#define Sys_LedBlueOff()				do{GPIO_BOP(SYS_LED_BLUE.port) = SYS_RUN_LED.pin;}while(0)				//set
#define Sys_LedBlueOn()                 do{GPIO_BC(SYS_LED_BLUE.port) = SYS_RUN_LED.pin;}while(0)				//reset



#define QUEUE_LENGTH  	20			//队列长度
#define ITEM_SIZE  		128			//单个子元素长度

typedef struct queneInfo{
	uint8_t	num;
	uint8_t	buffer[ITEM_SIZE];
}QUENE_INFO;

typedef struct sysInfo{
	u32 state;
	u32 ledState;
}SYS_INFO;



















extern QueueHandle_t xQueue1;
extern QueueHandle_t xQueue2;
extern TaskHandle_t handSysLedTask1;
extern TaskHandle_t handSysLedTask2;
extern SemaphoreHandle_t binarySemaphore;

extern TaskHandle_t sysQueneTaskHandel1;
extern TaskHandle_t sysQueneTaskHandel2;

void SysLedTask1(void *pParaments);
void SysLedTask2(void *pParaments);
void Sys_Init(void);
void Sys_CtrlIOInit(void);
void SysReadQueueTask(void *pParaments);
void SysWriteQueueTask(void *pParaments);




//业务
extern TaskHandle_t SysTaskUart2;
void Sys_Uart2Task();
extern TaskHandle_t SysIsrTask;
void Sys_IsrTask();
extern TaskHandle_t SysWightTask;
void Sys_WightTask();
#endif
