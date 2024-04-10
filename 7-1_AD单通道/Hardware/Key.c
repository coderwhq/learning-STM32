#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "key.h"

void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(KEY_GPIO_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN | KEY2_GPIO_PIN;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(KEY_GPIO_PORT, &GPIO_InitStructure);
	
}
// ��ȡ�Ƿ���
uint8_t Key_GetNum(void)
{
	uint8_t KeyNum = 0;
	
	if(GPIO_ReadInputDataBit(KEY_GPIO_PORT, KEY1_GPIO_PIN) == KEY_ON) // ����
	{
		Delay_ms(20); // ��������
		while(GPIO_ReadInputDataBit(KEY_GPIO_PORT, KEY1_GPIO_PIN) == KEY_ON); // �������һֱ���£��������￨ס
		Delay_ms(20); // �ɿ�����
		KeyNum = 1;
	}
	
	if(GPIO_ReadInputDataBit(KEY_GPIO_PORT, KEY2_GPIO_PIN) == KEY_ON) // ����   
	{
		Delay_ms(20); // ��������
		while(GPIO_ReadInputDataBit(KEY_GPIO_PORT, KEY2_GPIO_PIN) == KEY_ON); // �������һֱ���£��������￨ס
		Delay_ms(20); // �ɿ�����
		KeyNum = 2;
	} 
	
	return KeyNum;
}

