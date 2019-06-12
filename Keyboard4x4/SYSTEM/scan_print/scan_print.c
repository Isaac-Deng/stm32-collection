#include "scan_print.h"
#include "exti.h"
#include "usart.h"

u8 row = 0;

u16 GPIOE_Pinx[4] = {GPIO_Pin_0, GPIO_Pin_2, GPIO_Pin_4, GPIO_Pin_6};
u16 GPIOF_Pinx[4] = {GPIO_Pin_0, GPIO_Pin_2, GPIO_Pin_4, GPIO_Pin_6};
u8 keyBoard[4][4] = {'1', '2', '3', 'A', 
										 '4', '5', '6', 'B', 
										 '7', '8', '9', 'C', 
										 '*', '0', '#', 'D'};

void scan_init(void)
{
	//��E������ߵ�ƽ���ȴ��ж��ź�
	for(row = 0; row < 4; row++)
		GPIO_SetBits(GPIOE, GPIOE_Pinx[row]);
}

u8 scan_Key(u8 col)
{	
	//����E�����
	for(row = 0; row < 4; row++)
		GPIO_ResetBits(GPIOE, GPIOE_Pinx[row]);
	
	//ɨ�����E(row)��ȷ��������
	for(row = 0; row < 4; row++){
		GPIO_SetBits(GPIOE, GPIOE_Pinx[row]);
		if(GPIO_ReadInputDataBit(GPIOF, GPIOF_Pinx[col])){	//��ȡЧ������
			return keyBoard[row][col];
		}else{
			GPIO_ResetBits(GPIOE, GPIOE_Pinx[row]);
		}
	}
	return 'E';
}

void scan_print(u8 col)
{
	printf("%c\r\n", scan_Key(col));
}
