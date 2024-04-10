#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

/*
	可变包长，含包头包尾
	包头为@，载荷数据不固定，包尾为\r\n
*/
char Serial_RxPacket[100]; // 存储文本数据包的数组
uint8_t Serial_RxFlag; // 是否接收完成的标志位

void Serial_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	// 输出引脚TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	// 接收引脚 RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	USART_InitStructure.USART_BaudRate = 9600; // 波特率，直接写就行，内部会自动帮忙计算9600对应的分频系数，然后写入BRR寄存器
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 硬件流控
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // 或上接收模式即可
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	
	USART_Init(USART1, &USART_InitStructure);
	
	// 打开USART中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	// RXNE标志位 一旦置1了，就会向NVIC申请中断
	
	// 配置NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	
	NVIC_Init(&NVIC_InitStructure);
	
	
	USART_Cmd(USART1, ENABLE);
}



void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	// 在库函数的状态寄存器中有一句话：对USART_DR的写操作，将该位清零。
	// 所以当我们下一次在SendData时，它会自动清零，那么我们就不需要手动清零了
}


void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for(i = 0; i < Length; i ++)
	{
		Serial_SendByte(Array[i]);
	}
}

void Serial_SendString(char *String)
{
	
	uint8_t i;
	for(i = 0; String[i] != '\0'; i ++)
	{
		Serial_SendByte(String[i]);
	}
	
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while(Y --)
	{
		Result *= X;
	}
	return Result;
}

void Serial_SendNumber(uint32_t Number, uint8_t Length)
{

	uint8_t i;
	char offset = '0';

	for(i = 0; i < Length; i ++)
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - 1 - i) % 10 + offset);
	}
	
}

// 重写 stdio.h 中的 fputc 函数，fputc是printf函数的底层，这里相当于将printf函数重定向到了串口
int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}


// 封装 串口使用sprintf 的过程//////相关知识点：C语言可变参数列表
void Serial_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(String);
}

// 判断是否接受到了数据包
//uint8_t Serial_GetRxFlag(void)
//{
//	if(Serial_RxFlag == 1)
//	{
//		Serial_RxFlag = 0;
//		return 1;
//	}
//	return 0;
//}



// 每收到一个字节都会进入这个中断，通过判断状态机的状态，来决定如何操作本次传入的字节
void USART1_IRQHandler(void)
{
	// 这个静态变量类似全局变量，函数进入只会初始化一次0，与全局变量不同的是，静态变量只能在本函数使用
	static uint8_t RxState = 0;
	static uint8_t pRxPacket = 0; // 当前存储字节的数量
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		uint8_t RxData = USART_ReceiveData(USART1);
		// 需要用elseif否则在状态转移时（在RxState=0时置RxState = 1）下面的if会立刻生效
		if(RxState == 0)
		{
			// 防止数据包发的太快来不及处理，导致数据包错位，这里需要判断Flag为0时才接收数据
			if(RxData == '@' && Serial_RxFlag == 0)
			{
				RxState = 1;
				pRxPacket = 0;// 提前清零pRxPacket
			}
		}
		else if(RxState == 1)
		{
			// 因为数据长度不固定所以需要先判断是否是包尾
			if(RxData == '\r')
			{
				RxState = 2;
			}
			else
			{
				Serial_RxPacket[pRxPacket ++ ] = RxData;
			}
		}
		else if(RxState == 2)
		{
			if(RxData == '\n')
			{
				RxState = 0;
				Serial_RxPacket[pRxPacket ++ ] = '\0'; // 在结尾加个\0，构成完整的字符串方便外界直接操作字符串
				Serial_RxFlag = 1; // 一个数据包接受完成，置一个标志位告诉外界
			}
		}
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
	
}

