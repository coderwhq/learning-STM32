#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "Buzzer.h"
#include "LightSensor.h"
#include "OLED.h"
#include "CountSensor.h"
uint8_t KeyNum;

int main(void)
{
	
	OLED_Init();
	CountSensor_Init();
	
	OLED_ShowString(1, 1, "Count:");
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	
	
	while(1)
	{
		OLED_ShowNum(1, 7, CountSensor_Get(), 5);
		
		OLED_ShowString(2, 1, "Running");
		Delay_ms(100);
		OLED_ShowString(2, 1, "       ");
		Delay_ms(100);
		
		PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI); // �ڲ���ֱ�ӵ��� WFI �� WFE
		/*
			�˳�ֹͣģʽʱ��HSI��ѡΪϵͳʱ�ӣ�8M
		*/
		
		SystemInit(); // ���»���ϵͳ��Ƶ
	}
	
	//Buzzer_OFF();
}
