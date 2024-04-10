#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "Buzzer.h"
#include "LightSensor.h"
#include "OLED.h"
#include "AD.h"
 
uint16_t AD0, AD1, AD2, AD3;

/*
12λADC��ģ��ת��������һ����αƽ���ģ������ת���������ж��18��ͨ�����ɲ���16���ⲿ��2���ڲ��ź�Դ��

�ⲿ�źž���16��GPIO��

2���ڲ��źţ�һ�����ڲ��¶ȴ����������Բ���CPU���¶ȣ���һ�����ڲ��ο���ѹ��1.2v���ҵĻ�׼��ѹ��

ģ�⿴�Ź��Զ���������ѹ��Χ

����ͨ��ֻ��һ���Ĵ��������Ҫ��ͨ��ת����Ϊ��ֹDR���ݼĴ��������ǣ�Ҫ���DMAʹ��
ע��ͨ����4���Ĵ���

*/

int main(void)
{
	
	OLED_Init();
	AD_Init();
	
	OLED_ShowString(1, 1, "AD0:");
	OLED_ShowString(2, 1, "AD1:");
	OLED_ShowString(3, 1, "AD2:");
	OLED_ShowString(4, 1, "AD3:");
	
	while(1)
	{
		// ����ת��ɨ��ģʽ, ����ת��DMA��ʵ��AD��ͨ������
		//AD_GetValue();
		
		
		OLED_ShowNum(1, 5, AD_Value[0], 4);
		OLED_ShowNum(2, 5, AD_Value[1], 4);
		OLED_ShowNum(3, 5, AD_Value[2], 4);
		OLED_ShowNum(4, 5, AD_Value[3], 4);
		
		
		Delay_ms(100);
	}
	
	//Buzzer_OFF();
}
