#include "stm32f10x.h"                  // Device header
#include "Delay.h"

int main(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3; // 按位或运算
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//GPIO_ResetBits(GPIOA, GPIO_Pin_0);
	while(1)
	{
		//GPIO_Write(GPIOB, ~(GPIO_Pin_5 | GPIO_Pin_0 | GPIO_Pin_1));
		//Delay_ms(500);
		//GPIO_Write(GPIOB, 0xFFFF);
		//Delay_ms(500);
		GPIO_Write(GPIOA, ~0x0001);
		Delay_ms(500);
		GPIO_Write(GPIOA, ~0x0002);
		Delay_ms(500);
		GPIO_Write(GPIOA, ~0x0004);
		Delay_ms(500);
		GPIO_Write(GPIOA, ~0x0008);
		Delay_ms(500);
	}
	GPIO_Write(GPIOA, 0xFFFF);
}
