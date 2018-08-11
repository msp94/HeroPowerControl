#include "usart1.h"
#include "remote.h"

/**
  * @brief  USART1初始化
  * @param  无
  * @retval 无
 **/
void USART1_Configuration(void)
{
  USART_InitTypeDef usart;
	GPIO_InitTypeDef  gpio;
  NVIC_InitTypeDef  nvic;
  DMA_InitTypeDef   dma;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_DMA2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	
	gpio.GPIO_Pin = GPIO_Pin_10;
	gpio.GPIO_Mode = GPIO_Mode_AF;
  gpio.GPIO_OType = GPIO_OType_PP;
  gpio.GPIO_Speed = GPIO_Speed_100MHz;
  gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&gpio);
    
  USART_DeInit(USART1);
	usart.USART_BaudRate = 100000;   //SBUS 100K baudrate
	usart.USART_WordLength = USART_WordLength_8b;
	usart.USART_StopBits = USART_StopBits_1;
	usart.USART_Parity = USART_Parity_Even;
	usart.USART_Mode = USART_Mode_Rx;
  usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1,&usart); 
	
	nvic.NVIC_IRQChannel = USART1_IRQn;                          
	nvic.NVIC_IRQChannelPreemptionPriority = 1;   //pre-emption priority 
	nvic.NVIC_IRQChannelSubPriority = 1;		    //subpriority 
	nvic.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&nvic);
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
  nvic.NVIC_IRQChannel = DMA2_Stream5_IRQn;
  NVIC_Init(&nvic);
	
  DMA_DeInit(DMA2_Stream5);
  DMA_StructInit(&dma);
  dma.DMA_Channel= DMA_Channel_4;
  dma.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DR);
  dma.DMA_Memory0BaseAddr = (uint32_t)&rc_buffer;
  dma.DMA_DIR =DMA_DIR_PeripheralToMemory;
  dma.DMA_BufferSize = 18;
  dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
  dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  dma.DMA_Mode = DMA_Mode_Circular;
  dma.DMA_Priority = DMA_Priority_Medium;
  dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
  dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
  dma.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream5,&dma);

	DMA_ITConfig(DMA2_Stream5,DMA_IT_TC,ENABLE);
  DMA_Cmd(DMA2_Stream5,ENABLE);
  USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);        //usart rx idle interrupt  enabled
  USART_Cmd(USART1, ENABLE);
}
