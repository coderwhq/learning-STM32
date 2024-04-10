#include "stm32f10x.h"
#include "PWM.h"


void Servo_Init(void)
{
	PWM_Init();
}
// ��δ��뻹�漰��һ��������ȡ����������Ϊ�Ƕ���Ҫ��ת��ΪPWM��ռ�ձȡ�
// ʹ�ø��������Ը���ȷ��ִ�����ת������Ϊ�����������ṩС�����֣����������ͣ��� uint16_t��ֻ�ܱ�ʾ������
void Servo_SetAngle(float Angle)
{
	// 0     500
	// 180   2500
	PWM_SetCompare2(Angle / 180 * 2000 + 500);
}


