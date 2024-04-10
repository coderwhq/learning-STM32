#include "stm32f10x.h"
#include "MPU6050_Reg.h"

#define MPU6050_ADDRESS 	0xD0

// 封装I2C_CheckEvent函数，增加超时退出功能
void MPU6050_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint32_t Timeout = 10000;
	
	while(I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)
	{
		Timeout --;
		if(Timeout == 0)
		{
			break; // 这里只是进行简单的退出，实际项目中需要做一些错误处理
		}	
	}
}


// 指定地址写一个字节
void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	
//	MyI2C_Start();						// 阻塞式程序，内部有Delay等待电平变化结束
//	MyI2C_SendByte(MPU6050_ADDRESS);
//	MyI2C_ReceiveAck();
//	MyI2C_SendByte(RegAddress);
//	MyI2C_ReceiveAck();
//	MyI2C_SendByte(Data);
//	MyI2C_ReceiveAck();
//	MyI2C_Stop();
	
	
	I2C_GenerateSTART(I2C2, ENABLE); // 非阻塞式程序，在函数结束后，往往都需要手动等待标志位
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	// 库函数中发送数据都自带了接受应答的过程，同样接收数据也自带了发送应答的过程
	// 发送地址
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	
	I2C_SendData(I2C2, RegAddress);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING); // 中间传送字节流需要等待EV8事件
	
	I2C_SendData(I2C2, Data);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED); // 最后一个字节传输完毕后需要等待EV8_2事件
	
	I2C_GenerateSTOP(I2C2, ENABLE);
	
	
}

// 指定地址读一个字节
uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	
//	MyI2C_Start(); 
//	MyI2C_SendByte(MPU6050_ADDRESS);
//	MyI2C_ReceiveAck();
//	MyI2C_SendByte(RegAddress);
//	MyI2C_ReceiveAck();
//	
//	MyI2C_Start(); // 重复起始条件
//	MyI2C_SendByte(MPU6050_ADDRESS | 0x01); // 修改读写位为1 读
//	MyI2C_ReceiveAck();
//	Data = MyI2C_ReceiveByte();
//	MyI2C_SendAck(1); // 不给从机应答
//	MyI2C_Stop();

	I2C_GenerateSTART(I2C2, ENABLE);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	
	I2C_SendData(I2C2, RegAddress);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED); // 注意这里最好使用I2C_EVENT_MASTER_BYTE_TRANSMITTED，等待前一个字节发送完

	I2C_GenerateSTART(I2C2, ENABLE); // 重复起始条件 // 如果当前还有字节正在移位，那么这个起始条件将会延迟
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Receiver);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED); // 注意主机接收和主机发送的EV6事件不是同一个事件
	
	// 因为我们在这个程序中只接收一个字节的数据，那么这一个字节就是最后一个字节
	// 根据库函数的描述，当发送最后一个字节时，需要在接收之前提前设置不应答 和 生成终止条件
	I2C_AcknowledgeConfig(I2C2, DISABLE); 
	I2C_GenerateSTOP(I2C2, ENABLE);
	
	/* 从机在这里会在SDA上面发送数据 */
	
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED); // EV7事件是当接收到一个字节之后会产生
	Data = I2C_ReceiveData(I2C2); // 读取DR寄存器
	
	I2C_AcknowledgeConfig(I2C2, ENABLE); // 不要忘记最后在置1，恢复默认
	
	return Data;
}

void MPU6050_Init(void)
{
	// 硬件I2C初始化
	
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable; // 用于在接收一个字节后是否给从机应答，和I2C_AcknowledgeConfig这个函数的效果一样
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // 从机地址长度
	I2C_InitStructure.I2C_ClockSpeed = 50000; // 时钟频率  最大400kHz，>100kHz为快速状态，<=100kHz为标准速度
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2; // 时钟占空比，只有在时钟>100kHz的快速状态下才有用，在<=100kHz的标准速度下，占空比是固定的1:1
	I2C_InitStructure.I2C_OwnAddress1 = 0x00; // 自身地址，是STM32作为从机使用的
	
	I2C_Init(I2C2, &I2C_InitStructure);
	
	I2C_Cmd(I2C2, ENABLE);
	
	// 通过写寄存器，配置MPU6050内的寄存器，需要对照MPU6050的寄存器手册查看
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01); // 解除睡眠，采用陀螺仪时钟
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00); // 6轴均不待机
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09); // 采样分频为10
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06); // 滤波参数最大
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18); // 陀螺仪最大量程
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18); // 加速度计最大量程
	
}

uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

void MPU6050_GetData(int16_t *AccX, int16_t	*AccY, int16_t *AccZ,
						int16_t *GyroX, int16_t	*GyroY, int16_t *GyroZ)
{
	uint8_t DataH, DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
	
	*AccX = (DataH << 8) | DataL; // 8位数据进行左移后会自动进行类型转换，移出去的位不会丢失
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
	
	*AccY = (DataH << 8) | DataL; 
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
	
	*AccZ = (DataH << 8) | DataL; 
	
	
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
	
	*GyroX = (DataH << 8) | DataL; 
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
	
	*GyroY = (DataH << 8) | DataL; 
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
	
	*GyroZ = (DataH << 8) | DataL; 
	
}



