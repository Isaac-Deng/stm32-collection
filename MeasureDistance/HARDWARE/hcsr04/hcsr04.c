#include "hcsr04.h"
#include "delay.h"

//���������
//�Ľ�˼·��
//���ȴ����տڸߵ�ƽ�����ʹ���ж�
//���ȡ����ʱ�䳬��4�׷�Χ����ʱ������ֹͣ����
//������ʽ�Ż�������С��
//��ȥ������С���� OK

#define HCSR04_PORT     GPIOB
#define HCSR04_CLK      RCC_APB2Periph_GPIOB
#define HCSR04_TRIG     GPIO_Pin_5	//������ƽ����PB.5
#define HCSR04_ECHO     GPIO_Pin_6	//

#define TRIG_Send  PBout(5) 	//λ������
#define ECHO_Reci  PBin(6)


u16 msHcCount = 0;//ms����

void Hcsr04Init()
{  
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;     //�������ڶ�ʱ�����õĽṹ��
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(HCSR04_CLK, ENABLE);
     
        //IO��ʼ��
    GPIO_InitStructure.GPIO_Pin = HCSR04_TRIG;       //���͵�ƽ����
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
    GPIO_Init(HCSR04_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(HCSR04_PORT,HCSR04_TRIG);
     
    GPIO_InitStructure.GPIO_Pin =   HCSR04_ECHO;     //���ص�ƽ����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		//��������(ԭ��������)
    GPIO_Init(HCSR04_PORT, &GPIO_InitStructure);  
    GPIO_ResetBits(HCSR04_PORT,HCSR04_ECHO);    
     
    //��ʱ����ʼ�� ʹ�û�����ʱ��TIM6
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);   //ʹ�ܶ�ӦRCCʱ��
    //���ö�ʱ�������ṹ��
    TIM_DeInit(TIM2);
    TIM_TimeBaseStructure.TIM_Period = (1000-1); //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ         ������1000Ϊ1ms
    TIM_TimeBaseStructure.TIM_Prescaler =(72-1); //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  1M�ļ���Ƶ�� 1US����
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;//����Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ         
        
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);   //��������жϣ����һ���ж����������ж�
    TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);    //�򿪶�ʱ�������ж�
    hcsr04_NVIC();
    TIM_Cmd(TIM6,DISABLE);     
}


//tips��static����������������ڶ�������Դ�ļ��ڣ����Բ���Ҫ��ͷ�ļ�������
static void OpenTimerForHc()        //�򿪶�ʱ��
{
    TIM_SetCounter(TIM6,0);//�������
    msHcCount = 0;
    TIM_Cmd(TIM6, ENABLE);  //ʹ��TIMx����
}
 
static void CloseTimerForHc()        //�رն�ʱ��
{
    TIM_Cmd(TIM6, DISABLE);  //ʹ��TIM6����
}
 
 
//NVIC����
void hcsr04_NVIC()
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;             //ѡ��ʱ��6�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռʽ�ж����ȼ�����Ϊ0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         //��Ӧʽ�ж����ȼ�����Ϊ0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;        //ʹ���ж�
    NVIC_Init(&NVIC_InitStructure);
}


//��ʱ��6�жϷ������
void TIM6_IRQHandler(void)   //TIM6�ж�
{
    if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)  //���TIM6�����жϷ������
    {
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update  );  //���TIM6�����жϱ�־ 
        msHcCount++;
    }
}
 

//��ȡ��ʱ��ʱ��
u32 GetEchoTimer(void)
{
    u32 t = 0;
    t = msHcCount*1000;//�õ�MS
    t += TIM_GetCounter(TIM6);//�õ�US
    TIM6->CNT = 0;  //��TIM2�����Ĵ����ļ���ֵ����
    delay_ms(10);	//����? ��ʱ�ȴ���ʱ�����㣿
    return t;
}
 

//һ�λ�ȡ������������� ���β��֮����Ҫ���һ��ʱ�䣬���ϻ����ź�
//(Ϊ�����������Ӱ�죬ȡ 5 �����ݵ�ƽ��ֵ���м�Ȩ�˲���)���ɼ��٣�
float Hcsr04GetLength(void )
{
    u32 t = 0;
    float lengthTemp = 0;

		TRIG_Send = 1;      //���Ϳڸߵ�ƽ���
		delay_us(10);				//�ο��ĵ�Ϊ10us
    TRIG_Send = 0;
    while(ECHO_Reci == 0);      //�ȴ����տڸߵ�ƽ���		//ʹ���жϼ���?
    OpenTimerForHc();        //�򿪶�ʱ��

    while(ECHO_Reci == 1);
    CloseTimerForHc();        //�رն�ʱ��
    t = GetEchoTimer();        //��ȡʱ��,�ֱ���Ϊ1US
    lengthTemp = ((float)t/58.0);//cm �������Եó�?
		if(lengthTemp >= 400.0 || lengthTemp <= 2.0)
				return 0;
		else
				return lengthTemp;
}

