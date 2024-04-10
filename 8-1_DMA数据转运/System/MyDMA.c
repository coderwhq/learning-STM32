#include "stm32f10x.h"                  // Device header

uint16_t MyDMA_Size;

void MyDMA_Init(uint32_t AddrA, uint32_t AddrB, uint16_t Size)
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	MyDMA_Size = Size;
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = AddrA;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;	
	DMA_InitStructure.DMA_MemoryBaseAddr = AddrB;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; // 传输方向
	DMA_InitStructure.DMA_BufferSize = Size; // 传输计数器
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; // 传输模式 是否使用自动重装
	// Mermoy to Mermory 就是软件触发，也叫存储器到存储器模式，，，，，，外设到存储器的转运模式是硬件触发
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable; // 软件触发 还是 硬件触发
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; // 优先级

	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	DMA_Cmd(DMA1_Channel1, DISABLE);
}

void MyDMA_Transfer(void)
{
	DMA_Cmd(DMA1_Channel1, DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel1, MyDMA_Size);
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	// 等待转换完成，转换完成后会置标志位，跳出循环
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);
	// 标志位需要手动清除
	DMA_ClearFlag(DMA1_FLAG_TC1);
	
}
