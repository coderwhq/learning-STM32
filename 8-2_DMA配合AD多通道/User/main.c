#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "Buzzer.h"
#include "LightSensor.h"
#include "OLED.h"
#include "AD.h"
 
uint16_t AD0, AD1, AD2, AD3;

/*
12位ADC（模数转换器）是一种逐次逼近型模拟数字转换器。它有多达18个通道，可测量16个外部和2个内部信号源。

外部信号就是16个GPIO口

2个内部信号，一个是内部温度传感器（可以测量CPU的温度），一个是内部参考电压（1.2v左右的基准电压）

模拟看门狗自动检测输入电压范围

规则通道只有一个寄存器，如果要多通道转换，为防止DR数据寄存器被覆盖，要结合DMA使用
注入通道有4个寄存器

*/

int main(void)
{
	
	OLED_Init();
	AD_Init();
	
	OLED_ShowString(1, 1, "AD0:");
	OLED_ShowString(2, 1, "AD1:");
	OLED_ShowString(3, 1, "AD2:");
	OLED_ShowString(4, 1, "AD3:");
	
	while(1)
	{
		// 单次转换扫描模式, 单次转运DMA，实现AD多通道触发
		//AD_GetValue();
		
		
		OLED_ShowNum(1, 5, AD_Value[0], 4);
		OLED_ShowNum(2, 5, AD_Value[1], 4);
		OLED_ShowNum(3, 5, AD_Value[2], 4);
		OLED_ShowNum(4, 5, AD_Value[3], 4);
		
		
		Delay_ms(100);
	}
	
	//Buzzer_OFF();
}
