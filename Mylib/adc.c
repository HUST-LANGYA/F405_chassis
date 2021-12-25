/**********************************************************************************************************
 * @文件     adc.c
 * @说明     adc初始化
 * @版本  	 V1.0
 * @作者     戴军
 * @日期     2021.12
**********************************************************************************************************/
#include "main.h"

unsigned short ADC_ConvertedValue[90];
#define ADC1_DR_Address    ((u32)0x4001244C)
#define ADC2_DR_Address    ((u32)0x4001244C)

/**********************************************************************************************************
*函 数 名: ADC_Configuration
*功能说明: ADC初始化(PA2|4	电容电压)
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void ADC_Configuration(void)
{
	GPIO_InitTypeDef         gpio;
	ADC_CommonInitTypeDef    adc_common;
	ADC_InitTypeDef          adc;
	DMA_InitTypeDef          dma;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2|RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
 

	gpio.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_4;  //I_IN	
	gpio.GPIO_Mode = GPIO_Mode_AIN;
  gpio.GPIO_PuPd=GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio);
	
	DMA_DeInit(DMA2_Stream0);
	dma.DMA_PeripheralBaseAddr = (u32)&(ADC1->DR);
	dma.DMA_Memory0BaseAddr = (u32)&ADC_ConvertedValue;
	dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
	dma.DMA_BufferSize = 90;//每个通道30
	dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
	dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	dma.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	
	dma.DMA_Mode = DMA_Mode_Circular;
	dma.DMA_Priority = DMA_Priority_High;
	dma.DMA_Channel=DMA_Channel_0;
	dma.DMA_FIFOMode=DMA_FIFOMode_Disable;
	dma.DMA_FIFOThreshold=DMA_FIFOThreshold_1QuarterFull;
	dma.DMA_MemoryBurst=DMA_MemoryBurst_Single;
	dma.DMA_PeripheralBurst=DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &dma);
	DMA_Cmd(DMA2_Stream0, ENABLE);
	
  adc_common.ADC_Mode=ADC_Mode_Independent;
  adc_common.ADC_Prescaler=ADC_Prescaler_Div6;	
	adc_common.ADC_DMAAccessMode=ADC_DMAAccessMode_1;
	ADC_CommonInit(&adc_common);

				 
	adc.ADC_ScanConvMode = ENABLE;			//各通道顺序扫描,若采集多个通道，必须开启此模式             
	adc.ADC_ContinuousConvMode = ENABLE;	               
	adc.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;  
	adc.ADC_DataAlign = ADC_DataAlign_Right;
	adc.ADC_NbrOfConversion = 3;						//扫描3个通道
	adc.ADC_Resolution=ADC_Resolution_12b;
	ADC_Init(ADC1, &adc);
	
	ADC_TempSensorVrefintCmd(ENABLE);//
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_480Cycles);	//u_in
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_480Cycles);	//u_in
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 3, ADC_SampleTime_480Cycles);	//u_in
	
	ADC_DMACmd(ADC1, ENABLE);

	ADC_Cmd(ADC1, ENABLE);
	ADC_SoftwareStartConv(ADC1);
}



