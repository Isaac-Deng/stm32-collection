#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"


#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//��ȡ����3(WK_UP) 

#define WKUP_PRES   4	//KEY_UP����(��WK_UP/KEY_UP)


void KEY_Init(void);//IO��ʼ��
u8 KEY_Scan(u8);  	//����ɨ�躯��					    
#endif
