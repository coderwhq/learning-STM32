#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "Buzzer.h"
#include "LightSensor.h"
#include "OLED.h"
#include "CountSensor.h"
#include "Encoder.h"
#include "Timer.h"

int16_t Speed;


/*
Encoder Interface 编码器接口
编码器接口可接收增量(正交)编码器的信号，
根据编码器旋转产生的正交信号脉冲，自动控制CNT自增或自减，从而指示编码器的位置、旋转方向和旋转速度
每个高级定时器和通用定时器都拥有1个编码器接口
两个输入引脚借用了输入捕获的通道1和通道2


本例实现的其实就是 测频法

Tim2 每一秒产生一个触发中断
中断函数每一秒会取出Tim3中，由编码器控制计数的cnt的值，这个值实际上就是输入信号的频率N(Hz/s)

*/

int main(void)
{
	
	OLED_Init();
	Timer_Init();
	Encoder_Init();
	
	OLED_ShowString(1, 1, "Speed:");
	// 模拟编码电机测速
	while(1)
	{
		OLED_ShowSignedNum(1, 7, Speed, 5);
		//Delay_ms(1000);
	}
}
// 每隔一秒触发内部中断测速
void TIM2_IRQHandler(void) 
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Speed = Encouder_Get();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

