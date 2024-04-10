#include "stm32f10x.h"
#include "LightSensor.h"

void LightSensorInit(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(LIGHT_SENSOR_CLOCK, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = LIGHT_SENSOR_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(LIGHT_SENSOR_GPIO, &GPIO_InitStructure);
	//GPIO_SetBits(LIGHT_SENSOR_GPIO, LIGHT_SENSOR_PIN);

}

uint8_t LightSensor_Get(void) 
{
	return GPIO_ReadInputDataBit(LIGHT_SENSOR_GPIO, LIGHT_SENSOR_PIN);
}

