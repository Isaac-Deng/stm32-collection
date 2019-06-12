#include "exti.h"
#include "usart.h"
#include "scan_print.h"
#include "delay.h"

u8 col = 0;	//���ж϶�ȡ�İ���������

//����GPIO��ʼ������
void GPIOX_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOF, ENABLE);//ʹ��PORT E,F ʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_4|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //���ó��������		//ԭ������ʶ��Ҫ��Ϊ��©���������������ʶ�����������
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE 0,2,4,6

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_4|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //���ó���������  
	GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��GPIOF 0,2,4,6

}


//�ⲿ�жϷ������
void EXTIX_Init(void)
{
 
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

  GPIOX_Init();	//GPIO��ʼ������

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��

  //GPIOF.0 �ж����Լ��жϳ�ʼ������   �����ش���
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource0);
  EXTI_InitStructure.EXTI_Line=EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  //GPIOF.2	  �ж����Լ��жϳ�ʼ������  �����ش���
 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource2);
 	EXTI_InitStructure.EXTI_Line=EXTI_Line2;
 	EXTI_Init(&EXTI_InitStructure);

	//GPIOF.4	  �ж����Լ��жϳ�ʼ������  �����ش���
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource4);
  EXTI_InitStructure.EXTI_Line=EXTI_Line4;
  EXTI_Init(&EXTI_InitStructure);

  //GPIOF.6	  �ж����Լ��жϳ�ʼ������  �����ش���
 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOF,GPIO_PinSource6); 
  EXTI_InitStructure.EXTI_Line=EXTI_Line6;
  EXTI_Init(&EXTI_InitStructure);



  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//ʹ��GPIOF.0���ڵ��ⲿ�ж�ͨ��
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure); 

  NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;			//ʹ��GPIOF.2���ڵ��ⲿ�ж�ͨ��
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//ʹ��GPIOF.4���ڵ��ⲿ�ж�ͨ��
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//�����ȼ�1 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ��GPIOF.6���ڵ��ⲿ�ж�ͨ��
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//�����ȼ�0 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
 
}

#define DELAY_SHAKE 200
//�ⲿ�жϷ������ 
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0)){
		delay_ms(DELAY_SHAKE);
		if(GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_0)){	//�жϷ�ʽ�ƺ�Ƿ��
			col = 0;
			scan_print(col);
			scan_init();
			EXTI_ClearITPendingBit(EXTI_Line0);
		}else{
			EXTI_ClearITPendingBit(EXTI_Line0);
		}
	}
}

void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2)){
		delay_ms(DELAY_SHAKE);
		if(GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_2)){
			col = 1;
			scan_print(col);
			scan_init();
			EXTI_ClearITPendingBit(EXTI_Line2);
		}else{
			EXTI_ClearITPendingBit(EXTI_Line2);
		}
	}
}

void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4)){
		delay_ms(DELAY_SHAKE);
		if(GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_4)){
			col = 2;
			scan_print(col);
			scan_init();
			EXTI_ClearITPendingBit(EXTI_Line4);
		}else{
			EXTI_ClearITPendingBit(EXTI_Line4);
		}
	}
}

void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line6)){
		delay_ms(DELAY_SHAKE);
		if(GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_6)){
			col = 3;
			scan_print(col);
			scan_init();
			EXTI_ClearITPendingBit(EXTI_Line6);
		}else{
			EXTI_ClearITPendingBit(EXTI_Line6);
		}
	}
}
