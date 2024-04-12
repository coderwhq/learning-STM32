#include "stm32f10x.h"
#include <stdarg.h>
#include <stdio.h>
/**

ֻ�ܷ���һ���ֽڣ����ܽ���һ���ֽ�

�̶�HEX���ݰ�����(����Ϊ4)
�̶�HEX���ݰ�����(����Ϊ4) ���ݰ���ͷ FF ��β FE

�����ַ���
�����ַ���  ��ͷ @ ��β \r\n

��ֱ�ӷ���ʮ��������

�ڲ���װ��printf�����������������ʹ��printfʱ������ʹ��Serial_Printf�������

**/
uint8_t Serial_RxFlag; // ���ͨ�������־λ���ж��Ƿ��ܵ�������

uint8_t Serial_TxPacket[4];
uint8_t Serial_RxPacket[4];
char Serial_RxString[100];

void Serial_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // Ӳ�����أ���ʱ��֪���Ǹ����
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; // ʹ�ܷ��ͺͽ���ģʽ

	USART_Init(USART1, &USART_InitStructure);
	// ���Ͳ�̫��Ҫ��ע���ص���Ҫ��ע���գ�����������Ҫ�����жϣ����жϺ����д�������
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1, ENABLE);
}

// ����һ���ֽ�
void Serial_SendByte(uint8_t Byte)
{
	// ��Ϊ��Ӳ������USART�������ڲ���ʵ����ʼλ�ͽ���λ�����ǲ���Ҫ������
	USART_SendData(USART1, Byte); 
	// �ȴ��ֽڷ�����ϣ����ⲻ��Ҫ�ֶ����㣬��DR��д����(�ٴη���)���Զ�����
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) != SET){};
}

void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for(i = 0; i < Length; i ++)
	{
		 Serial_SendByte(Array[i]);
	}
}
// ���Ϳɱ��������ݰ�
void Serial_SendHexPacket(void)
{
	Serial_SendByte(0xFF); // ��ͷ
	Serial_SendArray(Serial_TxPacket, 4);
	Serial_SendByte(0xFE); // ��β
}

void Serial_SendString(char* String)
{
	uint8_t i;
	for(i = 0; String[i] != '\0'; i ++)
	{
		Serial_SendByte((uint8_t)String[i]);
	}
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while(Y --)
	{
		Result *= X;
	}
	return Result;
}
// ������ת�����ַ������ͳ�ȥ
void Serial_SendNumber(uint32_t Number)
{
	char offset = '0';
	uint8_t i, Length = 0;
	uint32_t Temp = Number;
	while(Temp != 0)
	{
		Length ++;
		Temp /= 10;
	}
	for(i = 1; i <= Length; i ++)
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - i) % 10 + offset);
	}
}

uint8_t Serial_GetReceiveFlag(void)
{
	uint8_t TempFlag = 0;
	
	if(Serial_RxFlag == 1)
	{
		Serial_RxFlag = 0;
		TempFlag = 1;
	}
		
	return TempFlag;
}


// ��д stdio.h �е� fputc ������fputc��printf�����ĵײ㣬�����൱�ڽ�printf�����ض����˴���
int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}

// ������������趼��ʹ��printfʱ�������������������װ������ʹ��sprintf �Ĺ���//////���֪ʶ�㣺C���Կɱ�����б�
void Serial_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(String);
}


// ��������ʱ�����ж�
void USART1_IRQHandler(void)
{
	// �����̬��������ȫ�ֱ�������������ֻ���ʼ��һ��0����ȫ�ֱ�����ͬ���ǣ���̬����ֻ���ڱ�����ʱ�Ķ�������ʹ��
	// Ϊ�˱��ⷢ��������Ҳ���ڰ�ͷ��β���������������״̬�����б���
	static uint8_t RxState = 0; // 0 �ȴ����ܵ���ͷ  1 ��������  2�ȴ����ܰ�β
	static uint8_t pRxPacket = 0;
	static uint8_t Serial_RxHexFlag = 0;
	static uint8_t Serial_RxStringFlag = 0;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		uint8_t RxData = USART_ReceiveData(USART1);
		
		if(RxState == 0)
		{
			if(RxData == 0xFF)
			{
				RxState = 1;	// ״̬���任Ϊ�������ݵ�״̬
				pRxPacket = 0;
				Serial_RxHexFlag = 1; // ʶ�𵽷��͵�Ϊ Digital���ݰ� �İ�ͷ���ý��������ܵ�Ϊ Digital���ݰ������� �ı�־λΪ1
			}
			else if(RxData == '@')
			{
				RxState = 1;	// ״̬���任Ϊ�������ݵ�״̬
				pRxPacket = 0;
				Serial_RxStringFlag = 1;
			}
		}
		else if(RxState == 1)
		{
			if(Serial_RxHexFlag == 1)
			{
				Serial_RxPacket[pRxPacket ++] = RxData;
				if(pRxPacket >= 4)
				{
					pRxPacket = 0;
					RxState = 2;
				}
			}
			else if(Serial_RxStringFlag == 1)
			{
				if(RxData == '\r')
				{
					RxState = 2;
				}
				else
				{
					Serial_RxString[pRxPacket ++] = RxData;
				}
			}
		}
		else if(RxState == 2)
		{
			if(Serial_RxHexFlag == 1)
			{
				if(RxData == 0xFE)
				{	
					RxState = 0;
					Serial_RxHexFlag = 0;
					Serial_RxFlag = 1; // �Ѿ����ܵ�����
				}
			}
			else if(Serial_RxStringFlag == 1)
			{
				if(RxData == '\n')
				{	
					RxState = 0;
					Serial_RxString[pRxPacket ++] = '\0';
					Serial_RxStringFlag = 0;
					Serial_RxFlag = 1; // �Ѿ����ܵ�����
				}
			}
		}
				
	}
}

