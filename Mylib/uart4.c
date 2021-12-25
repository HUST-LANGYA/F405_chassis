/**********************************************************************************************************
 * @文件     uart4.c
 * @说明     uart4初始化(裁判系统通信)
 * @版本  	 V1.1
 * @作者     戴军
 * @日期     2021.12
**********************************************************************************************************/
#include "uart4.h"

unsigned char JudgeReceiveBuffer[JudgeBufBiggestSize];
unsigned char JudgeSend[SEND_MAX_SIZE];
/**********************************************************************************************************
*函 数 名: UART4_Configuration
*功能说明: uart4初始化(裁判系统通信)
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void UART4_Configuration(void)
{
	USART_InitTypeDef usart;
	GPIO_InitTypeDef  gpio;
	NVIC_InitTypeDef  nvic;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10|GPIO_PinSource11,GPIO_AF_UART4);
	
	gpio.GPIO_Pin = GPIO_Pin_11;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&gpio);

	gpio.GPIO_Pin = GPIO_Pin_10;  
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_PuPd=GPIO_PuPd_UP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&gpio);

	USART_DeInit(UART4);
	usart.USART_BaudRate = 115200;
	usart.USART_WordLength = USART_WordLength_8b;
	usart.USART_StopBits = USART_StopBits_1;
	usart.USART_Parity = USART_Parity_No ;
	usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   
	USART_Init(UART4,&usart);

	USART_Cmd(UART4,ENABLE);
	USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE); 
	
	nvic.NVIC_IRQChannel = DMA1_Stream2_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
	
	{
		DMA_InitTypeDef  dma;

		dma.DMA_PeripheralBaseAddr = (uint32_t)&(UART4->DR);
		dma.DMA_Memory0BaseAddr = (uint32_t)JudgeReceiveBuffer;
		dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
		dma.DMA_BufferSize = JudgeBufBiggestSize;
		dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
		dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		dma.DMA_Mode = DMA_Mode_Circular;
		dma.DMA_Priority = DMA_Priority_VeryHigh;
		dma.DMA_Channel=DMA_Channel_4;
		DMA_Init(DMA1_Stream2, &dma);
		DMA_ITConfig(DMA1_Stream2,DMA_IT_TC,ENABLE);
		DMA_Cmd(DMA1_Stream2, ENABLE);
	}
  
    nvic.NVIC_IRQChannel = DMA1_Stream4_IRQn;
		nvic.NVIC_IRQChannelPreemptionPriority = 3;
		nvic.NVIC_IRQChannelSubPriority = 3;
		nvic.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&nvic);
		USART_DMACmd(UART4,USART_DMAReq_Tx,ENABLE);
		{
		  DMA_InitTypeDef 	dma;
			dma.DMA_PeripheralBaseAddr = (uint32_t)&(UART4->DR);
			dma.DMA_Memory0BaseAddr = (uint32_t)JudgeSend;
			dma.DMA_DIR = DMA_DIR_MemoryToPeripheral;
			dma.DMA_BufferSize = 30;
			dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
			dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
			dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
			dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
			dma.DMA_Mode = DMA_Mode_Circular;
			dma.DMA_Priority = DMA_Priority_VeryHigh;
			dma.DMA_Channel=DMA_Channel_4;

			DMA_Init(DMA1_Stream4,&dma);
			DMA_ITConfig(DMA1_Stream4,DMA_IT_TC,ENABLE);
			DMA_Cmd(DMA1_Stream4,DISABLE);
	 }		
}

void DMA1_Stream2_IRQHandler()
{	
	if(DMA_GetFlagStatus(DMA1_Stream2,DMA_FLAG_TCIF4) == SET)
	{
		DMA_ClearFlag(DMA1_Stream2,DMA_FLAG_TCIF4);
    JudgeBuffReceive(JudgeReceiveBuffer,0); 
	}
}

void DMA1_Stream4_IRQHandler()
{
	if(DMA_GetITStatus(DMA1_Stream4,DMA_FLAG_TCIF4)!=RESET)
	{
		DMA_ClearFlag(DMA1_Stream4,DMA_FLAG_TCIF4);
		DMA_ClearITPendingBit(DMA1_Stream4,DMA_IT_TCIF4);
		DMA_Cmd(DMA1_Stream4,DISABLE);
	}	
}
