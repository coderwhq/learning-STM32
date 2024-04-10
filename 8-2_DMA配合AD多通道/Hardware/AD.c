#include "stm32f10x.h"                  // Device header

uint16_t AD_Value[4];

void AD_Init(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); // ADCCLK = 72M / 6 = 12MHz
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 配置转换序列表
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);
	
	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; // 对齐模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 外部触发源选择，本节代码使用软件触发
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; // 独立模式还是双ADC模式？
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; // 连续转换还是单次转换
	ADC_InitStructure.ADC_ScanConvMode = ENABLE; // 扫描模式（多个序列）
	ADC_InitStructure.ADC_NbrOfChannel = 4; // 在扫描模式下会用到的通道数目

	ADC_Init(ADC1, &ADC_InitStructure);
	
	// A地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
	// 低16位是ADC1数据，高16位是ADC2数据，这里只用了ADC1, 所以取出低16位
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	
	// B地址          这里想存储到SRAM中
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)AD_Value;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	
	// A地址为源，B地址为目的地  A --->> B
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; // 传输方向
	DMA_InitStructure.DMA_BufferSize = 4; // 传输计数器
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; // 传输模式 是否使用自动重装
	// Mermoy to Mermory 就是软件触发，也叫存储器到存储器模式(使能)，，，，，，外设到存储器的转运模式是硬件触发(失能)
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; // 软件触发 还是 硬件触发
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; // 优先级

	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	// DMA启动的三个条件 1.计数器不为0 2.DMA使能 3.触发源有信号
	DMA_Cmd(DMA1_Channel1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);
	
	
	/// 校准ADC
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1) == SET); // 等待校准是否完成
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1) == SET); // 等待校准是否完成
	
	// 如果配置了 连续转换扫描模式, 循环转运DMA，实现AD多通道触发 则可以直接在这里触发转换
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

// 单次转换扫描模式, 单次转运DMA，实现AD多通道触发
void AD_GetValue(void)
{
	// 单个通道转换完成就会触发DMA请求
	
	DMA_Cmd(DMA1_Channel1, DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel1, 4);
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	// 软件触发转换
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);
	// 标志位需要手动清除
	DMA_ClearFlag(DMA1_FLAG_TC1);
	
}


