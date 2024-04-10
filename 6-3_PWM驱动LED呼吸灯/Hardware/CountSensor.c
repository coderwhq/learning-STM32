#include "stm32f10x.h"                  // Device header

uint16_t CountSensor_Count;

void CountSensor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	// EXTI 和 NVIC(是内核的外设) 的时钟是一直打开的不需要我们开启,RCC管理的内核外的外设
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line14; // EXTI的第14个线路
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; // 开启中断
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; // 中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // 下降沿触发
	
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 分组方式整个芯片只能用一种，所以整个项目只需要执行一次这个代码即可，可以放在主函数内执行
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	
	NVIC_Init(&NVIC_InitStructure);

}

uint16_t CountSensor_Get(void)
{
	return CountSensor_Count;
}

/**
这个中断函数无需手动调用，当发生中断时系统会自动调用此函数
中断函数都是无参无返回值的
在stm32中每个中断函数都是固定的，每个中断通道都对应一个中断函数，中断函数名可以参考启动文件
*/
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line14) == SET)
	{
		CountSensor_Count ++;
		// 中断程序结束后一定要再调用一下清除中断标志位的函数
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
}



