#ifndef __Serial_H
#define __Serial_H

#include <stdio.h>

extern uint8_t Serial_RxFlag;
extern uint8_t Serial_TxPacket[];
extern uint8_t Serial_RxPacket[];
extern char Serial_RxString[];

void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);

void Serial_SendHexPacket(void);

void Serial_SendString(char* String);
void Serial_SendNumber(uint32_t Number);

void Serial_Printf(char *format, ...);

uint8_t Serial_GetReceiveFlag(void);

#endif

