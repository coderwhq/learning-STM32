#include "stm32f10x.h"                  // Device header
#include "MyFLASH.h"

// �����޸ı������еĳ���洢��ַ���Է�������󸲸�����Ҫ�洢���ݵĵط�
/*
	�������������ֱ�Ӳ���FLASH��������̫���㣬������ΪҪ�Ȳ�����д�룬�������׶�ʧ����
	
	ʹ��SRAM����������ռ以��ӳ��ķ���
	
	��������ֻ��Ҫ�޸�SRAM�е�����Store_Data����SRAM�ϵ�ᶪʧ����֮��Storeģ��ὫStore_Data���屸�ݵ������У��ϵ粻��ʧ��
	ÿ���ϵ�ʱ���ڽ��ϴ��������б�������ݳ�ʼ���ָ���SRAM�����У������Ϳ��Թ�������ʡ�

*/

#define STORE_START_ADDRESS		0x0800FC00
#define STORE_COUNT				512

uint16_t Store_Data[STORE_COUNT]; // 512����ÿ�����������ֽڣ��պö�Ӧ�����һҳ1024�ֽ�

void Store_Init(void)
{
	uint16_t i;
	// �ж�����֮ǰ�Ƿ񱣴�����ݣ�0xA5A5���Զ��ı�־λ������0x0800FC00��
	if(MyFLASH_ReadHalfWord(STORE_START_ADDRESS) != 0xA5A5)
	{
		// û�б�־λ��˵��֮ǰû��������ݣ�����Ҫ�Ƚ��в���������Ȼ��д���־λ���ڽ�ÿһλ��ʼ��Ϊ0
		MyFLASH_ErasePage(STORE_START_ADDRESS);
		MyFLASH_ProgramHalfWord(STORE_START_ADDRESS, 0xA5A5);
		
		for(i = 1; i < STORE_COUNT; i ++) // i=0��λ�ô���Ǳ�־λ
		{
			MyFLASH_ProgramHalfWord(STORE_START_ADDRESS + i * 2, 0x0000);
		}
	}
	// �ϵ�ʱ�������汸�ݵ����ݣ��ָ���SRAM�����У������ϵ�ͬ�������SRAM������
	for(i = 0; i < STORE_COUNT; i ++)
	{
		Store_Data[i] = MyFLASH_ReadHalfWord(STORE_START_ADDRESS + i * 2);
	}
	
}

void Store_Save(void)
{
	uint16_t i;
	MyFLASH_ErasePage(STORE_START_ADDRESS);
	
	for(i = 0; i < STORE_COUNT; i ++)
	{
		MyFLASH_ProgramHalfWord(STORE_START_ADDRESS + i * 2, Store_Data[i]);
	}
}

void Store_Clear(void)
{
	uint16_t i;
	for(i = 1; i < STORE_COUNT; i ++)
	{
		Store_Data[i] = 0x0000;
	}
	Store_Save(); // �������µ�������
}

