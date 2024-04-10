#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "Buzzer.h"
#include "LightSensor.h"

uint8_t KeyNum;

int main(void)
{
	LED_Init();
	Buzzer_Init();
	LightSensorInit();

	
	while(1)
	{
		// Ϊʲô������ʹ�����������turn�������޷�ʵ�ֹ���
		
		// �������贫�����ڲ�����������ģʽ�����Զ˿ڳ�ʼΪ1�����������й�ʱ������Ϊ0������Ϊ0
		// ���ⰵʱΪ0������ߵ�ƽ�����Ǵ������Դ���������ĸߵ�ƽ������ʱΪ1������͵�ƽ���������ڲ�mos�ܴ򿪣����Žӵ͵�ƽ���Ϊ0
		if(LightSensor_Get() == 1) // �ߵ�ƽ������Դ���ڵ�ʱ�����ƿ���������
		{
//			Buzzer_Turn();
//			LED_R_Turn();
			LED_G_ON();
//			Buzzer_ON();
		}
		else
		{
//			Buzzer_Turn(); 
//			LED_R_Turn();
			LED_G_OFF();
//			Buzzer_OFF();
		}
	}
	
	//Buzzer_OFF();
}
