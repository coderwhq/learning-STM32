#include "stm32f10x.h"   // Device header

extern uint16_t Num; // extern 关键字意味着有个外部变量 Num，让编译器自动去其他文件里寻找

/*
中断 TIM  --->  NVIC  --->  CPU
时钟周期=频率的倒数

发射一个脉冲所用的时间叫做**时钟周期**
一个**机器周期**等于多个时钟周期，指完成一次具有一定功能所需要的最短时间

72MHz  1s内可以发出 72000000 个脉冲信号，1一个脉冲信号为1Hz

72MHz的时钟周期为1s/72MHz = 1us/72Hz      us 对应 M   ms 对应 K

所以如果计72个数，则需要72个脉冲，耗费时间为 1/72 * 72 = 1us

所以如果计72000个数则耗费时间为1/72 * 72000 = 1ms



16的计数器CNT、预分频器PSC(16位)、自动重装寄存器ARR(16位) 三个一起称为 时基单元

预分频器：对计数器的时钟进行分频，让计数更加灵活 0一分频 1二分频
自动重装寄存器是：计数的目标值，就是我想要计多少个时钟申请中断

2^16 * 2^16 = 可记录的最大脉冲数N，N / (每秒震荡次数)72MHz = 最大计时 59.65s

 

CK_CNT：进入计数器的频率
CK_PSC：进入预分频器的频率
PSC：预分频器的参数
ARR：自动重装寄存器的参数

计数器计数频率：CK_CNT = CK_PSC / (PSC + 1)

计数器溢出频率：CK_CNT_OV = CK_CNT / (ARR + 1) = CK_PSC / (PSC + 1) / (ARR + 1)
溢出频率＝计数频率/(重装目标值+1) 
*/
void Timer_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/******* 1.打开APB1中的TIM2时钟 *******/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	/******* 2.选择内部时钟 *******/
	TIM_InternalClockConfig(TIM2); // 内部时钟
	/******* 3.配置时基单元，开启时钟中断通道 *******/
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;  // 指定时钟分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	/*
		首先要知道单位 s 与 Hz 是相对应的
		要实现一秒一计数也就是 
		CK_PSC = 72MHz
		(ARR + 1) * (PSC + 1) = CK_PSC 
		1Hz = CS_PSC / (PSC + 1) / (ARR + 1)
		比如：
		TIM_Period：10000 - 1
		TIM_Prescaler：7200 - 1
		理解：将内部时钟系统的1秒内发来的 72MHz 进行 7200分频，这样进入计数器的频率CK_CNT就是
		72MHz / 7200 = 10000Hz，这个10000Hz意味着一秒内会有10000个脉冲信号发送给计数器，
		也就是计数器会执行10000次计数，那么再设置自动重装寄存器为 10000，
		也就是当计数器加到10000时，执行中断函数，而最后算出来的这个 1Hz 就是 触发中断的频率
		在中断函数内执行Second++的操作，这样就可以实现每一秒钟发送一次中断函数
	*/
	TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1; // 自动重装寄存器
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1; // 预分频器
	
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; // 高级定时器才有
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);  // 初始化时基单元
	/* 为什么要清除标志位？
		因为BaseInit 的最后一行代码为了让我们设置的分频器立刻生效，手动生成了一个更新事件，这样预分频器的值就有效了
		但是触发了更新事件，就会触发更新中断，更新中断会置更新中断标志位，所以一旦初始化完了以后，就会立刻进入中断事件
		所以清除一下标志位就行
	*/
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // 配置定时器的中断输出控制，使能和选择通道
	
	/******* 4.配置NVIC中断通道 *******/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	
	NVIC_Init(&NVIC_InitStructure);
	
	/*******5.启动定时器*******/
	TIM_Cmd(TIM2, ENABLE); // 用来使能计数器
	
}


// TIM2 的中断函数
//void TIM2_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
//	{
//		Num ++;
//		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//	}
//}

