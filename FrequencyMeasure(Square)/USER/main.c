#include "stm32f10x.h"
#include "timer.h"
#include "delay.h"
#include "usart.h"


///////////////////////////////////////
//������Ƶ�ʡ�ռ�ձȲ�������Ԥ�Ʊ�д���ַ�����
//
//		������ 2018/7/25
///////////////////////////////////////

extern u8	MEASURE_MODE;		//0:����ģʽ   ѡ��ߵ�ƽ����  1:ѡ�����ڲ���

int main(void)
{	
	u32 getPeriod = 0;
	u32 getHLevel = 0;
	
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
	
 	TIM5_Cap_Init(0XFFFF,72-1);	//��1Mhz��Ƶ�ʼ��� oxFFFF���
	
	while(1)
	{
//		printf("OK\r\n");
		delay_ms(1000);
		if((getHLevel = TIM5_GetExactValue(MODE_HLEVEL)) != 0)
			printf("H-Level: %d us\r\n", getHLevel);
		if((getPeriod = TIM5_GetExactValue(MODE_PERIOD)) != 0)
			printf("Perior: %d us\r\n", getPeriod);
		printf("Duty_Ratio:%.2f%%\r\n\r\n", (float)((double)getHLevel/((double)getPeriod/100)));
	}
	
}
