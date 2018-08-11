#ifndef _TASK_H__
#define _TASK_H__

#include "sys.h"
#include "includes.h"

/*---------------------------------------------------------*/
//任务优先级
#define START_TASK_PRIO		3
//任务堆栈大小	
#define START_STK_SIZE 		256

void start_task(void *p_arg);
void start_Task_Create(void);
/*---------------------------------------------------------*/
//任务优先级
#define CONTROL_TASK_PRIO		4
//任务堆栈大小
#define CONTROL_STK_SIZE 		512

extern OS_TCB ControlTaskTCB;

void control_task(void *p_arg);
void control_task_create(void);
/*---------------------------------------------------------*/
//任务优先级
#define CAN2_TASK_PRIO		7
//任务堆栈大小
#define CAN2_STK_SIZE 		256

void can2_task(void *p_arg);
void can2_task_create(void);
/*---------------------------------------------------------*/
//任务优先级
#define USART2_TASK_PRIO		8
//任务堆栈大小
#define USART2_STK_SIZE 		256

void usart2_task(void *p_arg);
void usart2_task_create(void);
/*---------------------------------------------------------*/
//任务优先级
#define MONITOR_TASK_PRIO		10
//任务堆栈大小	
#define MONITOR_STK_SIZE 		256

void monitor_task(void *p_arg);
void monitor_task_create(void);
/*---------------------------------------------------------*/
//任务优先级
#define LED_TASK_PRIO		11
//任务堆栈大小	
#define LED_STK_SIZE 		128

extern OS_TCB LedTaskTCB;

void led_task(void *p_arg);
void led_task_create(void);
/*---------------------------------------------------------*/
//任务优先级
#define PRINT_TASK_PRIO		12
//任务堆栈大小	
#define PRINT_STK_SIZE 		512

void print_task(void *p_arg);
void print_task_create(void);
/*---------------------------------------------------------*/

#endif
