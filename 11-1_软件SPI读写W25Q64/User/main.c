#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "Buzzer.h"
#include "LightSensor.h"
#include "OLED.h"
#include "W25Q64.h"
uint8_t KeyNum;

uint8_t MID;
uint16_t DID;

uint8_t ArrayWrite[] = {0x55, 0xB2, 0xC3, 0xD4};
uint8_t ArrayRead[4];

int main(void)
{
	
	OLED_Init();
	
	W25Q64_Init();
	
	OLED_ShowString(1, 1, "MID:   DID:");
	OLED_ShowString(2, 1, "W:");
	OLED_ShowString(3, 1, "R:");
	
	W25Q64_ReadID(&MID, &DID);
	OLED_ShowHexNum(1, 5, MID, 2);
	OLED_ShowHexNum(1, 12, DID, 4);
	// ����W25Q64��ע�������д��ǰ����Ҫ����
	W25Q64_SectorErase(0x000000);
	W25Q64_PageProgram(0x000000, ArrayWrite, 4);
	
	W25Q64_ReadData(0x000000, ArrayRead, 4);
	
	OLED_ShowHexNum(2, 3, ArrayWrite[0], 2);
	OLED_ShowHexNum(2, 6, ArrayWrite[1], 2);
	OLED_ShowHexNum(2, 9, ArrayWrite[2], 2);
	OLED_ShowHexNum(2, 12, ArrayWrite[3], 2);
	
	OLED_ShowHexNum(3, 3, ArrayRead[0], 2);
	OLED_ShowHexNum(3, 6, ArrayRead[1], 2);
	OLED_ShowHexNum(3, 9, ArrayRead[2], 2);
	OLED_ShowHexNum(3, 12, ArrayRead[3], 2);
	
	while(1)
	{

	}
	
	//Buzzer_OFF();
}
/*

SPI

ͬ����ȫ˫������λ����

�����ƿ�SCK

�м����ӻ��ͻ���������SS��·��ÿ���ӻ���������һ��SS��·���͵�ƽ��Ч

����SPI�豸��SCK��MOSI��MISO�ֱ�����һ��

�����������Ϊ���������������������Ϊ���ջ���������

SPI��һ���涨����SSΪ�ߵ�ƽʱ����ô��Ӧ������MISO����Ҫ�л�Ϊ����̬�����ŶϿ���������κε�ƽ������SSΪ�͵�ƽʱ����Ӧ����MISO���Ųſ����л�Ϊ�������

��ʼ������SS�Ӹߵ�ƽ�л����͵�ƽ

��ֹ������SS�ӵ͵�ƽ�л����ߵ�ƽ

�����ֽڹ�������ģʽ��ͨ������CPOL��CPHA��CPHAʱ����λ��������һ�����ز������뻹�ǵڶ������ز������룬**�����ܵ������������ز��������½��ز���**����Ҫ��CPOL��ϣ�����ȷ�������ػ����½��أ�������

00����״̬SCKΪ�׵�ƽ�������ز������ݣ��������ݣ����½����Ƴ����ݣ�����SCKĬ��Ϊ�͵�ƽ�����Ե�һ�������ĵ�ƽ�����������أ���������Ҫ���Ƴ����ݲ����������ز������ݣ�������SS������ʼ����ʱ��**��Ҫ��SCK�����ص���֮ǰ��ִ��һ���Ƴ�����**�����������Ƴ���ʱ����01ģʽ��ǰ���ʱ�ӣ�

01����״̬SCKΪ�͵�ƽ���������Ƴ����ݣ��½��ز������ݣ��������ݣ�

10ȡ��00ģʽ��SCK

11 ȡ��01ģʽ��SCK




W25Q64�ڲ��Ĵ洢���ռ仮�֣���������Ϊ�Ļ��֣�����Ӱ��Ӳ����Ӳ��Ҳ��ʶ�������ǵĻ���

2^10B = 1024B = 1KB

2^10KB = 1024KB = 1MB

2^23B = 8MB

8MB -> 128*block(64KB)

block(64KB) -> 16*sector(4KB)

sector(4KB) -> 16*page(256B)


*/


