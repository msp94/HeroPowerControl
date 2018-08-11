#include "task.h"
#include "monitor.h"
#include "control.h"
#include "remote.h"

extern u8 LedFlag;
FrameRate_Struct framerate = {0,0,0,0,0};

    //系统状态，每一位对应一个错误，0正常，1错误
/****************************************************************
    15  |   14  |   13  |   12  |   11  |   10  |   9   |   8   |
        |       |  imu2 | CAN1-4| CAN2-5| CAN2-4| CAN2-3| CAN2-2|
-----------------------------------------------------------------
    7   |   6   |   5   |   4   |   3   |   2   |   1   |   0   |
  CAN2-1| CAN1-3|CAN1-2 | CAN1-1| CAN1-6| CAN1-5|  imu  |  DBUS |
****************************************************************/
uint16_t StateFlagBit = 0;

OS_TCB MonitorTaskTCB;
CPU_STK MONITOR_TASK_STK[MONITOR_STK_SIZE];
/**
  * @brief  监视任务
  * @param  无
  * @retval 无
 **/
void monitor_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err);
		if(framerate.DBUS < 10)//遥控器
		{
			framerate.DBUS = 0;
			StateFlagBit |= 0x0001;
			RemoteDataInit();
		}
		else
		{
			StateFlagBit &= 0xFFFE;
			framerate.DBUS = 0;			
		}
		/*------------------------------------------------*/
		#if IS_CTR_CAN2_MOTOR_1 
		if(framerate.leftup < 100)
		{
			LedFlag = 0;
			StateFlagBit |= 0x0080;
			framerate.leftup = 0;
		}
		else
		{
			StateFlagBit &= 0xFF7F;
			framerate.leftup = 0;
		}
		#else
		StateFlagBit &= 0xFF7F;
		framerate.leftup = 0;
		#endif
		#if IS_CTR_CAN2_MOTOR_2 
		if(framerate.rightup < 100)
		{
			LedFlag = 0;
			StateFlagBit |= 0x0100;
			framerate.rightup = 0;
		}
		else
		{
			StateFlagBit &= 0xFEFF;
			framerate.rightup = 0;
		}
		#else
		StateFlagBit &= 0xFEFF;
		framerate.rightup = 0;
		#endif
		#if IS_CTR_CAN2_MOTOR_3 
		if(framerate.leftdown < 100)
		{
			LedFlag = 0;
			StateFlagBit |= 0x0200;
			framerate.leftdown = 0;
		}
		else
		{
			StateFlagBit &= 0xFDFF;
			framerate.leftdown = 0;
		}
		#else
		StateFlagBit &= 0xFDFF;
		framerate.leftdown = 0;
		#endif
		#if IS_CTR_CAN2_MOTOR_4 
		if(framerate.rightdown < 100)
		{
			LedFlag = 0;
			StateFlagBit |= 0x0400;
			framerate.rightdown = 0;
		}
		else
		{
			StateFlagBit &= 0xFBFF;
			framerate.rightdown = 0;
		}
		#else
		StateFlagBit &= 0xFBFF;
		framerate.rightdown = 0;
		#endif
	}
}
/**
  * @brief  监视任务创建
  * @param  无
  * @retval 无
 **/
void monitor_task_create(void)
{
	OS_ERR err;
	//创建消息队列
	OSTaskCreate((OS_TCB 	* )&MonitorTaskTCB,		
	               (CPU_CHAR	* )"monitor task", 		
                 (OS_TASK_PTR )monitor_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )MONITOR_TASK_PRIO,     
                 (CPU_STK   * )&MONITOR_TASK_STK[0],	
                 (CPU_STK_SIZE)MONITOR_STK_SIZE/10,	
                 (CPU_STK_SIZE)MONITOR_STK_SIZE,		
                 (OS_MSG_QTY  )0,
                 (OS_TICK	  )0,
                 (void   	* )0,
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);
}
