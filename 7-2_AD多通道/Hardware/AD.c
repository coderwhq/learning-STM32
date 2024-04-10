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
	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; // ����ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // �ⲿ����Դѡ�񣬱��ڴ���ʹ���������
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; // ����ģʽ����˫ADCģʽ��
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; // ����ת�����ǵ���ת��
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; // ɨ��ģʽ��������У�
	ADC_InitStructure.ADC_NbrOfChannel = 1; // ��ɨ��ģʽ�»��õ���ͨ����Ŀ

	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_Cmd(ADC1, ENABLE);
	
	
	/// У׼ADC
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1) == SET); // �ȴ�У׼�Ƿ����
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1) == SET); // �ȴ�У׼�Ƿ����
	
	
}

// ����ת����ɨ��ģʽʵ��AD��ͨ������
uint16_t AD_GetValue(uint8_t ADC_Channel)
{
	ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_55Cycles5);
	// �������ת��
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	// ADC_FLAG_EOC ��λ�ڹ����ע��ͨ����ת������ʱ���ã������������ɶ�ȡADC_DR(���ݼĴ���)ʱ�Զ����
	// �������� 55.5 + ת������ �̶�Ϊ 12.5 = 68T�����while��Ҫ�ȴ� 1/12M * 68,,,,12M�Ƕ�ADCCLK����6��Ƶ���Ƶ�ʣ���ԼΪ5.6us
	// ���ʹ������ת��ģʽ����ô�Ͳ���Ҫ�ж������־λ�ˣ���Ϊ�Ĵ�����ֵ�᲻�ϵ�ˢ�����µ�ת�����
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); // ת���ȴ�ʱ��
	// ת����ɺ�
	
	return ADC_GetConversionValue(ADC1); // ��ȡת��ֵ�������ȡADC_DR�Ĵ�����ֵʱ�����Զ�����ADC_FLAG_EOC��־λ
	
	
}


