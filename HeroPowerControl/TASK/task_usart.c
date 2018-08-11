#include "task.h"
#include "STMGood.h"

OS_TCB Usart2TaskTCB;
CPU_STK USART2_TASK_STK[USART2_STK_SIZE];

char Rx_data2 = 0;
OS_SEM	USART2_SEM;

void usart2_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		OSSemPend(&USART2_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量
		Dealdata(Rx_data2);
		OSSched();
	}
}
void usart2_task_create(void)
{
	OS_ERR err;
	//创建一个信号量
	OSSemCreate ((OS_SEM*	)&USART2_SEM,
                 (CPU_CHAR*	)"USART2_SEM",
                 (OS_SEM_CTR)0,		
                 (OS_ERR*	)&err);
	OSTaskCreate((OS_TCB 	* )&Usart2TaskTCB,		
	               (CPU_CHAR	* )"usart2 task", 		
                 (OS_TASK_PTR )usart2_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )USART2_TASK_PRIO,     
                 (CPU_STK   * )&USART2_TASK_STK[0],	
                 (CPU_STK_SIZE)USART2_STK_SIZE/10,	
                 (CPU_STK_SIZE)USART2_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);
}
