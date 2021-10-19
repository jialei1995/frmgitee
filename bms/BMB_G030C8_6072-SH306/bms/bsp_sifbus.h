#ifndef __SIFBUS_H
#define __SIFBUS_H
#include "main.h"

#define USESIF_FUNCTION 1

#define CHG_DEF   LL_GPIO_IsInputPinSet(GPIOA,LL_GPIO_PIN_4)

#define    SIF_PROTOCAL_CF  2
#define    SIF_PROTOCAL_GTDS   1
#define    SIF_PROTOCAL_NULL  0



#define    SIF_MODE_GTDS   1
#define    SIF_MODE_NULL   0

#define    SIF_TESTMODE_TIMEOUT 5

#define    SIF_BIT_TIME  500 



typedef struct
{
	uint8_t SIFMode;
	uint8_t IdleTime;
	uint8_t SIFModetime;
	uint8_t DataCompleteFlag;
	uint8_t SIFConnectFlag;
	uint8_t SIFSendFalg;
}SIFBUSst;

extern SIFBUSst SIFBUSData;

void SIFProcess(uint32_t SysTime);
void SIFBUS_SendMsg(uint8_t *buf, uint16_t len);
uint8_t BootCmd_Process(uint8_t cmd,uint16_t length);


#endif 

