#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "Buzzer.h"
#include "LightSensor.h"
#include "OLED.h"
#include "MyDMA.h"

/*
DMA direct memory access ֱ�Ӵ洢����ȡ��Э��cpu�������ת�˵Ĺ�����

DMA�����ṩ���裨����洢��DR���ʹ洢���������ڴ�SRAM������洢��Flash���洢���ʹ洢��֮��ĸ������ݴ��䣬����CPU��Ԥ����ʡ��CPU����Դ���Ǹ���ת��

�洢�����洢��������ת��һ��ʹ��������������赽�洢��������ת��һ��ʹ���ض���Ӳ������


ROM ����ʧ�ԣ����粻��ʧ

	0x0800 0000 ����洢��Flash���洢C���Ա����ĳ������

	0x1FFF F000 ϵͳ�洢�����洢BootLoader�����ڴ�������

	0x1FFF F800 ѡ���ֽڣ��洢һЩ�����ڳ����������ò���

RAM ��ʧ�ԣ��粻��ʧ

	0x2000 0000 �����ڴ�SRAM���洢���й����е���ʱ����

	0x4000 0000 ����Ĵ������洢������������ò���

	0xE000 0000 �ں�����Ĵ������洢�ں˸�����������ò���

�������������Ѵ�����������㣬ѭ��ģʽ��������Զ���װ�����Զ��߲���ͬʱʹ�ã����ͬʱ��DMA��ͣ��������

дDMA�Ĵ��������ʱ������Ҫ�ȹر�DMA����д��������DMA����ʱд����������������ֲ�Ĺ涨

*/

//uint8_t aa = 0x66; // ����SRAM��
//const uint8_t aa = 0x66; // ����flash��

uint8_t DataA[] = {0x01, 0x02, 0x03, 0x04};
uint8_t DataB[] = {0, 0, 0, 0};

int main(void)
{
	
	OLED_Init();
	
	MyDMA_Init((uint32_t)DataA, (uint32_t)DataB, 4); // ���������ǵ�ַ, ��Ҫ��ָ������洢����Ҫ��ʾδ���֣�����Ҫǿ������ת��

	
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
