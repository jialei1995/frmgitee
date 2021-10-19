#ifndef __SOC_H
#define	__SOC_H
#include "main.h"

#define CYCMAST 0x55000000
#define DESIGN_CAP_MULT(m) (uint32_t)(DEF_CAP*10*m)//ZHQL_1013
#define CFG_Value_SJian 50  //衰减系数 每40次衰减1%
typedef enum
{
	CURVE_STATIC = 0,
	CURVE_C_33,
	CURVE_C_50,
	CURVE_D_33,
	CURVE_D_50,
	CURVE_D_100
	
}CURVE;//soc曲线
typedef enum
{
	Cal_NO = 0x00,
	Cal_OK = 0x5A,
	Cal_DEF = 0xA5,
	Cal_Temp = 0xAA,
	
}SOCCalRET;//soc曲线
extern uint16_t CutCapValue;

void  CycleCount_Init(void);
void Soc_Init(void);
void Soc_ClrOrSetCtrl(void);

uint8_t Get_Percent(uint32_t val,uint32_t total);
uint8_t RealRemainCap_Calibration(void);
void RemainCap_Integral(int32_t curr,uint16_t time,uint16_t IntegralPower);//ZHQL_1013
void Cycle_Integral(int32_t curr,uint16_t time);//ZHQL_1013
#endif

