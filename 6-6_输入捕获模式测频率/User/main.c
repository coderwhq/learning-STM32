#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"
#include "IC.h"

uint8_t i;

/*

	输入捕获：当通道输入引脚出现电平跳变时，当前CNT的值会被锁存在CCR中（测周法）
	可用于测量PWM波形的频率、占空比、脉冲间隔（频率）、电平持续时间（占空比）等参数
	
	可配置PWMI模式，同时测量频率和占空比
	可配置主从触发模式，实现硬件全自动测量
	频率测量：
		测频法 适合测量高频信号 出结果慢 稳定 在闸门时间T内，对上升沿计次得到N	fx = N / T
		测周法 适合测量低频信号 出结果快 波动 两个上升沿内，以标准频率fc计次，得到N，然后取倒数	fx = fc / N
		中界频率：测频法和测周法误差相等的频率点   fm = sqrt(fc / T);
		
	1/1000000 是1Hz的持续时间，时钟周期，当出现上升沿时，会把cnt的值锁存在CCR中，
	用CCR的值乘上这个时钟周期，就是输入频率时钟周期，再取个倒数就是输入频率
*/

int main(void)
{
	
	OLED_Init();
	PWM_Init();
	IC_Init();
	
	OLED_ShowString(1, 1, "Freq:00000Hz");
	OLED_ShowString(2, 1, "Cnt:00000Hz");
	
	PWM_SetPrescaler(720 - 1); 	// Freq = 72M / (PSC + 1) / 100
	PWM_SetCompare1(50);		// Duty = CCR / 100
	
	while(1)
	{
		OLED_ShowNum(1, 6, IC_GetFreq(), 5);
		OLED_ShowNum(2, 5, IC_GetCnt(), 5);
	}
}
