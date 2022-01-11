/**********************************************************************************************************
 * @�ļ�     can2.c
 * @˵��     can2��ʼ��
 * @�汾  	 V1.1
 * @����     ����
 * @����     2021.12
**********************************************************************************************************/
#include "main.h"
/**********************************************************************************************************
*�� �� ��: CAN2_Configuration
*����˵��: can2��ʼ��
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void CAN2_Configuration()
{
		GPIO_InitTypeDef GPIO_InitStructure;
		CAN_InitTypeDef        CAN_InitStructure;
		CAN_FilterInitTypeDef  CAN_FilterInitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		/* ��GPIOʱ�ӡ�AFIOʱ�ӣ�CANʱ�� */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);

		/* CAN2 RX PB12 */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		/* CAN2 TX PB13 */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType= GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);								


		/* CAN2_RX1 Enabling interrupt */								 	  
		NVIC_InitStructure.NVIC_IRQChannel=CAN2_RX1_IRQn;	// FIFO_1
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);				


		/* CAN  BaudRate = RCC_APB1PeriphClock/(1+CAN_BS1+CAN_BS2)/CAN_Prescaler */
		CAN_DeInit(CAN2);
		CAN_StructInit(&CAN_InitStructure);   
		
		CAN_InitStructure.CAN_TTCM=DISABLE;
		CAN_InitStructure.CAN_ABOM=DISABLE;
		CAN_InitStructure.CAN_AWUM=DISABLE;
		CAN_InitStructure.CAN_NART=DISABLE;
		CAN_InitStructure.CAN_RFLM=DISABLE;
		CAN_InitStructure.CAN_TXFP=DISABLE;
		CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;   
		//CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack;
		CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
		CAN_InitStructure.CAN_BS1=CAN_BS1_4tq;  
		CAN_InitStructure.CAN_BS2=CAN_BS2_2tq;	
		CAN_InitStructure.CAN_Prescaler=6;

		CAN_Init(CAN2,&CAN_InitStructure);   // CAN2		
		
		CAN_FilterInitStructure.CAN_FilterNumber=14;	// 
		CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdList;	 // ��ʶ���б�ģʽ
		CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_16bit;   // 16λ������
		CAN_FilterInitStructure.CAN_FilterIdHigh=0x101 << 5;			// ��������ʶ��
		CAN_FilterInitStructure.CAN_FilterIdLow=0x072<<5 | CAN_ID_STD;				
		CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x102 << 5;		
		CAN_FilterInitStructure.CAN_FilterMaskIdLow=0 | CAN_ID_STD;
		CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO1;	 // FIFO1ָ�������
		CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
		CAN_FilterInit(&CAN_FilterInitStructure);
		CAN_ITConfig(CAN2,CAN_IT_FMP1,ENABLE);  // CAN2 FIFO1

}

/**********************************************************************************************************
*�� �� ��: CAN2_RX0_IRQHandler
*����˵��: can2�����ж� ����������Ϣ
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
//CanRxMsg rx_message_0;
//void CAN2_RX0_IRQHandler()
//{
//	if (CAN_GetITStatus(CAN2,CAN_IT_FMP0)!= RESET) 
//	{
//		CAN_Receive(CAN2, CAN_FIFO0, &rx_message_0);
//  	Can2Receive0(&rx_message_0);
//		CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);
//	}
//}

//CanRxMsg rx_message_1;
//void CAN2_RX0_IRQHandler()
//{
//	if (CAN_GetITStatus(CAN2,CAN_IT_FMP1)!= RESET) 
//	{
//		CAN_Receive(CAN2, CAN_FIFO1, &rx_message_1);
//  	Can2Receive1(&rx_message_1);
//		CAN_ClearITPendingBit(CAN2, CAN_IT_FMP1);
//	}
//}

/**********************************************************************************************************
*�� �� ��: CAN2_RX1_IRQHandler
*����˵��: can2�����ж� xyw���ٶ���Ϣ
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
CanRxMsg rx_message_2;
void CAN2_RX1_IRQHandler()
{
	if (CAN_GetITStatus(CAN2,CAN_IT_FMP1)!= RESET) 
	{
		CAN_Receive(CAN2, CAN_FIFO1, &rx_message_2);
  	Can2Receive1(&rx_message_2);
		CAN_ClearITPendingBit(CAN2, CAN_IT_FMP1);
	}
}

