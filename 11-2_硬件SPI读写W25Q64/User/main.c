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
	// 根据W25Q64的注意事项，在写入前必须要擦除
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

同步，全双工，高位先行

主机掌控SCK

有几个从机就会引出几个SS线路，每个从机单独想用一个SS线路，低电平有效

所有SPI设备的SCK、MOSI、MISO分别连在一起

输出引脚配置为推挽输出，输入引脚配置为浮空或上拉输入

SPI有一个规定，当SS为高电平时，那么对应主机的MISO引脚要切换为高阻态（引脚断开，不输出任何电平），当SS为低电平时，对应主机MISO引脚才可以切换为推挽输出

起始条件：SS从高电平切换到低电平

终止条件：SS从低电平切换到高电平

交换字节共有四种模式，通过配置CPOL和CPHA（CPHA时钟相位，决定第一个边沿采样移入还是第二个边沿采样移入，**并不能单独决定上升沿采样还是下降沿采样**，需要和CPOL配合，才能确定上升沿还是下降沿）来决定

00空闲状态SCK为底电平，上升沿采样数据（移入数据），下降沿移除数据，由于SCK默认为低电平，所以第一个到来的电平边沿是上升沿，而我们需要先移除数据才能在上升沿采样数据，所以在SS触发起始条件时，**需要在SCK上升沿到来之前先执行一次移除数据**（数据移入移除的时机比01模式提前半个时钟）

01空闲状态SCK为低电平，上升沿移除数据，下降沿采样数据（移入数据）

10取反00模式的SCK

11 取反01模式的SCK




W25Q64内部的存储区空间划分，这里是认为的划分，并不影响硬件，硬件也意识不到我们的划分

2^10B = 1024B = 1KB

2^10KB = 1024KB = 1MB

2^23B = 8MB

8MB -> 128*block(64KB)

block(64KB) -> 16*sector(4KB)

sector(4KB) -> 16*page(256B)


*/


