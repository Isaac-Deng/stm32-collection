#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include "adc.h"
#include "timer.h"
#include "key.h"
 
//////////////////////////////////////////////////
//��Ƶ�źŲ��Գ���
//���� 8kHz	//V-==GND  V+==3.3V
//
//������
//2018/8/7
//////////////////////////////////////////////////
#define SAMPLE_SIZE 8000*3	//������������

u16 voiceSample[SAMPLE_SIZE] = {0};	//���������������
extern u8 adcFlag;
	
int main(void)
{	
	u16 i = SAMPLE_SIZE;	//ѭ������
	delay_init();
	KEY_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);
	Adc_Init();
	TIM3_Int_Init(100-1, 90-1);	//720kHz�ļ���Ƶ��(Tclk/(psc+1))��arr������90Ϊ8kHz��Tclk==72MHz
	printf("Initialized\r\n");
	
	while(1){
		while(KEY_Scan(0)){
			printf("Start Sampling:\r\n\r\n");
			for(i=0; i<SAMPLE_SIZE;){	//��TIM3�жϸı��־λ��������
				if(adcFlag){
					voiceSample[i] = Get_Adc(ADC_Channel_1);
					adcFlag = 0;
					i++;
				}
			}
			
			for(i=0; i<SAMPLE_SIZE; i++)	//�����������
				printf("%d\r\n", voiceSample[i]);
		}
	}
}
