#include "stm32f10x.h"
#include "PWM.h"


void Servo_Init(void)
{
	PWM_Init();
}
// 这段代码还涉及到一个除法和取整操作，因为角度需要被转换为PWM的占空比。
// 使用浮点数可以更精确地执行这个转换，因为浮点数可以提供小数部分，而整数类型（如 uint16_t）只能表示整数。
void Servo_SetAngle(float Angle)
{
	// 0     500
	// 180   2500
	PWM_SetCompare2(Angle / 180 * 2000 + 500);
}


