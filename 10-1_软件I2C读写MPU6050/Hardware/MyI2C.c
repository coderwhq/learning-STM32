#include "stm32f10x.h"                  // Device header
#include "Delay.h"
//#define SCL_PORT 			GPIOB
//#define SCL_PIN 			GPIO_Pin_10
//#define SDA_PIN 			GPIO_Pin_11

// ������ֲ������дSCL��������1��0 �Ϳ����ͷŻ�����SCL
void MyI2C_W_SCL(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)BitValue);
	Delay_us(10);
}

// дSDA
void MyI2C_W_SDA(uint8_t BitValue)
{
	// ֻҪ����ֵ��0����ô����1
	GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)BitValue);
	Delay_us(10);
}
// ��SDA
uint8_t MyI2C_R_SDA(void)
{
	uint8_t BitValue;
	BitValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
	Delay_us(10);
	return BitValue;
}

void MyI2C_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; // ���ģʽ��Ȼ��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11); // �ÿ���״̬
}


/* 
	�����ģ�����ʱ����Ҫȷ��ÿһ�β�����������ֹ�������Ľ�βSCL��Զ����0������
	�����Ļ�ÿһ�β����Ŀ�ʼSCL��Զ����0Ϊ��ʼ
	SCLȫ��������������
*/ 

void MyI2C_Start(void)
{
	/*
		�������øߵ�ƽ����Ϊ���ظ���ʼ��������ǰ��SCLһ��Ϊ0��SDA��ȷ��
		���Գ�SCLΪ�͵�ƽʱ��SDA��1��Ȼ����SCL��1���ص�����״̬��˳���ܷ�����Ȼ���ܻᴥ����ֹ����
		Ȼ��������SDA������SCL�Ϳ��Դ�����ʼ����
	*/
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(0);
	
}


void MyI2C_Stop(void)
{
	/*
		��ֹ��������ǰ��SCLһ��Ϊ0��SDA����Ӧ��Ĳ�ͬ����Ϊ1����Ϊ0������Ϊ�˱�֤Ҫ��SCL�ߵ�ƽ�ڼ�ı�SDA
		��Ҫ�Ƚ�SDA��0��Ȼ���ٽ�SCL��1������ٽ�SDA��1����������������SCL�ߵ�ƽ�ڼ�ı�SDA
		�����SCL�ߵ�ƽ�ڼ�ı���SDA�ĵ�ƽ�����Ǵ�������ʼ����ֹ����

	*/
	MyI2C_W_SDA(0);
	
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(1);
	
}

// �����ֽ� ��λ����
void MyI2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	
	for(i = 0; i < 8; i ++)
	{
		// ��Ϊ����47��ע����ǰԼ���õĹ��������ڲ�����ʼʱSCLΪ0��ֱ���޸�SDA����
		if((Byte & (0x80 >> i)) == 0)
		{	
			MyI2C_W_SDA(0);
		}
		else
		{
			MyI2C_W_SDA(1);
		}
		
		// �޸�SDA�� SCL��1
		MyI2C_W_SCL(1);
		// ��ʱ���豸�Ѿ���SCLΪ�ߵ�ƽʱ��ȡ���ݣ���βSCL��0
		MyI2C_W_SCL(0);
	}
	
}


uint8_t MyI2C_ReceiveByte(void)
{
	
	uint8_t i, Byte = 0x00;
	
	// �����ͷ�SDA���л�Ϊ�ӻ�����ģʽ
	MyI2C_W_SDA(1);
	
	/****
		��ʱ�ӻ������ݷŵ�SDA��
	****/

	for(i = 0; i < 8; i ++)
	{

		// �ӻ��豸����һ��bit�����SCL�ǵ͵�ƽ��������������Ҫ��SCLΪ�ߵ�ƽʱ��ȡ���ݣ�����������1
		MyI2C_W_SCL(1);
		// ��ȡ����
		if(MyI2C_R_SDA() == 1)
		{
			Byte |= (0x80 >> i);
		}
		else
		{
			// �������������Ϊ0������൱��д��0�ˣ�ByteĬ�Ͼ�Ϊ0x00��
		}
			
		// ��βSCL��0���ôӻ���SDA�Ϸ�����һλ����
		MyI2C_W_SCL(0);
	}
	return Byte;
}




// ����Ӧ��
void MyI2C_SendAck(uint8_t Ackbit)
{
	MyI2C_W_SDA(Ackbit);
	MyI2C_W_SCL(1); // �ߵ�ƽ���ӻ���ȡӦ��
	MyI2C_W_SCL(0);
}


uint8_t MyI2C_ReceiveAck(void)
{
	
	uint8_t AckByte;
	// �ͷ�SDA���ߵĿ���Ȩ���ӻ�����Ӧ��λ
	MyI2C_W_SDA(1);
	// �øߵ�ƽ�������ڸߵ�ƽ��ȡӦ��λ
	MyI2C_W_SCL(1);
	AckByte = MyI2C_R_SDA();
	MyI2C_W_SCL(0);

	return AckByte;
}




