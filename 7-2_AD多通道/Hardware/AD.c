#include "stm32f10x.h"                  // Device header


void AD_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); // ADCCLK = 72M / 6 = 12MHz
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; // 对齐模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 外部触发源选择，本节代码使用软件触发
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; // 独立模式还是双ADC模式？
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; // 连续转换还是单次转换
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; // 扫描模式（多个序列）
	ADC_InitStructure.ADC_NbrOfChannel = 1; // 在扫描模式下会用到的通道数目

	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_Cmd(ADC1, ENABLE);
	
	
	/// 校准ADC
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1) == SET); // 等待校准是否完成
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1) == SET); // 等待校准是否完成
	
	
}

// 单次转换非扫描模式实现AD多通道触发
uint16_t AD_GetValue(uint8_t ADC_Channel)
{
	ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_55Cycles5);
	// 软件触发转换
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	// ADC_FLAG_EOC 该位在规则或注入通道组转换结束时设置，由软件清除或由读取ADC_DR(数据寄存器)时自动清除
	// 采样周期 55.5 + 转换周期 固定为 12.5 = 68T，这个while需要等待 1/12M * 68,,,,12M是对ADCCLK进行6分频后的频率，大约为5.6us
	// 如果使用连续转换模式，那么就不需要判断这个标志位了，因为寄存器的值会不断的刷新最新的转换结果
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); // 转换等待时间
	// 转换完成后：
	
	return ADC_GetConversionValue(ADC1); // 获取转换值，这里获取ADC_DR寄存器的值时，会自动清零ADC_FLAG_EOC标志位
	
	
}


