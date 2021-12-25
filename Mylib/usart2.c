/**********************************************************************************************************
 * @�ļ�     usart2.c
 * @˵��     usart2��ʼ��,DataScope ������9600
 * @�汾  	 V1.1
 * @����     ����
 * @����     2021.12
**********************************************************************************************************/
#include "main.h"

//unsigned char DataScopeSend_Buf[DataScopeSend_BufSize];

/**********************************************************************************************************
*�� �� ��: USART2_Configuration
*����˵��: USART2��ʼ�� ����DataScope)
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void USART2_Configuration(void)
{
	USART_InitTypeDef usart;
	GPIO_InitTypeDef  gpio;
	NVIC_InitTypeDef  nvic;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2|GPIO_PinSource3,GPIO_AF_USART2);

	gpio.GPIO_Pin = GPIO_Pin_3;			//��������
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio);

	gpio.GPIO_Pin = GPIO_Pin_2;  			//�������� 
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio);

	USART_DeInit(USART2);
	usart.USART_BaudRate = 9600;			//10000
	usart.USART_WordLength = USART_WordLength_8b;
	usart.USART_StopBits = USART_StopBits_1;
	usart.USART_Parity = USART_Parity_No;
	usart.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   
	USART_Init(USART2,&usart);
	  
	USART_Cmd(USART2,ENABLE);
	
	nvic.NVIC_IRQChannel = USART2_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 1;
	nvic.NVIC_IRQChannelSubPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;		
	NVIC_Init(&nvic);
}



