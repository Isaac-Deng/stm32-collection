#include "sys.h"

void SelectionSort(u32 *num,u32 n)
{
  u32 i = 0;
  u32 min = 0;
  u32 j = 0;
  u32 tmp = 0;
  for(i = 0;i < n-1;i++)
  {
 	  min = i;//ÿ�ν�min�ó���������ʼλ��Ԫ���±� 
    for(j = i;j < n;j++)//���������飬�ҵ���СԪ�ء� 
    {
	    if(num[min]>num[j])
	  {
	  min = j;
	  }
	}
	if(min != i)//�����СԪ�ز�����������ʼλ��Ԫ�أ�������ʼԪ�ؽ���λ�� 
	{
	  tmp = num[min];
	  num[min] = num[i];
	  num[i] = tmp;
	}
 }
}
