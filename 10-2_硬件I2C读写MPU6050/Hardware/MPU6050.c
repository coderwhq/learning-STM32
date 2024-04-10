#include "stm32f10x.h"
#include "MPU6050_Reg.h"

#define MPU6050_ADDRESS 	0xD0

// ��װI2C_CheckEvent���������ӳ�ʱ�˳�����
void MPU6050_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint32_t Timeout = 10000;
	
	while(I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)
	{
		Timeout --;
		if(Timeout == 0)
		{
			break; // ����ֻ�ǽ��м򵥵��˳���ʵ����Ŀ����Ҫ��һЩ������
		}	
	}
}


// ָ����ַдһ���ֽ�
void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	
//	MyI2C_Start();						// ����ʽ�����ڲ���Delay�ȴ���ƽ�仯����
//	MyI2C_SendByte(MPU6050_ADDRESS);
//	MyI2C_ReceiveAck();
//	MyI2C_SendByte(RegAddress);
//	MyI2C_ReceiveAck();
//	MyI2C_SendByte(Data);
//	MyI2C_ReceiveAck();
//	MyI2C_Stop();
	
	
	I2C_GenerateSTART(I2C2, ENABLE); // ������ʽ�����ں�����������������Ҫ�ֶ��ȴ���־λ
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	// �⺯���з������ݶ��Դ��˽���Ӧ��Ĺ��̣�ͬ����������Ҳ�Դ��˷���Ӧ��Ĺ���
	// ���͵�ַ
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	
	I2C_SendData(I2C2, RegAddress);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING); // �м䴫���ֽ�����Ҫ�ȴ�EV8�¼�
	
	I2C_SendData(I2C2, Data);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED); // ���һ���ֽڴ�����Ϻ���Ҫ�ȴ�EV8_2�¼�
	
	I2C_GenerateSTOP(I2C2, ENABLE);
	
	
}

// ָ����ַ��һ���ֽ�
uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	
//	MyI2C_Start(); 
//	MyI2C_SendByte(MPU6050_ADDRESS);
//	MyI2C_ReceiveAck();
//	MyI2C_SendByte(RegAddress);
//	MyI2C_ReceiveAck();
//	
//	MyI2C_Start(); // �ظ���ʼ����
//	MyI2C_SendByte(MPU6050_ADDRESS | 0x01); // �޸Ķ�дλΪ1 ��
//	MyI2C_ReceiveAck();
//	Data = MyI2C_ReceiveByte();
//	MyI2C_SendAck(1); // �����ӻ�Ӧ��
//	MyI2C_Stop();

	I2C_GenerateSTART(I2C2, ENABLE);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	
	I2C_SendData(I2C2, RegAddress);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED); // ע���������ʹ��I2C_EVENT_MASTER_BYTE_TRANSMITTED���ȴ�ǰһ���ֽڷ�����

	I2C_GenerateSTART(I2C2, ENABLE); // �ظ���ʼ���� // �����ǰ�����ֽ�������λ����ô�����ʼ���������ӳ�
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Receiver);
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED); // ע���������պ��������͵�EV6�¼�����ͬһ���¼�
	
	// ��Ϊ���������������ֻ����һ���ֽڵ����ݣ���ô��һ���ֽھ������һ���ֽ�
	// ���ݿ⺯�������������������һ���ֽ�ʱ����Ҫ�ڽ���֮ǰ��ǰ���ò�Ӧ�� �� ������ֹ����
	I2C_AcknowledgeConfig(I2C2, DISABLE); 
	I2C_GenerateSTOP(I2C2, ENABLE);
	
	/* �ӻ����������SDA���淢������ */
	
	MPU6050_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED); // EV7�¼��ǵ����յ�һ���ֽ�֮������
	Data = I2C_ReceiveData(I2C2); // ��ȡDR�Ĵ���
	
	I2C_AcknowledgeConfig(I2C2, ENABLE); // ��Ҫ�����������1���ָ�Ĭ��
	
	return Data;
}

void MPU6050_Init(void)
{
	// Ӳ��I2C��ʼ��
	
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable; // �����ڽ���һ���ֽں��Ƿ���ӻ�Ӧ�𣬺�I2C_AcknowledgeConfig���������Ч��һ��
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // �ӻ���ַ����
	I2C_InitStructure.I2C_ClockSpeed = 50000; // ʱ��Ƶ��  ���400kHz��>100kHzΪ����״̬��<=100kHzΪ��׼�ٶ�
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2; // ʱ��ռ�ձȣ�ֻ����ʱ��>100kHz�Ŀ���״̬�²����ã���<=100kHz�ı�׼�ٶ��£�ռ�ձ��ǹ̶���1:1
	I2C_InitStructure.I2C_OwnAddress1 = 0x00; // �����ַ����STM32��Ϊ�ӻ�ʹ�õ�
	
	I2C_Init(I2C2, &I2C_InitStructure);
	
	I2C_Cmd(I2C2, ENABLE);
	
	// ͨ��д�Ĵ���������MPU6050�ڵļĴ�������Ҫ����MPU6050�ļĴ����ֲ�鿴
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01); // ���˯�ߣ�����������ʱ��
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00); // 6���������
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09); // ������ƵΪ10
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06); // �˲��������
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18); // �������������
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18); // ���ٶȼ��������
	
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
	
	*AccX = (DataH << 8) | DataL; // 8λ���ݽ������ƺ���Զ���������ת�����Ƴ�ȥ��λ���ᶪʧ
	
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



