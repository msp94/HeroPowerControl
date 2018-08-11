#include "task.h"
#include "print.h"
#include "pid.h"
#include "remote.h"
#include "judge.h"

OS_TCB PrintTaskTCB;
CPU_STK PRINT_TASK_STK[PRINT_STK_SIZE];
/**
  * @brief  打印任务
  * @param  无
  * @retval 无
 **/
void print_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		OSTimeDlyHMSM(0,0,Delay_s,Delay_ms,OS_OPT_TIME_HMSM_STRICT,&err);
	}
}
/**
  * @brief  打印任务创建
  * @param  无
  * @retval 无
 **/
void print_task_create(void)
{
	OS_ERR err;
	OSTaskCreate((OS_TCB 	* )&PrintTaskTCB,		
	               (CPU_CHAR	* )"print task", 		
                 (OS_TASK_PTR )print_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )PRINT_TASK_PRIO,     
                 (CPU_STK   * )&PRINT_TASK_STK[0],	
                 (CPU_STK_SIZE)PRINT_STK_SIZE/10,	
                 (CPU_STK_SIZE)PRINT_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);
}
