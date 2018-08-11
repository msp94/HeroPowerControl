#include "bsp_init.h"
#include "task.h"
#include "pid.h"
#include "remote.h"
#include "sys.h"
#include "includes.h"


int main(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	
	RemoteDataInit();
	ChassisDataInit();
	BSP_Init();
	
	OSInit(&err);		//初始化UCOSIII
	OS_CRITICAL_ENTER();//进入临界区
	start_Task_Create();	//创建开始任务
	OS_CRITICAL_EXIT();	//退出临界区	 
	OSStart(&err);  //开启UCOSIII
	while(1);
}
