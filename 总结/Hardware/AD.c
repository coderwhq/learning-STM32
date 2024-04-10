#include "stm32f10x.h"

uint16_t AD_Value[4];

void AD_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); // ADCCLK = 72M / 6 = 12MHz
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 配置转换序列表?????????
	/*
		如果不配置这个，并不知道你使用了哪几个ADC1的通道，配置了才能和上面打开的GPIO口一一对应起来
		配置这个还可以规定每个通道处于转换序列表的位置
	*/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);
	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; // 连续模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE; // 扫描模式
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; // 对齐方式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 软件触发
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; // 单双ADC模式
	ADC_InitStructure.ADC_NbrOfChannel = 4; // 通道数
	
	ADC_Init(ADC1, &ADC_InitStructure);
	// A地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	// B地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)AD_Value;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	
	DMA_InitStructure.DMA_BufferSize = 4; // 传输计数器
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; // 属于硬件触发
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// 传输模式，是否自动重装
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; 

	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	DMA_Cmd(DMA1_Channel1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE); // 开启ADC发送DMA请求的功能
	ADC_Cmd(ADC1, ENABLE);

	/// 校准ADC????????????????
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1) == SET); // 等待校准是否完成
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1) == SET); // 等待校准是否完成

	// 如果配置了 连续转换扫描模式, 循环转运DMA，实现AD多通道触发 则可以直接在这里触发转换
	//ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

// 单次转换扫描模式, 单次转运DMA，实现AD多通道触发
void AD_GetValue(void)
{
	// 手册规定需要先禁用，在配置计数器，然后再启用
	DMA_Cmd(DMA1_Channel1, DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel1, 4);
	DMA_Cmd(DMA1_Channel1, ENABLE);
	// 软件触发转换
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);
	DMA_ClearFlag(DMA1_FLAG_TC1);
}


