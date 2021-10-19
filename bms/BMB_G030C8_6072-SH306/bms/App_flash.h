#ifndef __FLASH_H
#define __FLASH_H
#include "main.h"



typedef struct
{
	uint32_t  appmask;
	uint32_t  yuliu;
	uint8_t id[16];
	uint8_t chgMode;
	uint8_t workMode;
	uint16_t CycleCount;
	int16_t currentRate;
	uint16_t FistUseData;
	uint32_t cyc_CHG;
	uint32_t shortCnt;
	int8_t VoltRate[32];
	uint32_t BKP3_Data;
	uint16_t P_UV_Count;
	uint16_t checkh;
}FlashPara;

extern FlashPara flashpara;

#define FLASH_USER_END_ADDR   (FLASH_BASE + 0X10000)

uint8_t Flash_WritePara(void);

void FlashInit(void);
#endif
