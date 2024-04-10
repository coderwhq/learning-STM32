#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "Buzzer.h"
#include "LightSensor.h"
#include "OLED.h"
uint8_t KeyNum;

int main(void)
{
	// ����������Ź��Ѿ���Ӳ��ѡ������������LSI��������ǿ���ڴ�״̬�����Ҳ��ܱ��رա���LSI�����ȶ���ʱ�ӹ�Ӧ��IWDG��
	OLED_Init();
	
	OLED_ShowString(1, 1, "IWDG TEST");
	
	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == SET)
	{
		// �������Ź�������λ
		
		OLED_ShowString(2, 1, "IWDGRST");
		Delay_ms(500);
		OLED_ShowString(2, 1, "       ");
		Delay_ms(100);
		
		RCC_ClearFlag();
	}
	else
	{
		// ����������λ
		OLED_ShowString(3, 1, "RST");
		Delay_ms(500);
		OLED_ShowString(3, 1, "   ");
		Delay_ms(100);
	}
	
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); // ���д����
	// ��ʱʱ��1s		Tiwdg = Tlsi * PRԤ��Ƶϵ�� * (RL + 1)			Tlsi = 1 / Flsi = 1 / 40KHz = 0.025ms
	IWDG_SetPrescaler(IWDG_Prescaler_16);
	IWDG_SetReload(2500 - 1);
	IWDG_ReloadCounter(); // ��ǰιһ�¹�����֤��һ��ι�����ھ���1s���Ͻ�һ��
	IWDG_Enable(); // ι����ʹ��ʱ���ͻ�����Ĵ���д��0x5555֮���ֵ���������ǲ���Ҫ�ֶ�ִ��д����
	while(1)
	{
		Key_GetNum();
		
		IWDG_ReloadCounter();
		
		OLED_ShowString(4, 1, "FEED");
		Delay_ms(200);  
		OLED_ShowString(4, 1, "    ");
		Delay_ms(600); 
	}
}

/*

WDG(Watchdog)���Ź�

���Ź����Լ�س��������״̬����������Ϊ���©����Ӳ�����ϵ�Ÿ��ŵ�ԭ�򣬳��ֿ������ܷ�����ʱ�����Ź��ܼ�ʱ��λ�������������볤ʱ��İչ�״̬����֤ϵͳ�Ŀɿ��ԺͰ�ȫ��

���Ź���������һ����ʱ������ָ��ʱ�䷶Χ�ڣ�����û��ִ��ι��(���ü�����)����ʱ�����Ź�Ӳ����·���Զ�������λ�ź�
STM32�����������Ź�

  �������Ź�(IWDG)��������������ʱ�侫��Ҫ��ϵ�

  ���ڿ��Ź�(WWDG)��ʹ��APB1��ʱ�ӣ�û��ר�õ�ʱ�ӣ������Ƕ�����Ҫ���Ź��ھ�ȷ��ʱ����������

*/


