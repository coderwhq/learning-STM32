#include "stm32f10x.h"                  // Device header
#include "MySPI.h"
#include "W25Q64_Ins.h"


/**

	д�����ʱ:
		д�����ǰ�������Ƚ���дʹ��
		ÿ������λֻ����1��дΪ0��������0��дΪ1
		д������ǰ�����Ȳ�������������������λ��Ϊ1
		�������밴��С������Ԫ����
		����д����ֽ�ʱ�����д��һҳ�����ݣ�����ҳβλ�õ����ݣ���ص�ҳ�׸���д��
		д�����������оƬ����æ״̬������Ӧ�µĶ�д����

	��ȡ����ʱ:
		ֱ�ӵ��ö�ȡʱ������ʹ�ܣ�������������û��ҳ�����ƶ�ȡ���������󲻻����æ״̬����������æ״̬ʱ��ȡ

*/

void W25Q64_Init(void)
{
	MySPI_Init();

}

void W25Q64_ReadID(uint8_t *MID, uint16_t *DID)
{
	MySPI_Start();
	MySPI_SwapByte(W25Q64_JEDEC_ID);
	*MID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	*DID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	*DID <<= 8;
	*DID |= MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	MySPI_Stop();
}

void W25Q64_WriteEnable(void)
{
	MySPI_Start();
	MySPI_SwapByte(W25Q64_WRITE_ENABLE);
	MySPI_Stop();
}

void W25Q64_WaitBusy(void)
{
	uint32_t Timeout = 100000;
	MySPI_Start();
	MySPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);
	// ��ȡ���ص�״̬�Ĵ���
	while((MySPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01) == 0x01)
	{
		Timeout --;
		if(Timeout == 0)
		{
			break;
		}
	}
	MySPI_Stop();
}
// д������
void W25Q64_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count)
{
	uint16_t i;
	
	W25Q64_WriteEnable(); // дʹ��
	
	MySPI_Start();
	
	MySPI_SwapByte(W25Q64_PAGE_PROGRAM);
	
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8); // �Զ���ȥ��8λ
	MySPI_SwapByte(Address); // �Զ���ȥ��16λ
	
	for(i = 0; i < Count; i ++)
	{
		MySPI_SwapByte(DataArray[i]);
	}
	MySPI_Stop();
	// ���Զ�дʧ��
	W25Q64_WaitBusy();
}
// ����ָ����ַ��������
void W25Q64_SectorErase(uint32_t Address)
{
	W25Q64_WriteEnable(); // дʹ��
	MySPI_Start();
	
	MySPI_SwapByte(W25Q64_SECTOR_ERASE_4KB);
	
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8); // �Զ���ȥ��8λ
	MySPI_SwapByte(Address); // �Զ���ȥ��16λ
	MySPI_Stop();
	// ���Զ�дʧ��
	
	W25Q64_WaitBusy();
}


void W25Q64_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count)
{
	uint32_t i;
	MySPI_Start();
	
	MySPI_SwapByte(W25Q64_READ_DATA);
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);
	
	for(i = 0; i < Count; i ++)
	{
		DataArray[i] = MySPI_SwapByte(W25Q64_DUMMY_BYTE); // �洢���ڲ��ĵ�ַָ���Զ�����
	}	
	MySPI_Stop();
}




