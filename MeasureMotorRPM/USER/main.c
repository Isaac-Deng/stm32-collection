#include "stm32f10x.h"
#include "timer.h"
#include "delay.h"
#include "usart.h"


///////////////////////////////////////
//������ת�ٲ��Գ���	
//ʹ��PA0
//
//������ 
//2018/8/2
///////////////////////////////////////

extern u8	MEASURE_MODE;		//0:����ģʽ   ѡ��ߵ�ƽ����  1:ѡ�����ڲ���

int main(void)
{	
	u32 getPeriod = 0;
	u32 roundPerMin = 0;
	
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
	
 	TIM5_Cap_Init(0XFFFF,72-1);	//��1Mhz��Ƶ�ʼ��� oxFFFF���	//����Ƶ���Եõ����߾���?
	
	while(1)
	{
//		printf("OK\r\n");
//		delay_ms(1000);

		if((getPeriod = TIM5_GetExactValue(MODE_PERIOD)) != 0){
			roundPerMin = 60000000 / (getPeriod * 13);
			printf("Period: %d us      RoundPerMinute: %d RPM  \r\n", getPeriod, roundPerMin);
		}	
	}
}
