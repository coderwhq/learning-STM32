#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "Buzzer.h"
#include "LightSensor.h"
#include "OLED.h"

uint8_t KeyNum;

uint16_t ArrayWrite[] = {0x1234, 0x5678};
uint16_t ArrayRead[2];	

/*
BKP(备份寄存器)

在STM32断电后，只要VBAT仍然有外部电源供电，数据就不会丢失，并且系统复位后，BKP的数据也不会复位，所以BKP并不能真正实现掉电不丢失，需要VBAT引脚提供备用电源来维持，当主电源有电时，VBAT不会用到，会用主电源供电，这样可以节省电池电量

BKP中包含VBAT

BKP中包含TAMPER引脚产生的侵入事件将所有备份寄存器内容清除（防拆功能）

BKP中包含RTC时钟校准寄存器

三个共用一个引脚
*/

int main(void)
{
	
	OLED_Init();				//OLED初始化
	Key_Init();	
	
	OLED_ShowString(1, 1, "W:");
	OLED_ShowString(2, 1, "R:");
	
	/*
		通过设置寄存器RCC_APB1ENR的PWREN和BKPEN位来打开电源和后备接口的时钟
		电源控制寄存器(PWR_CR)的DBP位来使能对后备寄存器和RTC的访问。
	*/
	// 这三步是为了符合使用BPK的注意事项
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
	
	PWR_BackupAccessCmd(ENABLE);
	
	while(1)
	{
		KeyNum = Key_GetNum();		//获取按键键码
		
		if (KeyNum == 1)			//按键1按下
		{
			ArrayWrite[0] ++;		//测试数据自增
			ArrayWrite[1] ++;
			
			BKP_WriteBackupRegister(BKP_DR1, ArrayWrite[0]);	//写入测试数据到备份寄存器
			BKP_WriteBackupRegister(BKP_DR2, ArrayWrite[1]);
			
			OLED_ShowHexNum(1, 3, ArrayWrite[0], 4);		//显示写入的测试数据
			OLED_ShowHexNum(1, 8, ArrayWrite[1], 4);
		}
		
		ArrayRead[0] = BKP_ReadBackupRegister(BKP_DR1);		//读取备份寄存器的数据
		ArrayRead[1] = BKP_ReadBackupRegister(BKP_DR2);
		
		OLED_ShowHexNum(2, 3, ArrayRead[0], 4);				//显示读取的备份寄存器数据
		OLED_ShowHexNum(2, 8, ArrayRead[1], 4);
	}
	
	//Buzzer_OFF();
}
