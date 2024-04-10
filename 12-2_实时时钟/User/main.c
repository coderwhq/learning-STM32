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
		OLED_ShowNum(4, 6, (32767 - RTC_GetDivider()) / 32767.0 * 999, 10); // 可以利用这个来精确到毫秒微秒
		
	}
	
	//Buzzer_OFF();
}
/*
RTC(实时时钟)

是一个独立的定时器，可为系统提供时钟和日历的功能

RTC和时钟配置系统处于后备区域，系统复位时数据不清零，VDD断电后可借助VBAT供电继续走时

32位的可编程计数器，对应unix的秒计数器 

20位的可编程预分频器，可适配不同频率的输入时钟

可以选择三种RTC时钟源（RTCCLK）

  HSE时钟除以128（通常为8MHz/128）

  LSE振荡器时钟（通常为32.768KHz）（最常用）（只有这个时钟可以通过VBAT电源供电，另外两个在电源断电以后是停止运行的）

  LSI振荡器时钟（40KHz）

  

操作BKP和RTC的注意事项

  设置RCC_APB1ENR的PWREN和BKPEN，使能PWR和BKP时钟

  设置PWR_CR的DBP，使能对BKP和RTC的访问

若在读取RTC寄存器时，RTC的APB1接口曾经处于禁止状态，则软件首先必须等待RTC_CRL寄存器中的RSF位(寄存器同步标志)被硬件置1（初始化时调用一个等待同步的函数就行）

必须设置RTC CRL寄存器中的CNF位，使RTC进入配置模式后，才能写入RTC PRL、RTC CNT、RTC ALR寄存器

对RTC任何寄存器的写操作，都必须在前一次写操作结束后进行。可以通过查询RTC CR寄 存器中的RTOFF状态位，判断RTC寄存器是否处于更新中。仅当RTOFF状态位是1时，才可以写入RTC寄存器
*/

