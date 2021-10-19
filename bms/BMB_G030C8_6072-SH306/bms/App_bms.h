#ifndef __BMS_H
#define __BMS_H   
#include "main.h"

#define   NotBit(x,y)            ((x)^=(1<<(y)))
#define   SetBit(x,y)            ((x) |= (1<<(y)))
#define   ClrBit(x,y)            ((x) &= ~(1<<(y)))
#define  	GetBit(x,y)     		(((x)>>(y))&(0x01))

#define TimeOut(n) (n*2L)

typedef union
{
	uint16_t bytes;
	struct 	
	{
		uint8_t bit0:1;
		uint8_t bit1:1;
		uint8_t bit2:1;
		uint8_t bit3:1;
		uint8_t bit4:1;
		uint8_t bit5:1;
		uint8_t bit6:1;
		uint8_t bit7:1;
		uint8_t bit8:1;
		uint8_t bit9:1;
		uint8_t bit10:1;
		uint8_t bit11:1;
		uint8_t bit12:1;
		uint8_t bit13:1;
		uint8_t bit14:1;
		uint8_t bit15:1;
	}bits;
}S16_BITS;

typedef union
{
	uint16_t halfword;
	struct 	
	{
		uint8_t byte_l:8;
		uint8_t byte_h:8;
	}bytes;
}S_HALFWORD;

typedef union
{
	int16_t shalfword;
	struct 	
	{
		int8_t byte_l:8;
		int8_t byte_h:8;
	}bytes;
}S_SHALFWORD;

typedef union
{
	uint32_t word;
	struct 	
	{
		uint8_t byte_1:8;
		uint8_t byte_2:8;
		uint8_t byte_3:8;
		uint8_t byte_4:8;
	}bytes;
	struct 	
	{
		uint16_t Short_1:16;
		uint16_t Short_2:16;
	}u16data;
}S_WORD;

typedef union
{
	int32_t ssword;
	struct 	
	{
		int8_t byte_1:8;
		int8_t byte_2:8;
		int8_t byte_3:8;
		int8_t byte_4:8;
	}bytes;
}S_SWORD;

typedef union
{
	int64_t sdword;
	struct 	
	{
		int8_t byte_1:8;
		int8_t byte_2:8;
		int8_t byte_3:8;
		int8_t byte_4:8;
		int8_t byte_5:8;
		int8_t byte_6:8;
		int8_t byte_7:8;
		int8_t byte_8:8;
	}bytes;
}S_SDWORD;

typedef union
{
	uint64_t dword;
	struct 	
	{
		uint8_t byte_1:8;
		uint8_t byte_2:8;
		uint8_t byte_3:8;
		uint8_t byte_4:8;
		uint8_t byte_5:8;
		uint8_t byte_6:8;
		uint8_t byte_7:8;
		uint8_t byte_8:8;
	}bytes;
}S_DWORD;


typedef union
{
	uint8_t bytes;
	struct 	
	{
		uint8_t bit0:1;
		uint8_t bit1:1;
		uint8_t bit2:1;
		uint8_t bit3:1;
		uint8_t bit4:1;
		uint8_t bit5:1;
		uint8_t bit6:1;
		uint8_t bit7:1;
		
	}bits;

}S8_BITS;





typedef struct 
{
	uint16_t alarmByte;
	uint8_t SOC;
	uint8_t SOH;

	uint8_t setSoc;//设置
	uint8_t batStat;
	
	uint8_t chgMos;
	uint8_t dsgMos;
	

	uint8_t cellTemp[6];
	uint8_t tempMax;
	uint8_t tempMin;
	uint8_t tempAvg;
	
	uint8_t balTemp;
	uint8_t mosTemp;
	uint8_t envTemp;
	
	uint16_t cellVolt[25];
	uint16_t batVolt[25];
	int16_t voltMax;
	int8_t voltMaxNum;
	int16_t voltMin;
	int16_t voltAvg;
	int16_t voltDiff;
	uint16_t Period_Integral;
	uint16_t  IntegralPower;//ZHQL_1013
	uint16_t  IntegralPower_C;//ZHQL_1013

	uint16_t  IntegralPowerjisuan;	
	uint32_t totalSumVolt;//单体累加总压

	uint32_t FullChargeCap;/*充满容量*/	
	uint32_t RemainCap;	/*积分剩余容量*/
	
//	uint32_t AbsCap;/*绝对容量*/	
	
	uint32_t TotalDsgCap;
	int32_t current;
	int8_t CurrCailFalg;		//电流校准标志
	int8_t DeepSleepCMD;
	int8_t SleepCMD;
	uint8_t SocChangFlag;
}SysPara;

typedef union
{
	uint16_t bytes;
	struct 	
	{
		uint16_t flashSave_Flag:1;
		uint16_t socInit_Flag:1;//初始化条件
		uint16_t DET_Flag:1;
		uint16_t LoadFlag:1;
		
		uint16_t StopSocAdddFlag:1;
		uint16_t StopSocCutFlag:1;
		uint16_t CHGingFlag:1;
		uint16_t DSGingFlag:1;

		uint16_t RTCWeak:1;
		uint16_t CHGWeak:1;
		uint16_t DSGWeak:1;
		uint16_t SIFWeak:1;
		
		uint16_t VoltChargeBigFlag:1;
		uint16_t SysWeakFlag:1;
		uint16_t SampOverFlag:1;
		uint16_t Rs485Staty:1;
		
	}bits;

}SysFlag;

//客户通讯指令
typedef union
{
	uint8_t bytes;
	struct 	
	{
		uint8_t sleepCmd:1;
		uint8_t dsgMosOnCmd:1;
		uint8_t dsgMosOffCmd:1;
		uint8_t chgMosOnCmd:1;
		uint8_t chgMosOffCmd:1;
		
	}bits;

}CustomCmd;

extern SysFlag sysflag;
extern SysPara syspara;

typedef enum 
{
	LONGOUT = 0X01,
	EXITWAKE = 0X02,
	EXITWAKECMD = 0X03,
	BURNIN = 0XA5,
}workMode;

typedef enum 
{
	UNCOM = 0X01,
	COM = 0X02,
}chgMode ;

void Soft_Delay(uint16_t time);
uint8_t CRC8(uint8_t *ptr,uint8_t len);

void Process_100ms_CYC(void);
void Process_500ms_CYC1(void);

#endif









