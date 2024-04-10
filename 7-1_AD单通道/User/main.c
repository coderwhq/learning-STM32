#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "Buzzer.h"
#include "LightSensor.h"
#include "OLED.h"
#include "AD.h"
 
uint16_t ADValue;
float Voltage;


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
	
	OLED_ShowString(1, 1, "ADValue:");
	OLED_ShowString(2, 1, "Voltage:0.00V");
	while(1)
	{
		ADValue = AD_GetValue();
		// ��һ�����Ա任���������ź� 0--4095 ת���� 0--3.3 ������Ϊ��������4095����ȥС�����֣�����Ҫǿ��ת��Ϊfloat
		Voltage = (float)ADValue / 4095 * 3.3; 
		OLED_ShowNum(1, 9, ADValue, 4);
		OLED_ShowNum(2, 9, Voltage, 1);
		OLED_ShowNum(2, 11, (uint16_t)(Voltage * 100) % 100, 2); // ��Ϊ�����Ͳ���ֱ��ȡ������Ҫǿ��ת��
		
		
		Delay_ms(100);
	}
	
	//Buzzer_OFF();
}
