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
	
	OLED_Init();
	MyRTC_Init();
	
	OLED_ShowString(1, 1, "Date:XXXX-XX-XX");
	OLED_ShowString(2, 1, "Time:XX:XX:XX");
	OLED_ShowString(3, 1, "CNT :");
	OLED_ShowString(4, 1, "DIV :");
	while(1)
	{
		MyRTC_ReadTime();
		
		OLED_ShowNum(1, 6, MyRTC_Time[0], 4);
		OLED_ShowNum(1, 11, MyRTC_Time[1], 2);
		OLED_ShowNum(1, 14, MyRTC_Time[2], 2);
		OLED_ShowNum(2, 6, MyRTC_Time[3], 2);
		OLED_ShowNum(2, 9, MyRTC_Time[4], 2);
		OLED_ShowNum(2, 12, MyRTC_Time[5], 2);		
		
		OLED_ShowNum(3, 6, RTC_GetCounter(), 10);
		OLED_ShowNum(4, 6, (32767 - RTC_GetDivider()) / 32767.0 * 999, 10); // ���������������ȷ������΢��
		
	}
	
	//Buzzer_OFF();
}
/*
RTC(ʵʱʱ��)

��һ�������Ķ�ʱ������Ϊϵͳ�ṩʱ�Ӻ������Ĺ���

RTC��ʱ������ϵͳ���ں�����ϵͳ��λʱ���ݲ����㣬VDD�ϵ��ɽ���VBAT���������ʱ

32λ�Ŀɱ�̼���������Ӧunix��������� 

20λ�Ŀɱ��Ԥ��Ƶ���������䲻ͬƵ�ʵ�����ʱ��

����ѡ������RTCʱ��Դ��RTCCLK��

  HSEʱ�ӳ���128��ͨ��Ϊ8MHz/128��

  LSE����ʱ�ӣ�ͨ��Ϊ32.768KHz������ã���ֻ�����ʱ�ӿ���ͨ��VBAT��Դ���磬���������ڵ�Դ�ϵ��Ժ���ֹͣ���еģ�

  LSI����ʱ�ӣ�40KHz��

  

����BKP��RTC��ע������

  ����RCC_APB1ENR��PWREN��BKPEN��ʹ��PWR��BKPʱ��

  ����PWR_CR��DBP��ʹ�ܶ�BKP��RTC�ķ���

���ڶ�ȡRTC�Ĵ���ʱ��RTC��APB1�ӿ��������ڽ�ֹ״̬����������ȱ���ȴ�RTC_CRL�Ĵ����е�RSFλ(�Ĵ���ͬ����־)��Ӳ����1����ʼ��ʱ����һ���ȴ�ͬ���ĺ������У�

��������RTC CRL�Ĵ����е�CNFλ��ʹRTC��������ģʽ�󣬲���д��RTC PRL��RTC CNT��RTC ALR�Ĵ���

��RTC�κμĴ�����д��������������ǰһ��д������������С�����ͨ����ѯRTC CR�� �����е�RTOFF״̬λ���ж�RTC�Ĵ����Ƿ��ڸ����С�����RTOFF״̬λ��1ʱ���ſ���д��RTC�Ĵ���
*/

