#include "task.h"
#include "remote.h"
#include "control.h"
#include "pid.h"

OS_SEM	TIME_SEM;
OS_TCB ControlTaskTCB;
CPU_STK CONTROL_TASK_STK[CONTROL_STK_SIZE];

void control_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	while(1)
	{
		OSSemPend(&TIME_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量
		OS_CRITICAL_ENTER();	//进入临界区
		RemoteController();	
		ChassisController();
		OS_CRITICAL_EXIT();	//进入临界区
	}
}
void control_task_create(void)
{
	OS_ERR err;
	OSSemCreate ((OS_SEM*	)&TIME_SEM,
                 (CPU_CHAR*	)"TIME_SEM",
                 (OS_SEM_CTR)0,
                 (OS_ERR*	)&err);
	OSTaskCreate((OS_TCB 	* )&ControlTaskTCB,		
	               (CPU_CHAR	* )"control task", 		
                 (OS_TASK_PTR )control_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )CONTROL_TASK_PRIO,     
                 (CPU_STK   * )&CONTROL_TASK_STK[0],	
                 (CPU_STK_SIZE)CONTROL_STK_SIZE/10,	
                 (CPU_STK_SIZE)CONTROL_STK_SIZE,		
                 (OS_MSG_QTY  )0,
                 (OS_TICK	  )0,
                 (void   	* )0,
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);
}
