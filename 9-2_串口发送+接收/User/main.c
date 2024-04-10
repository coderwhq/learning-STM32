#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "Buzzer.h"
#include "LightSensor.h"
#include "OLED.h"
#include "Serial.h"

uint8_t KeyNum;
/*
串口通信

串口协议硬件部分：协议规定一个设备使用TX发送高低电平，另一个设备使用RX接受高低电平，

线路中使用TTL，因为STM32是3.3v的器件，所以线路对地是3.3v就说明发送了逻辑1，对地是0v就说明发送了逻辑0

串口协议软件部分：串口中每个字节都装载在一个数据帧内，

串口使用异步通信，所以需要双方设备约定好通信速率（波特率 码元/s、比特率bps bit/s），二进制调制下，一个码元就是一个bit，此时波特率=比特率

起始位：串口的空闲状态（没有数据传输时）引脚置高电平，需要数据传输时必须要先发送一个起始位，起始位必须是低电平，来打破空闲状态，并且告诉另一个设备我要开始发送数据了

停止位：在一个字节数据发送完后，必须要有一个停止位，用于数据帧间隔，固定为高电平，同时也为下一次起始位的下降沿做准备，停止位的长度是可以配置的

数据位：数据帧的有效载荷，地位先行，从右到左

校验位：用于数据帧间隔，固定位高电平，无校验、奇校验、偶校验，发送方会保证1的个数为奇数或偶数，而在校验位补1或0，接收方会验证数据位和校验位，如果1的个数还是原来的奇数或偶数，就认为数据没出错，如果在传输中因干扰，有一位由1变成0了或者由0变成1了，那接收方就认为传输出错，就可以选择丢弃或者重新传。（如果有两位数据同时出错，奇偶校验就检验不出来了，可以了解一下CRC校验）

USART 通用同步异步收发器

USART是STM32内部集成的硬件外设，可根据数据寄存器的一个字节数据自动生成数据帧时序，从TX引脚发送出去，也可自动接收RX引脚的数据帧时序，拼接为一个字节数据，存放在数据寄存器里

自带波特率发生器4.5Mbits/s，是用来配置波特率的，其实就是一个分频器（9600、115200）

可配置数据位长度8/9 、停止位长度0.5/1/1.5/2

可选校验位无校验、奇校验、偶校验

DR数据寄存器，发送数据和接受数据共用一个DR寄存器，当读取DR寄存器的值时，功能为接受数据，当写入DR寄存器的值时，功能为发送数据

波特率=fpclk / (16 * DIV)
*/
uint8_t RxData;
int main(void)
{
	
	OLED_Init();
	Serial_Init();
	//OLED_ShowString(1, 1, "RxData:");
	OLED_ShowString(2, 1, "RxData:");
	while(1)
	{
		/*
			串口接收数据：查询方法
			查询标志位是否置1
		*/
//		if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)
//		{
//			RxData = USART_ReceiveData(USART1); // 对DR寄存器的读操作可以将标志位清零，所以这里不需要手动清零
//			OLED_ShowHexNum(1, 8, RxData, 2);
//		}
		
		/*
			串口接收数据：中断方法
			中断函数在Serial.c文件中，仍然需要模拟查询标志位是否置1的操作
		*/
		
		if(Serial_GetRxFlag() == 1)
		{
			RxData = Serial_GetRxData();
			Serial_SendByte(RxData);
			OLED_ShowHexNum(2, 8, RxData, 2);
		}
	}
	
	//Buzzer_OFF();
}

