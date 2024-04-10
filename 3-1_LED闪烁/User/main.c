#include "stm32f10x.h"                  // Device header
#include "Delay.h"

int main(void)
{
//	RCC->APB2ENR = 0x00000010;
//	GPIOC->CRH = 0x00300000;
//	GPIOC->ODR = 0x00002000; 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
	//GPIO_ResetBits(GPIOA, GPIO_Pin_1);
	//GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
	//GPIO_Write();
	
//	while(1)
//	{
//		GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);
//		Delay_ms(500);
//		GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
//		Delay_ms(500);
//	}
}
