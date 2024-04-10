#include "stm32f10x.h"                  // Device header


void MySPI_W_SS(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)BitValue);
}


void MySPI_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	// 是SCK的时钟频率， 72MHz / 128,如果是SPI2，那么就是32MHz / 128, 因为我们使用的SPI1是APB2的外设，SPI2是APB1的外设
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128; 
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CRCPolynomial = 7; // CRC校验多项式，填默认值7
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; // 数据宽度
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // 配置双线全双工，裁剪引脚的功能
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; // 高位先行还是低位先行, MSB高位先行，LSB低位先行
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; // 这个一般我们自己模拟
	
	SPI_Init(SPI1, &SPI_InitStructure);
	
	SPI_Cmd(SPI1, ENABLE);
	
	MySPI_W_SS(1);
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
	// 硬件SPI必须是发送且同时接收，要想接收必须得提前发送，这样才会生成时序波
	
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) != SET);
	// 一旦移位寄存器有数据了，时序波形就会自动产生，不需要像软件一样，手动调用一些控制时序的函数
	// 并且也不需要手动控制 MOSI 的高低电平，这些硬件都会帮我们实现，我们只需要将数据放到TDR寄存器中就行
	SPI_I2S_SendData(SPI1, ByteSend); 
	
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) != SET);
	
	return SPI_I2S_ReceiveData(SPI1); // 标志位会自动清除
}

