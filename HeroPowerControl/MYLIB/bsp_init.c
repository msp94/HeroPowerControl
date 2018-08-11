#include "bsp_init.h"
#include "control.h"
#include "remote.h"

/**
  * @brief  底层驱动初始化
  * @param  无
  * @retval 无
 **/
void BSP_Init(void)
{
	delay_init(168);       //延时初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //中断分组配置
	#if IS_OPEN_CAN2_TASK
	CAN2_Configuration();
	#endif
	delay_ms(100);
	
	Led_Configuration();    //LED初始化
	USART1_Configuration();
	delay_ms(50);
	#if IS_OPEN_USART2_TASK
	USART2_Configuration();
	delay_ms(50);
	#endif
	TIM6_Configuration();
	delay_ms(4300);
	TIM6_Start();
}
