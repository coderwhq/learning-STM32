#include "stm32f10x.h"                  // Device header
#include "MySPI.h"
#include "W25Q64_Ins.h"


/**

	写入操作时:
		写入操作前，必须先进行写使能
		每个数据位只能由1改写为0，不能由0改写为1
		写入数据前必须先擦除，擦除后，所有数据位变为1
		擦除必须按最小擦除单元进行
		连续写入多字节时，最多写入一页的数据，超过页尾位置的数据，会回到页首覆盖写入
		写入操作结束后，芯片进入忙状态，不响应新的读写操作

	读取操作时:
		直接调用读取时序，无需使能，无需额外操作，没有页的限制读取操作结束后不会进入忙状态，但不能在忙状态时读取

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
	// 读取返回的状态寄存器
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
// 写入数据
void W25Q64_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count)
{
	uint16_t i;
	
	W25Q64_WriteEnable(); // 写使能
	
	MySPI_Start();
	
	MySPI_SwapByte(W25Q64_PAGE_PROGRAM);
	
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8); // 自动舍去高8位
	MySPI_SwapByte(Address); // 自动舍去高16位
	
	for(i = 0; i < Count; i ++)
	{
		MySPI_SwapByte(DataArray[i]);
	}
	MySPI_Stop();
	// 会自动写失能
	W25Q64_WaitBusy();
}
// 擦除指定地址所在扇区
void W25Q64_SectorErase(uint32_t Address)
{
	W25Q64_WriteEnable(); // 写使能
	MySPI_Start();
	
	MySPI_SwapByte(W25Q64_SECTOR_ERASE_4KB);
	
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8); // 自动舍去高8位
	MySPI_SwapByte(Address); // 自动舍去高16位
	MySPI_Stop();
	// 会自动写失能
	
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
		DataArray[i] = MySPI_SwapByte(W25Q64_DUMMY_BYTE); // 存储区内部的地址指针自动自增
	}	
	MySPI_Stop();
}




