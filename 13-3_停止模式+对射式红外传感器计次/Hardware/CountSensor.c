#include "stm32f10x.h"                  // Device header

uint16_t CountSensor_Count;

void CountSensor_Init(void)
{
	
	/*
		GPIOx_Pin_(1-16)外设   --->   AFIO(选择引脚)   --->   EXTI    --->    NVIC
	*/
	
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	// EXTI 和 NVIC(是内核的外设) 的时钟是一直打开的不需要我们开启,RCC管理的是内核外的外设
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	/* 
		AFIO 中断引脚选择，就是一个数据选择器
		可以在Pin引脚相同（PA0 PB0 PC0....）的众多引脚里选择其中一个链接到后面的EXTI通道 
	
		在STM32中AFIO主要完成两个任务：复用功能引脚重映射、中断引脚选择
	*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// 函数内部在操作AFIO
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
	
	/*
		EXTI外部中断 Extern Interrupt 边沿检测及控制
		EXTI可以检测指定GPIO口的电平信号，当其指定的GPIO口产生电平变化时，
		EXTI发出中断申请，经NVIC裁决后即可中断CPU主程序，使CPU执行EXTI对应的中断程序
		
		EXTI支持触发引脚电平的变化的方式为：上升沿/下降沿/双边沿/软件触发(程序执行一句代码就可以触发中断)
		
		支持所有的GPIO口，Pin引脚相同的不能同时触发中断，比如PA0 PB0 PC0不能同时触发
		
		输入：16个GPIO_Pin、PVD输出、RTC闹钟、USB唤醒、以太网唤醒，共20个中断线路
		输出：11个通向NVIC(EXTI0、1、2、3、4，EXTI9_5，EXTI15_10，PVD，RTC，USB，以太网)、20个通向其他外设(事件响应)
		
		触发响应的方式：中断响应（通向CPU）/事件响应（不会触发中断，触发别的外设操作，属于外设之间的联合工作）
	*/
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line14; // EXTI的第14个线路
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; // 开启
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; // 中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // 下降沿触发
	
	EXTI_Init(&EXTI_InitStructure);
	
	
	/*
		NVIC嵌套中断向量控制器，用于统一分配中断优先级和管理中断，是一个内核外设，cpu的秘书
		NVIC有很多的输入口，可以接入EXTI、TIM、ADC、USART……，一个外设还可能会同时占用多个中断通道，
		比如上述的EXTI占用了 11 个中断通道
		NVIC只有一个输出口通向CPU
	*/
	
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 分组方式整个芯片只能用一种，所以整个项目只需要执行一次这个代码即可，可以放在主函数内执行
	// 配置通道的使能或失能和通道的优先级
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	// 抢占优先级高的可以中断嵌套，响应优先级高的可以优先排队，均相同的按照中断号排队，
	// PreemptionPriority抢占优先级，SubPriority响应优先级
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
类似回调函数
*/
void EXTI15_10_IRQHandler(void)
{
	// 中断函数中不要执行耗时过长的代码，也最好不要操作硬件 和 不要和主函数调用相同的函数
	if(EXTI_GetITStatus(EXTI_Line14) == SET)
	{
		CountSensor_Count ++;
		// 中断程序结束后一定要再调用一下清除中断标志位的函数
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
}



