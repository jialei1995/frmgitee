#ifndef __PCMONITOR_H
#define __PCMONITOR_H
#include "main.h"

#define BUFFER_MAX 200//定义缓冲区大小
#define PC_CMDHEAD 3

typedef struct{
	
	uint16_t headPosition;//缓冲区头部位置
	uint16_t tailPosition;
	uint8_t ringBuf[BUFFER_MAX];//缓冲区数组	
	uint8_t dummy;
	uint8_t rcvFlag;
	uint8_t NodataTime;
}ringBuffer_t;
extern ringBuffer_t RS485Buf;

void RingBuf_Write(uint8_t data);

uint8_t RingBuf_Read(uint8_t* val);

void RS485_SendByte(uint8_t *txdBuf,uint16_t len);

void PCMonitor_Process(uint8_t head);
void RS485_Process(void);
#endif

