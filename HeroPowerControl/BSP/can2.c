#include "can.h"
/*----CAN2_TX-----PB13----*/
/*----CAN2_RX-----PB12----*/

void CAN2_Configuration(void)
{
  CAN_InitTypeDef        can;
  CAN_FilterInitTypeDef  can_filter;
  GPIO_InitTypeDef       gpio;
  NVIC_InitTypeDef       nvic;
	
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//–Ë“™≈‰÷√CAN1 œ»
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);
    
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_CAN2);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_CAN2); 

  gpio.GPIO_Pin  = GPIO_Pin_5 | GPIO_Pin_6 ;
  gpio.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(GPIOB, &gpio);
	
  nvic.NVIC_IRQChannel                   = CAN2_RX1_IRQn; 
  nvic.NVIC_IRQChannelPreemptionPriority = 2;
  nvic.NVIC_IRQChannelSubPriority        = 1;
  nvic.NVIC_IRQChannelCmd                = ENABLE;
  NVIC_Init(&nvic); 
	
  nvic.NVIC_IRQChannel                   = CAN2_TX_IRQn; 
  nvic.NVIC_IRQChannelPreemptionPriority = 1;
  nvic.NVIC_IRQChannelSubPriority        = 1;
  nvic.NVIC_IRQChannelCmd                = ENABLE;
  NVIC_Init(&nvic);

  CAN_DeInit(CAN2);
  CAN_StructInit(&can);

  can.CAN_TTCM      = DISABLE;
  can.CAN_ABOM      = ENABLE;    
  can.CAN_AWUM      = DISABLE;    
  can.CAN_NART      = DISABLE;    
  can.CAN_RFLM      = DISABLE;    
  can.CAN_TXFP      = ENABLE;     
  can.CAN_Mode      = CAN_Mode_Normal; 
  can.CAN_SJW       = CAN_SJW_1tq;
  can.CAN_BS1       = CAN_BS1_9tq;
  can.CAN_BS2       = CAN_BS2_4tq;
  can.CAN_Prescaler = 3;   //CAN BaudRate 42/(1+9+4)/3=1Mbps
  CAN_Init(CAN2, &can);
    
  can_filter.CAN_FilterNumber        =14;
  can_filter.CAN_FilterMode          =CAN_FilterMode_IdMask;
  can_filter.CAN_FilterScale         =CAN_FilterScale_32bit;
  can_filter.CAN_FilterIdHigh        =0x0000;
  can_filter.CAN_FilterIdLow         =0x0000;
  can_filter.CAN_FilterMaskIdHigh    =0x0000;
  can_filter.CAN_FilterMaskIdLow     =0x0000;
  can_filter.CAN_FilterFIFOAssignment=1;//the message which pass the filter save in fifo0
  can_filter.CAN_FilterActivation    =ENABLE;
  CAN_FilterInit(&can_filter);
		
  CAN_ITConfig(CAN2,CAN_IT_FMP1,ENABLE);
  CAN_ITConfig(CAN2,CAN_IT_TME,ENABLE); 
}
