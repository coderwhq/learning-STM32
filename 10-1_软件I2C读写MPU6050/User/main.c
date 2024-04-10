#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "Buzzer.h"
#include "LightSensor.h"
#include "OLED.h"
#include "MPU6050.h"

uint8_t ID;
int16_t AX, AY, AZ, GX, GY, GZ;

int main(void)
{	
	OLED_Init();
	MPU6050_Init();
	
	OLED_ShowString(1, 1, "ID:");
	ID = MPU6050_GetID();
	OLED_ShowHexNum(1, 4, ID, 2);
	
	// 读取的数据 / 32768 = x / 满量程     x就是具体的角速度值或加速度
	
	while(1)
	{
		MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
		OLED_ShowSignedNum(2, 1, AX, 5);
		OLED_ShowSignedNum(3, 1, AY, 5);
		OLED_ShowSignedNum(4, 1, AZ, 5);
		
		OLED_ShowSignedNum(2 , 8, GX, 5);
		OLED_ShowSignedNum(3, 8, GY, 5);
		OLED_ShowSignedNum(4, 8, GZ, 5);
		
	}
	
	//Buzzer_OFF();
}
/*
I2C

同步，半双工，带数据应答，支持总线挂载多设备（一主多从，多主多从）

所有I2C设备的SCL连在一起，SDA连在一起

设备的SCL和SDA均要配置成开漏输出模式

SCL和SDA各添加一个上拉电阻，阻值一般为4.7KΩ左右

主机掌控SCL的绝对控制权

空闲状态SCL和SDA都是被上拉电阻弱上拉为高电平

起始条件：SCL高电平期间，SDA从高电平切换到低电平

终止条件：SCL高电平期间，SDA从低电平切换到高电平

起始和终止都是主机产生的，在总线空闲 状态时，不允许从机碰总线

发送一个字节：SCL低电平期间，主机将数据位依次放到SDA线上（高位先行），然后释放SCL，从机将在SCL高电平期间读取数据位，所以SCL高电平期间SDA不允许有数据变化，依次循环上述过程8次，即可发送一个字节 ，从机几乎都是贴着SCL的上升沿读取数据

接受一个字节：SCL低电平期间，从机将数据位依次放到SDA线上（高位先行），然后释放SCL，主机将在SCL高电平期间读取数据位，所以SCL高电平期间SDA不允许有数据变化，依次循环上述过程8次，即可接受一个字节（主机在接受之前，需要释放SDA）从机几乎都是贴着SCL的下降沿发送数据

接受和发送的SCL时序是不一样的，由主机来控制

发送应答信号：主机在接收完一个字节之后，在下一个时钟发送一位数据，数据0表示应答，数据1表示非应答，此应答位用于告诉从机，还需不需要从机继续发送，0表示需要，1表示不需要

接受应答信号：主机在发送完一个字节之后，在下一个时钟接受一位数据，判断从机是否应答，数据0表示应答，数据1表示非应答（主机在接收之前，需要释放SDA） ，此应答位用于判断从机是否接受到数据，0表示接受到，1表示没接收到



永远都是在SCL的低电平期间对SDA进行改变的操作，永远都是在SCL的高电平期间读取数据位，低电平期间改变数据

所以在SCL的高电平期间是不允许改变SDA的电平的，如果在SCL高电平期间改变了SDA的电平，则是触发了起始或终止条件





MPU6050     6轴  姿态传感器，通过数据融合（姿态解算法：互补滤波、卡尔曼滤波），可进一步得到姿态角（欧拉角），常用于平衡车、飞行器等需要检测自身姿态的场景

3轴加速度计（Accelerometer）：测量XYZ轴的加速度

3轴陀螺仪传感器（Gyroscope）：测量XYZ轴的角速度

16位ADC采集传感器，量化范围 -32768 ~ 32767

VCC、GND电源

SCL、SDA   I2C通信

XCL、XDA  主机I2C通信，用于外接磁力计或者气压计扩展功能

AD0 从机地址的最低位

INT 中断信号输出
*/