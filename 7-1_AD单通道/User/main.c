#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "Buzzer.h"
#include "LightSensor.h"
#include "OLED.h"
#include "AD.h"
 
uint16_t ADValue;
float Voltage;


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
	
	OLED_ShowString(1, 1, "ADValue:");
	OLED_ShowString(2, 1, "Voltage:0.00V");
	while(1)
	{
		ADValue = AD_GetValue();
		// 做一个线性变换，让数字信号 0--4095 转换成 0--3.3 伏，因为整数整除4095会舍去小数部分，所以要强制转换为float
		Voltage = (float)ADValue / 4095 * 3.3; 
		OLED_ShowNum(1, 9, ADValue, 4);
		OLED_ShowNum(2, 9, Voltage, 1);
		OLED_ShowNum(2, 11, (uint16_t)(Voltage * 100) % 100, 2); // 因为浮点型不能直接取余所以要强制转换
		
		
		Delay_ms(100);
	}
	
	//Buzzer_OFF();
}
