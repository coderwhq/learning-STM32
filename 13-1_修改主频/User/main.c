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
	
	OLED_Init();
	OLED_ShowString(1, 1, "SYSCLK:");
	OLED_ShowNum(1, 8, SystemCoreClock, 8);
	
	while(1)
	{
		OLED_ShowString(2, 1, "Running");
		Delay_ms(500); // ��Ϊdelay�����ڲ�Ĭ��ʹ�õ���72M��Ƶ�����Ե�����Ƶ��Ϊ32Mʱ��Delay�ͻᷭһ��
		OLED_ShowString(2, 1, "       ");
		Delay_ms(500);
	}
	
	//Buzzer_OFF();
}
/*
PWR

PWR(Power Control)��Դ����

PWR�������STM32�ڲ��ĵ�Դ���粿�֣�����ʵ�ֿɱ�̵�ѹ������͵͹���ģʽ�Ĺ���

�ɱ�̵�ѹ�����(PVD)���Լ��VDD��Դ��ѹ����VDD�½���PVD��ֵ���»�������PVD��ֵ֮��ʱ��PVD�ᴥ���жϣ�����ִ�н����ر�����

�͹���ģʽ����˯��ģʽ(Sleep)��ͣ��ģʽ(Stop)�ʹ���ģʽ(Standby)������ϵͳ����ʱ������STM32�Ĺ��ģ��ӳ��豸ʹ��ʱ��
*/

