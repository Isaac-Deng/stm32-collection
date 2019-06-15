#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"

								    
//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��PORTAʱ��

	//��ʼ�� WK_UP-->GPIOA.0	  ��������
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0���ó����룬Ĭ������	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.0

}

//���������������İ���(��һ������)
//mode:0,��֧��������;1,֧��������;
//���ذ���ֵ
//0��û���κΰ�������
//1��KEY3���� WK_UP
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&WK_UP==0)
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(WK_UP==0)	return WKUP_PRES;
	}else if(WK_UP==1)	key_up=1; 	    
 	return 0;// �ް�������
}
