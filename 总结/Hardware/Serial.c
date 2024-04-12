#include "stm32f10x.h"
#include <stdarg.h>
#include <stdio.h>
/**

只能发送一个字节，不能接收一个字节

固定HEX数据包发送(长度为4)
固定HEX数据包接收(长度为4) 数据包包头 FF 包尾 FE

发送字符串
接收字符串  包头 @ 包尾 \r\n

可直接发送十进制数字

内部封装了printf函数，当多个串口想使用printf时，可以使用Serial_Printf这个函数

**/
uint8_t Serial_RxFlag; // 外界通过这个标志位来判断是否受到了数据

uint8_t Serial_TxPacket[4];
uint8_t Serial_RxPacket[4];
char Serial_RxString[100];

void Serial_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 硬件流控，暂时不知道是干嘛的
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; // 使能发送和接受模式

	USART_Init(USART1, &USART_InitStructure);
	// 发送不太需要关注，重点需要关注接收，接收数据需要触发中断，在中断函数中处理数据
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1, ENABLE);
}

// 发送一个字节
void Serial_SendByte(uint8_t Byte)
{
	// 因为是硬件控制USART，所以内部会实现起始位和结束位，我们不需要做处理
	USART_SendData(USART1, Byte); 
	// 等待字节发送完毕，另外不需要手动清零，对DR的写操作(再次发送)将自动清零
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) != SET){};
}

void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for(i = 0; i < Length; i ++)
	{
		 Serial_SendByte(Array[i]);
	}
}
// 发送可变数组数据包
void Serial_SendHexPacket(void)
{
	Serial_SendByte(0xFF); // 包头
	Serial_SendArray(Serial_TxPacket, 4);
	Serial_SendByte(0xFE); // 包尾
}

void Serial_SendString(char* String)
{
	uint8_t i;
	for(i = 0; String[i] != '\0'; i ++)
	{
		Serial_SendByte((uint8_t)String[i]);
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
// 将数字转换成字符串发送出去
void Serial_SendNumber(uint32_t Number)
{
	char offset = '0';
	uint8_t i, Length = 0;
	uint32_t Temp = Number;
	while(Temp != 0)
	{
		Length ++;
		Temp /= 10;
	}
	for(i = 1; i <= Length; i ++)
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - i) % 10 + offset);
	}
}

uint8_t Serial_GetReceiveFlag(void)
{
	uint8_t TempFlag = 0;
	
	if(Serial_RxFlag == 1)
	{
		Serial_RxFlag = 0;
		TempFlag = 1;
	}
		
	return TempFlag;
}


// 重写 stdio.h 中的 fputc 函数，fputc是printf函数的底层，这里相当于将printf函数重定向到了串口
int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}

// 当多个串口外设都想使用printf时，可以用这个函数，封装，串口使用sprintf 的过程//////相关知识点：C语言可变参数列表
void Serial_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(String);
}


// 接收数据时触发中断
void USART1_IRQHandler(void)
{
	// 这个静态变量类似全局变量，函数进入只会初始化一次0，与全局变量不同的是，静态变量只能在被定义时的定义域内使用
	// 为了避免发送数据中也存在包头包尾的情况，我们引入状态机进行编码
	static uint8_t RxState = 0; // 0 等待接受到包头  1 接受数据  2等待接受包尾
	static uint8_t pRxPacket = 0;
	static uint8_t Serial_RxHexFlag = 0;
	static uint8_t Serial_RxStringFlag = 0;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		uint8_t RxData = USART_ReceiveData(USART1);
		
		if(RxState == 0)
		{
			if(RxData == 0xFF)
			{
				RxState = 1;	// 状态机变换为接收数据的状态
				pRxPacket = 0;
				Serial_RxHexFlag = 1; // 识别到发送的为 Digital数据包 的包头，置接下来接受的为 Digital数据包的数据 的标志位为1
			}
			else if(RxData == '@')
			{
				RxState = 1;	// 状态机变换为接收数据的状态
				pRxPacket = 0;
				Serial_RxStringFlag = 1;
			}
		}
		else if(RxState == 1)
		{
			if(Serial_RxHexFlag == 1)
			{
				Serial_RxPacket[pRxPacket ++] = RxData;
				if(pRxPacket >= 4)
				{
					pRxPacket = 0;
					RxState = 2;
				}
			}
			else if(Serial_RxStringFlag == 1)
			{
				if(RxData == '\r')
				{
					RxState = 2;
				}
				else
				{
					Serial_RxString[pRxPacket ++] = RxData;
				}
			}
		}
		else if(RxState == 2)
		{
			if(Serial_RxHexFlag == 1)
			{
				if(RxData == 0xFE)
				{	
					RxState = 0;
					Serial_RxHexFlag = 0;
					Serial_RxFlag = 1; // 已经接受到数据
				}
			}
			else if(Serial_RxStringFlag == 1)
			{
				if(RxData == '\n')
				{	
					RxState = 0;
					Serial_RxString[pRxPacket ++] = '\0';
					Serial_RxStringFlag = 0;
					Serial_RxFlag = 1; // 已经接受到数据
				}
			}
		}
				
	}
}

