#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "Buzzer.h"
#include "LightSensor.h"
#include "OLED.h"
#include "MyDMA.h"

/*
DMA direct memory access 直接存储器存取，协助cpu完成数据转运的工作。

DMA可以提供外设（外设存储器DR）和存储器（运行内存SRAM，程序存储器Flash）存储器和存储器之间的高速数据传输，无需CPU干预，节省了CPU的资源，是复制转运

存储器到存储器的数据转运一般使用软件触发，外设到存储器的数据转运一般使用特定的硬件触发


ROM 非易失性，掉电不丢失

	0x0800 0000 程序存储器Flash：存储C语言编译后的程序代码

	0x1FFF F000 系统存储器：存储BootLoader，用于串口下载

	0x1FFF F800 选项字节：存储一些独立于程序代码的配置参数

RAM 易失性，电不丢失

	0x2000 0000 运行内存SRAM：存储运行过程中的临时变量

	0x4000 0000 外设寄存器：存储各个外设的配置参数

	0xE000 0000 内核外设寄存器：存储内核各个外设的配置参数

软件触发就是想把传输计数器清零，循环模式是清零后自动重装，所以二者不能同时使用，如果同时用DMA就停不下来了

写DMA的传输计数器时，必须要先关闭DMA才能写，不能在DMA开启时写传输计数器，这是手册的规定

*/

//uint8_t aa = 0x66; // 存在SRAM中
//const uint8_t aa = 0x66; // 存在flash中

uint8_t DataA[] = {0x01, 0x02, 0x03, 0x04};
uint8_t DataB[] = {0, 0, 0, 0};

int main(void)
{
	
	OLED_Init();
	
	MyDMA_Init((uint32_t)DataA, (uint32_t)DataB, 4); // 数组名就是地址, 需要用指针变量存储，若要显示未数字，则需要强制类型转换

	
	OLED_ShowString(1, 1, "DataA");
	OLED_ShowString(3, 1, "DataB");
	OLED_ShowHexNum(1, 8, (uint32_t)DataA, 8);
	OLED_ShowHexNum(3, 8, (uint32_t)DataB, 8);
	

	
//	OLED_ShowChar(1, 1, 'A');
//	OLED_ShowString(1, 3, "HelloWorld!");
//	OLED_ShowNum(2, 1, 12345, 5);
//	OLED_ShowSignedNum(2, 7, -66, 4);
//	OLED_ShowHexNum(3, 1, 0xAA55, 4);
//	OLED_ShowBinNum(4, 1, 0xAA55, 16);
//	OLED_ShowString(1, 6, "Hi XXY");
//	OLED_ShowString(2, 1, "I am a program");
//	OLED_ShowString(3, 1, "by WHQ coding.");
//	OLED_ShowString(4, 1, "I like you. (:");
	
	while(1)
	{
		OLED_ShowHexNum(2, 1, DataA[0], 2);
		OLED_ShowHexNum(2, 4, DataA[1], 2);
		OLED_ShowHexNum(2, 7, DataA[2], 2);
		OLED_ShowHexNum(2, 10, DataA[3], 2);
		
		
		OLED_ShowHexNum(4, 1, DataB[0], 2);
		OLED_ShowHexNum(4, 4, DataB[1], 2);
		OLED_ShowHexNum(4, 7, DataB[2], 2);
		OLED_ShowHexNum(4, 10, DataB[3], 2);
		
		Delay_ms(1000);
		
		MyDMA_Transfer();
		
		OLED_ShowHexNum(2, 1, DataA[0], 2);
		OLED_ShowHexNum(2, 4, DataA[1], 2);
		OLED_ShowHexNum(2, 7, DataA[2], 2);
		OLED_ShowHexNum(2, 10, DataA[3], 2);
		
		
		OLED_ShowHexNum(4, 1, DataB[0], 2);
		OLED_ShowHexNum(4, 4, DataB[1], 2);
		OLED_ShowHexNum(4, 7, DataB[2], 2);
		OLED_ShowHexNum(4, 10, DataB[3], 2);
		
		Delay_ms(1000);
		
		DataA[0] ++;
		DataA[1] ++;
		DataA[2] ++;
		DataA[3] ++;
		
	}
	
	//Buzzer_OFF();
}
