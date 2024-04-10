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
	
	// ����ת�����б�
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);
	
	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; // ����ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // �ⲿ����Դѡ�񣬱��ڴ���ʹ���������
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; // ����ģʽ����˫ADCģʽ��
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; // ����ת�����ǵ���ת��
	ADC_InitStructure.ADC_ScanConvMode = ENABLE; // ɨ��ģʽ��������У�
	ADC_InitStructure.ADC_NbrOfChannel = 4; // ��ɨ��ģʽ�»��õ���ͨ����Ŀ

	ADC_Init(ADC1, &ADC_InitStructure);
	
	// A��ַ
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
	// ��16λ��ADC1���ݣ���16λ��ADC2���ݣ�����ֻ����ADC1, ����ȡ����16λ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	
	// B��ַ          ������洢��SRAM��
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)AD_Value;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	
	// A��ַΪԴ��B��ַΪĿ�ĵ�  A --->> B
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; // ���䷽��
	DMA_InitStructure.DMA_BufferSize = 4; // ���������
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; // ����ģʽ �Ƿ�ʹ���Զ���װ
	// Mermoy to Mermory �������������Ҳ�д洢�����洢��ģʽ(ʹ��)���������������赽�洢����ת��ģʽ��Ӳ������(ʧ��)
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; // ������� ���� Ӳ������
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; // ���ȼ�

	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	// DMA�������������� 1.��������Ϊ0 2.DMAʹ�� 3.����Դ���ź�
	DMA_Cmd(DMA1_Channel1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);
	
	
	/// У׼ADC
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1) == SET); // �ȴ�У׼�Ƿ����
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1) == SET); // �ȴ�У׼�Ƿ����
	
	// ��������� ����ת��ɨ��ģʽ, ѭ��ת��DMA��ʵ��AD��ͨ������ �����ֱ�������ﴥ��ת��
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

// ����ת��ɨ��ģʽ, ����ת��DMA��ʵ��AD��ͨ������
void AD_GetValue(void)
{
	// ����ͨ��ת����ɾͻᴥ��DMA����
	
	DMA_Cmd(DMA1_Channel1, DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel1, 4);
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	// �������ת��
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);
	// ��־λ��Ҫ�ֶ����
	DMA_ClearFlag(DMA1_FLAG_TC1);
	
}


