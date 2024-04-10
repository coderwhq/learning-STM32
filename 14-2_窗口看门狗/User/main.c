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
	// 如果独立看门狗已经由硬件选项或软件启动，LSI振荡器将被强制在打开状态，并且不能被关闭。在LSI振荡器稳定后，时钟供应给IWDG。
	OLED_Init();
	
	OLED_ShowString(1, 1, "WWDG TEST");
	
	if(RCC_GetFlagStatus(RCC_FLAG_WWDGRST) == SET)
	{
		// 独立看门狗触发复位
		
		OLED_ShowString(2, 1, "WWDGRST");
		Delay_ms(500);
		OLED_ShowString(2, 1, "       ");
		Delay_ms(100);
		
		RCC_ClearFlag();
	}
	else
	{
		// 按键触发复位
		OLED_ShowString(3, 1, "RST");
		Delay_ms(500);
		OLED_ShowString(3, 1, "   ");
		Delay_ms(100);
	}
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
	
	// 超时时间 50ms，窗口时间30ms
	
	// 超时时间 Twwdg = Tpckl1 * 4096 * WDGTB预分频系数 * (T[5:0] + 1)
	// 窗口时间 Twin = Tpckl1 * 4096 * WDGTB预分频系数 * (T[5:0] - W[5:0])
	// Tpckl1 = 1 / Fpckl1
	
	WWDG_SetPrescaler(WWDG_Prescaler_8);
	WWDG_SetWindowValue(0x40 | 21); // 窗口时间
	WWDG_Enable(0x40 | 54); // 这也相当于一次喂狗操作，我们只需要写入T[6:0]位最高位WDGA会在这个函数内部被置1，即打开看门狗
	
	while(1)
	{
		Key_GetNum();
				
		//OLED_ShowString(4, 1, "FEED");
		Delay_ms(20);  
		//OLED_ShowString(4, 1, "    ");
		Delay_ms(20); 
		// 为了和第一次喂狗之间间隔的时间不能太快，需要再最后在喂狗
		WWDG_SetCounter(0x40 | 54); // 函数内部在喂狗时将最高位WDGA置0，但是不用担心看门狗会被关闭，根据手册描述，看门狗一旦打开后就无法关闭
	}
}

/*

WDG(Watchdog)看门狗

看门狗可以监控程序的运行状态，当程序因为设计漏洞、硬件故障电磁干扰等原因，出现卡死或跑飞现象时，看门狗能及时复位程序避免程序陷入长时间的罢工状态，保证系统的可靠性和安全性

看门狗本质上是一个定时器，当指定时间范围内，程序没有执行喂狗(重置计数器)操作时，看门狗硬件电路就自动产生复位信号
STM32内置两个看门狗

  独立看门狗(IWDG)：独立工作，对时间精度要求较低

  窗口看门狗(WWDG)：使用APB1的时钟，没有专用的时钟，不算是独立，要求看门狗在精确计时窗口起作用

*/


