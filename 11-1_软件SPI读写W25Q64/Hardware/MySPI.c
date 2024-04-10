#include "stm32f10x.h"                  // Device header


void MySPI_W_SS(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)BitValue);
}

void MySPI_W_SCK(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction)BitValue);
}

void MySPI_W_MOSI(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_7, (BitAction)BitValue);
}

uint8_t MySPI_R_MISO(void)
{
	return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6);
}


void MySPI_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 初始化的默认电平
	MySPI_W_SS(1);
	MySPI_W_SCK(0);
	
}


void MySPI_Start(void)
{
	MySPI_W_SS(0);
}

void MySPI_Stop(void)
{
	MySPI_W_SS(1);
}
// 模式0 交换字节
uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
	uint8_t i;
	for(i = 0; i < 8; i ++)
	{
		// 要保证MySPI_W_MOSI这个函数是非0即1的特征才能这么写，否则要用if判断或者右移到最低位
		MySPI_W_MOSI(ByteSend & 0x80);
		ByteSend <<= 1;
		/****在此时，从机也移出了一个数据位，放在MISO上****/
		MySPI_W_SCK(1);
		/****在此时，从机会把MOSI上的数据读走****/
		ByteSend |= MySPI_R_MISO();
		MySPI_W_SCK(0);
	}
	return ByteSend;
}

