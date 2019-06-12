#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
#include "beep.h"

void EXTIX_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	KEY_Init();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	//����IO�ڸ���ʱ��
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource4);	//����IO�����ж��ߵ�ӳ���ϵ
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line4;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);	//��ʼ�������жϣ����ô���������
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);	//�����жϷ��飨NVIC������ʹ���ж�
	
	
}

void EXTI4_IRQHandler(void)
{
	delay_ms(10);
	if(KEY0 == 0){
		LED0 = !LED0;
		LED1 = !LED1;
	}
	EXTI_ClearITPendingBit(EXTI_Line4);
}