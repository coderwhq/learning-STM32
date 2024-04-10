#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "Buzzer.h"
#include "LightSensor.h"
#include "OLED.h"
#include "Serial.h"

uint8_t KeyNum;

uint8_t RxData;
int main(void)
{
	
	OLED_Init();
	Serial_Init();
	OLED_ShowString(1, 1, "RxData:");
	// OLED_ShowString(2, 1, "RxData:");
	while(1)
	{
		
		if(Serial_GetRxFlag() == 1)
		{
			RxData = Serial_GetRxData();
			Serial_SendByte(RxData);
			OLED_ShowHexNum(1, 8, RxData, 2);
		}
		
		
		OLED_ShowString(2, 1, "Running");
		Delay_ms(100);
		OLED_ShowString(2, 1, "       ");
		Delay_ms(100);
		
		__WFI(); // �жϻ���˯��ģʽ,,,,,,,,CPU�ϵ磬�������ڹ�����USART��
		//__WFE(); // �¼�����˯��ģʽ
	}
	
	//Buzzer_OFF();
}

