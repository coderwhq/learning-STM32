#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "Buzzer.h"
#include "LightSensor.h"
#include "OLED.h"
#include "MyRTC.h"

uint8_t KeyNum;

int main(void)
{
	uint32_t Alarm;
	OLED_Init();
	MyRTC_Init();
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	
	
	OLED_ShowString(1, 1, "CNT :");
	OLED_ShowString(2, 1, "ALR :");
	OLED_ShowString(3, 1, "ALRF:");
	
	PWR_WakeUpPinCmd(ENABLE);// �ֲ�����˵��ʹ��WAKEUP���ź����ű�ǿ������Ϊ��������ģʽ�����Բ���Ҫ��ʼ��GPIO
	
	Alarm = RTC_GetCounter() + 10;
	RTC_SetAlarm(Alarm); // ��������
	OLED_ShowNum(2, 6, Alarm, 10);
	
	while(1)
	{	
		OLED_ShowNum(1, 6, RTC_GetCounter(), 10);
		OLED_ShowNum(3, 6, RTC_GetFlagStatus(RTC_FLAG_ALR), 1);
		OLED_ShowString(4, 1, "Running");
		Delay_ms(100);
		OLED_ShowString(4, 1, "       ");
		Delay_ms(100);
		
		OLED_ShowString(4, 9, "STANDBY");
		Delay_ms(1000);
		OLED_ShowString(4, 9, "       ");
		Delay_ms(100);
		
		// �ڽ������ģʽ֮ǰҪ�ر���������
		OLED_Clear();
		
		PWR_EnterSTANDBYMode(); // ����ģʽ��ͨ�������¼����ѣ����Ѻ�����ͷ��ʼ����
	}
	
	//Buzzer_OFF();
}
