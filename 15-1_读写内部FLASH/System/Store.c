#include "stm32f10x.h"                  // Device header
#include "MyFLASH.h"

// 可以修改编译器中的程序存储地址，以防程序过大覆盖我们要存储数据的地方
/*
	如果在主函数中直接操作FLASH，这样不太方便，而且因为要先擦除在写入，所以容易丢失数据
	
	使用SRAM数组与闪存空间互相映射的方法
	
	主函数，只需要修改SRAM中的数组Store_Data（但SRAM断电会丢失），之后Store模块会将Store_Data整体备份到闪存中（断电不丢失）
	每次上电时，在将上次在闪存中保存的数据初始化恢复到SRAM数组中，这样就可以供程序访问。

*/

#define STORE_START_ADDRESS		0x0800FC00
#define STORE_COUNT				512

uint16_t Store_Data[STORE_COUNT]; // 512个，每个数据两个字节，刚好对应闪存的一页1024字节

void Store_Init(void)
{
	uint16_t i;
	// 判断闪存之前是否保存过数据，0xA5A5是自定的标志位，存在0x0800FC00上
	if(MyFLASH_ReadHalfWord(STORE_START_ADDRESS) != 0xA5A5)
	{
		// 没有标志位，说明之前没保存过数据，所以要先进行擦除操作，然后写入标志位，在将每一位初始化为0
		MyFLASH_ErasePage(STORE_START_ADDRESS);
		MyFLASH_ProgramHalfWord(STORE_START_ADDRESS, 0xA5A5);
		
		for(i = 1; i < STORE_COUNT; i ++) // i=0的位置存的是标志位
		{
			MyFLASH_ProgramHalfWord(STORE_START_ADDRESS + i * 2, 0x0000);
		}
	}
	// 上电时，把闪存备份的数据，恢复到SRAM数组中，，，上电同步闪存和SRAM的数据
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
	Store_Save(); // 清零后更新到闪存中
}

