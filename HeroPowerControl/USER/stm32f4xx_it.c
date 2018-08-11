/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "sys.h"
#include "includes.h"
#include "STMGood.h"
#include "remote.h"
#include "can.h"
#include "led.h"

extern OS_SEM	TIME_SEM;
void TIM6_DAC_IRQHandler(void)  
{
	OS_ERR err;
	OSIntEnter();
	if (TIM_GetITStatus(TIM6,TIM_IT_Update)!= RESET) 
	{
		TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
		TIM_ClearFlag(TIM6, TIM_FLAG_Update);
		OSSemPost(&TIME_SEM,OS_OPT_POST_1,&err);//发送信号量
	}
	OSIntExit();
}

void DMA2_Stream5_IRQHandler(void)
{
	OSIntEnter();
	if(DMA_GetITStatus(DMA2_Stream5, DMA_IT_TCIF5))
	{
		DMA_ClearFlag(DMA2_Stream5, DMA_FLAG_TCIF5);
		DMA_ClearITPendingBit(DMA2_Stream5, DMA_IT_TCIF5);
		RemoteDataPrcess(rc_buffer);
	}
	OSIntExit();
}

void USART1_IRQHandler(void)
{
	OSIntEnter();
	if(USART_GetITStatus(USART1, USART_IT_ORE_RX) != RESET)
	{
		USART1->SR;
		USART1->DR;
		USART_ClearITPendingBit(USART1,USART_IT_ORE_RX);
	}
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{
		(void)USART1->SR;
		(void)USART1->DR;
		USART_ClearITPendingBit(USART1,USART_IT_IDLE);
		DMA_Cmd(DMA2_Stream5, DISABLE);
		DMA2_Stream5->NDTR = (uint16_t)18u;     //relocate the dma memory pointer to the beginning position
		DMA_Cmd(DMA2_Stream5, ENABLE);
	}
	OSIntExit();
}

extern OS_SEM	USART2_SEM;
extern char Rx_data2;
void USART2_IRQHandler(void)
{
	OS_ERR err;
	OSIntEnter();
	if(USART_GetITStatus(USART2, USART_IT_ORE_RX) != RESET)
	{
		USART2->SR;
		USART2->DR;
		USART_ClearITPendingBit(USART2,USART_IT_ORE_RX);
	}
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		Rx_data2  = USART2->DR;
		OSSemPost(&USART2_SEM,OS_OPT_POST_1,&err);//发送信号量
	}
	OSIntExit();
}
void CAN2_RX1_IRQHandler(void)
{
	OS_ERR err;
	OSIntEnter();
	if(CAN_GetITStatus(CAN2,CAN_IT_FMP1)!= RESET) 
	{
		CAN_ClearITPendingBit(CAN2, CAN_IT_FMP1);
		CAN_Receive(CAN2, CAN_FIFO1, &rx_message2);
		OSSemPost(&CAN2_SEM,OS_OPT_POST_1,&err);//发送信号量   
	}
	OSIntExit();
}
void CAN2_TX_IRQHandler(void)
{
	OSIntEnter();
	if(CAN_GetITStatus(CAN2,CAN_IT_TME)!= RESET) 
	{
		CAN_ClearITPendingBit(CAN2,CAN_IT_TME);
	}
	OSIntExit();
}
/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
//void PendSV_Handler(void)
//{
//}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
//void SysTick_Handler(void)
//{
// 
//}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
