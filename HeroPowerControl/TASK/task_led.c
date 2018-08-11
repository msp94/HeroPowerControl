#include "task.h"
#include "led.h"

u8 LedFlag = 1;

OS_TCB LedTaskTCB;
CPU_STK LED_TASK_STK[LED_STK_SIZE];

void led_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		if(LedFlag)
		{
			LED0=~LED0;			
		}
		OSTimeDlyHMSM(0,0,0,300,OS_OPT_TIME_HMSM_STRICT,&err);
	}
}

void led_task_create(void)	//创建LED0任务
{
	OS_ERR err;
	OSTaskCreate((OS_TCB 	* )&LedTaskTCB,		
	               (CPU_CHAR	* )"led task", 		
                 (OS_TASK_PTR )led_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )LED_TASK_PRIO,     
                 (CPU_STK   * )&LED_TASK_STK[0],	
                 (CPU_STK_SIZE)LED_STK_SIZE/10,	
                 (CPU_STK_SIZE)LED_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);
}
