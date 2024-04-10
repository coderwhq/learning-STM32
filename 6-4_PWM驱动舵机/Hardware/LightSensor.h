#ifndef __LIGHT_SENSOR_H
#define __LIGHT_SENSOR_H
#include "stm32f10x.h"

#define LIGHT_SENSOR_CLOCK	RCC_APB2Periph_GPIOB
#define LIGHT_SENSOR_GPIO	GPIOB
#define LIGHT_SENSOR_PIN	GPIO_Pin_13

void LightSensorInit(void);
uint8_t LightSensor_Get(void);

#endif
