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
BKP(���ݼĴ���)

��STM32�ϵ��ֻҪVBAT��Ȼ���ⲿ��Դ���磬���ݾͲ��ᶪʧ������ϵͳ��λ��BKP������Ҳ���Ḵλ������BKP����������ʵ�ֵ��粻��ʧ����ҪVBAT�����ṩ���õ�Դ��ά�֣�������Դ�е�ʱ��VBAT�����õ�����������Դ���磬�������Խ�ʡ��ص���

BKP�а���VBAT

BKP�а���TAMPER���Ų����������¼������б��ݼĴ�����������������ܣ�

BKP�а���RTCʱ��У׼�Ĵ���

��������һ������
*/

int main(void)
{
	
	OLED_Init();				//OLED��ʼ��
	Key_Init();	
	
	OLED_ShowString(1, 1, "W:");
	OLED_ShowString(2, 1, "R:");
	
	/*
		ͨ�����üĴ���RCC_APB1ENR��PWREN��BKPENλ���򿪵�Դ�ͺ󱸽ӿڵ�ʱ��
		��Դ���ƼĴ���(PWR_CR)��DBPλ��ʹ�ܶԺ󱸼Ĵ�����RTC�ķ��ʡ�
	*/
	// ��������Ϊ�˷���ʹ��BPK��ע������
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
	
	PWR_BackupAccessCmd(ENABLE);
	
	while(1)
	{
		KeyNum = Key_GetNum();		//��ȡ��������
		
		if (KeyNum == 1)			//����1����
		{
			ArrayWrite[0] ++;		//������������
			ArrayWrite[1] ++;
			
			BKP_WriteBackupRegister(BKP_DR1, ArrayWrite[0]);	//д��������ݵ����ݼĴ���
			BKP_WriteBackupRegister(BKP_DR2, ArrayWrite[1]);
			
			OLED_ShowHexNum(1, 3, ArrayWrite[0], 4);		//��ʾд��Ĳ�������
			OLED_ShowHexNum(1, 8, ArrayWrite[1], 4);
		}
		
		ArrayRead[0] = BKP_ReadBackupRegister(BKP_DR1);		//��ȡ���ݼĴ���������
		ArrayRead[1] = BKP_ReadBackupRegister(BKP_DR2);
		
		OLED_ShowHexNum(2, 3, ArrayRead[0], 4);				//��ʾ��ȡ�ı��ݼĴ�������
		OLED_ShowHexNum(2, 8, ArrayRead[1], 4);
	}
	
	//Buzzer_OFF();
}
