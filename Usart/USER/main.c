#include "stm32f10x.h"

void USART1_Init(void)
{
	//GPIO,USART,NVIC ��ʼ���ṹ��
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	//ʹ����Ӧ�˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	//���ֲ��ʼ��A9
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;	//�̶�ģʽ ���
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	//���ֲ��ʼ��A10
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//�̶�ģʽ ���
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	//���ֲ��ʼ������
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//��ȡд��ģʽ
	USART_InitStruct.USART_Parity = USART_Parity_No;	//��żУ��
	USART_InitStruct.USART_StopBits = USART_StopBits_1;	//ֹͣλ
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;	//�ֳ�(ѡ��������żУ��)
	USART_Init(USART1, &USART_InitStruct);	//���ڳ�ʼ��
	
	USART_Cmd(USART1, ENABLE);	//���ڿ��� ʹ��
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	//�����ж����� ���������ж�
	
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;	//�ж����� ͨ��
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;	//��ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;//�����ȼ�
	NVIC_Init(&NVIC_InitStruct);	//��Ƕ�����жϿ�������ʼ��
	
}

void USART1_IRQHandler(void)
{
	u8 rec;
	if(USART_GetITStatus(USART1, USART_IT_RXNE)){
		rec = USART_ReceiveData(USART1);
		USART_SendData(USART1, rec);
	}
}

int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	USART1_Init();
	while(1);

}
