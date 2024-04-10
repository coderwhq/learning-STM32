#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"
#include "IC.h"

uint8_t i;

/*

	���벶�񣺵�ͨ���������ų��ֵ�ƽ����ʱ����ǰCNT��ֵ�ᱻ������CCR�У����ܷ���
	�����ڲ���PWM���ε�Ƶ�ʡ�ռ�ձȡ���������Ƶ�ʣ�����ƽ����ʱ�䣨ռ�ձȣ��Ȳ���
	
	������PWMIģʽ��ͬʱ����Ƶ�ʺ�ռ�ձ�
	���������Ӵ���ģʽ��ʵ��Ӳ��ȫ�Զ�����
	Ƶ�ʲ�����
		��Ƶ�� �ʺϲ�����Ƶ�ź� ������� �ȶ� ��բ��ʱ��T�ڣ��������ؼƴεõ�N	fx = N / T
		���ܷ� �ʺϲ�����Ƶ�ź� ������� ���� �����������ڣ��Ա�׼Ƶ��fc�ƴΣ��õ�N��Ȼ��ȡ����	fx = fc / N
		�н�Ƶ�ʣ���Ƶ���Ͳ��ܷ������ȵ�Ƶ�ʵ�   fm = sqrt(fc / T);
		
	1/1000000 ��1Hz�ĳ���ʱ�䣬ʱ�����ڣ�������������ʱ�����cnt��ֵ������CCR�У�
	��CCR��ֵ�������ʱ�����ڣ���������Ƶ��ʱ�����ڣ���ȡ��������������Ƶ��
*/

int main(void)
{
	
	OLED_Init();
	PWM_Init();
	IC_Init();
	
	OLED_ShowString(1, 1, "Freq:00000Hz");
	OLED_ShowString(2, 1, "Cnt:00000Hz");
	
	PWM_SetPrescaler(720 - 1); 	// Freq = 72M / (PSC + 1) / 100
	PWM_SetCompare1(50);		// Duty = CCR / 100
	
	while(1)
	{
		OLED_ShowNum(1, 6, IC_GetFreq(), 5);
		OLED_ShowNum(2, 5, IC_GetCnt(), 5);
	}
}
