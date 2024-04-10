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
	
	OLED_ShowString(1, 1, "WWDG TEST");
	
	if(RCC_GetFlagStatus(RCC_FLAG_WWDGRST) == SET)
	{
		// �������Ź�������λ
		
		OLED_ShowString(2, 1, "WWDGRST");
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
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
	
	// ��ʱʱ�� 50ms������ʱ��30ms
	
	// ��ʱʱ�� Twwdg = Tpckl1 * 4096 * WDGTBԤ��Ƶϵ�� * (T[5:0] + 1)
	// ����ʱ�� Twin = Tpckl1 * 4096 * WDGTBԤ��Ƶϵ�� * (T[5:0] - W[5:0])
	// Tpckl1 = 1 / Fpckl1
	
	WWDG_SetPrescaler(WWDG_Prescaler_8);
	WWDG_SetWindowValue(0x40 | 21); // ����ʱ��
	WWDG_Enable(0x40 | 54); // ��Ҳ�൱��һ��ι������������ֻ��Ҫд��T[6:0]λ���λWDGA������������ڲ�����1�����򿪿��Ź�
	
	while(1)
	{
		Key_GetNum();
				
		//OLED_ShowString(4, 1, "FEED");
		Delay_ms(20);  
		//OLED_ShowString(4, 1, "    ");
		Delay_ms(20); 
		// Ϊ�˺͵�һ��ι��֮������ʱ�䲻��̫�죬��Ҫ�������ι��
		WWDG_SetCounter(0x40 | 54); // �����ڲ���ι��ʱ�����λWDGA��0�����ǲ��õ��Ŀ��Ź��ᱻ�رգ������ֲ����������Ź�һ���򿪺���޷��ر�
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


