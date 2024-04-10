#include "stm32f10x.h"                  // Device header
#include <time.h>
uint16_t MyRTC_Time[] = {2023, 1, 1, 23, 59, 55};

void MyRTC_SetTime(void);

void MyRTC_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	
	PWR_BackupAccessCmd(ENABLE);
	
	// 防止时间初始化
	if(BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
	{
	
		RCC_LSEConfig(RCC_LSE_ON);

		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) != SET); // 等待开启时钟成功
		
		
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
		RCC_RTCCLKCmd(ENABLE);
		
		RTC_WaitForSynchro(); // 等待时钟同步
		RTC_WaitForLastTask(); // 等待上一次写入完成
		
		
		RTC_SetPrescaler(32768 - 1); // 32768分频           函数内部会自动调用进入配置模式和退出配置模式的代码
		RTC_WaitForLastTask(); // 等待上一次写入完成
		
		
		//RTC_SetCounter(1672588795);
		//RTC_WaitForLastTask(); // 等待上一次写入完成
		MyRTC_SetTime();
		
		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
	}
	else
	{
		RTC_WaitForSynchro(); // 等待时钟同步
		RTC_WaitForLastTask(); // 等待上一次写入完成
	}
	
}

void MyRTC_SetTime(void)
{
	time_t time_cnt;
	struct tm time_date;
	
	time_date.tm_year = MyRTC_Time[0] - 1900;
	time_date.tm_mon = MyRTC_Time[1] - 1;
	time_date.tm_mday = MyRTC_Time[2];
	time_date.tm_hour = MyRTC_Time[3];
	time_date.tm_min = MyRTC_Time[4];
	time_date.tm_sec = MyRTC_Time[5];
	
	time_cnt = mktime(&time_date) - 8 * 60 * 60;	// 减去时区偏移，以设置北京时间为基准，所以是-
	
	
	RTC_SetCounter(time_cnt);
	RTC_WaitForLastTask(); // 等待上一次写入完成
}

void MyRTC_ReadTime(void)
{
	time_t time_cnt;
	struct tm time_date;
	
	time_cnt = RTC_GetCounter() + 8 * 60 * 60; // 加上时区偏移
	
	time_date = *localtime(&time_cnt);
	
	MyRTC_Time[0] = time_date.tm_year + 1900;
	MyRTC_Time[1] = time_date.tm_mon + 1;
	MyRTC_Time[2] = time_date.tm_mday;
	MyRTC_Time[3] = time_date.tm_hour;
	MyRTC_Time[4] = time_date.tm_min;
	MyRTC_Time[5] = time_date.tm_sec;
}

