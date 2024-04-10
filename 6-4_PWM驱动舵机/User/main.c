#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "Buzzer.h"
#include "LightSensor.h"
#include "OLED.h"
#include "CountSensor.h"
#include "Encoder.h"
#include "Servo.h"

uint8_t KeyNum;

float Angle;

int main(void)
{
	
	OLED_Init();
	Servo_Init();
	Key_Init();
	Servo_SetAngle(0);
	OLED_ShowString(1, 1, "Angle:");
	
	while(1)
	{
		KeyNum = Key_GetNum();
		if(KeyNum == 1)
		{
			Angle += 30;
			if(Angle > 180)
			{
				Angle = 0;
			}
		}
		Servo_SetAngle(Angle);
		OLED_ShowNum(1, 7, Angle, 3);
	}
}

