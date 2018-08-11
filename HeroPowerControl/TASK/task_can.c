#include "task.h"
#include "led.h"
#include "can.h"
#include "remote.h"
#include "pid.h"
#include "monitor.h"
#include "judge.h"

OS_TCB Can2TaskTCB;
CPU_STK CAN2_TASK_STK[CAN2_STK_SIZE];

OS_SEM	CAN2_SEM;
CanRxMsg rx_message2;

void can2_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	while(1)
	{
		OSSemPend(&CAN2_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量
		OS_CRITICAL_ENTER();	//进入临界区
		switch(rx_message2.StdId)
		{
			case 0x201:
			{
				framerate.leftup++;
				chassisparam.LefUpBackSpeed=rx_message2.Data[2]<<8 | rx_message2.Data[3];
				break;
			}
			case 0x202:
			{
				framerate.rightup++;
				chassisparam.RigUpBackSpeed=rx_message2.Data[2]<<8 | rx_message2.Data[3];
				break;
			}
			case 0x203:
			{
				framerate.leftdown++;
				chassisparam.LefDoBackSpeed=rx_message2.Data[2]<<8 | rx_message2.Data[3];
				break;
			}
			case 0x204:
			{
				framerate.rightdown++;
				chassisparam.RigDoBackSpeed=rx_message2.Data[2]<<8 | rx_message2.Data[3];
				break;
			}
			case 0x104:
			{
				Receive104(rx_message2.Data);
				break;
			}
			default:
				break;
		}
		OS_CRITICAL_EXIT();	//进入临界区
		OSSched();
	}
}

void can2_task_create(void)
{
	OS_ERR err;
	OSSemCreate ((OS_SEM*	)&CAN2_SEM,
                 (CPU_CHAR*	)"CAN2_SEM",
                 (OS_SEM_CTR)0,
                 (OS_ERR*	)&err);
	OSTaskCreate((OS_TCB 	* )&Can2TaskTCB,
	               (CPU_CHAR	* )"can2 task",
                 (OS_TASK_PTR )can2_task,
                 (void		* )0,
                 (OS_PRIO	  )CAN2_TASK_PRIO,
                 (CPU_STK   * )&CAN2_TASK_STK[0],	
                 (CPU_STK_SIZE)CAN2_STK_SIZE/10,
                 (CPU_STK_SIZE)CAN2_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK	  )0,
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);
}
