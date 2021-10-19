#ifndef __SH36730x_H_
#define __SH36730x_H_
#include "main.h"

#define DEF_SCAN50ms  	0       //电压采样周期50ms
#define DEF_SCAN100ms  	1       //电压采样周期100ms
#define DEF_SCAN200ms  	2       //电压采样周期200ms
#define DEF_SCAN300ms  	3       //电压采样周期500ms
#define DEF_SCAN1s  		4       //电压采样周期1s
#define DEF_SCAN2s  		5       //电压采样周期2s
#define DEF_SCAN4s  		6       //电压采样周期4s
#define DEF_SCAN8s  		7       //电压采样周期8s

typedef enum
{
	GT_OK=0,
	GT_CRCERR,
	GT_WriteERR,
}GT_TYPE;

#define DEF_CADC_SCOPE             2      //电流采样范围  0:      400mv              1:     200mv            2:  100mv         3:       50mv
#define DEF_SCD_VOLTAGE          1      // 短路保护阈值 0:      100mv              1 :    200mv            2:  300mv         3:       400mv
#define DEF_SCD_TIME          	2      // 短路保护时间 0:      50us            1:     100us            2:  300us         3:       500us


#define DEF_VolSacnTime  		DEF_SCAN50ms   //电压采样周期

#define DEF_OV_TIME          		5      //  过压保护时间         4 :16个采样周期          5 :  32个采样周期 
#define DEF_CHS_Value          	0      //   0   1.4mV   1  3.0mV          充放电状态检测阈值




#define  CONFG6_VALUE  ((DEF_CADC_SCOPE<<6)|(DEF_SCD_VOLTAGE<<2)|(DEF_SCD_TIME<<0))
#define  CONFG7_VALUE  ((DEF_OV_TIME<<4)|(DEF_CHS_Value<<2) )  


#define DEF_TOTAL_CELLNUM 25
#define DEF_TOTAL_TEMPNUM 6



typedef union
{
	struct
	{
		uint8_t CHGR: 1;
		uint8_t LOAD : 1;
		uint8_t CHGING : 1;
		uint8_t DSGING : 1;
		uint8_t CHG:1;	
		uint8_t DSG:1;	
		uint8_t RESERVED:2;
	}Status_Bits;
	uint8_t Vaule;
}BSTATUS_Reg_t;


typedef union
{
	struct
	{
		uint8_t TWI_FLG: 1;
		uint8_t WDT_FLG : 1;
		uint8_t OV_FLG : 1;
		uint8_t SC_FLG : 1;
		uint8_t RESERVED:4;	
	}Flag_Bits;
	uint8_t Vaule;
}BFLAG1_Reg_t;


typedef union
{
	struct
	{
		uint8_t VADC_FLG: 1;
		uint8_t CADC_FLG : 1;
		uint8_t RST_FLG : 1;
		uint8_t RESERVED:5;	
	}Flag_Bits;
	uint8_t Vaule;
}BFLAG2_Reg_t;

typedef union
{
	struct
	{
		uint8_t CHGR_EN: 1;//dianchigeshu
		uint8_t LOAD_EN: 1;
		uint8_t OV_EN: 1;
		uint8_t SC_EN: 1;//
		uint8_t WDT_EN: 1; //预充电使能
		uint8_t PD_EN: 1; //预充电使能
		uint8_t CTLD_EN: 1; //预充电使能
		uint8_t LTCLR_EN: 1; //预充电使能
	}Bits;
	uint8_t Vaule;
}SCONF1_Reg_t;


typedef union
{
	struct
	{
		uint8_t CHG_C: 1;
		uint8_t DSG_C : 1;
		uint8_t ALARM_C : 1;
		uint8_t RESET_PF:1;	
		uint8_t RESERVED:4;	
	}Bits;
	uint8_t Vaule;
}SCONF2_Reg_t;

typedef union
{
	struct
	{
		uint8_t SCAN_C: 3;//dianchigeshu
		uint8_t VADC_C: 1;
		uint8_t VADC_EN: 1;
		uint8_t CBIT_C: 1;//
		uint8_t CADC_M: 1; //
		uint8_t CADC_EN: 1; //
	}Bits;
	uint8_t Vaule;
}SCONF3_Reg_t;





extern SCONF1_Reg_t SCONF1_Data ;
extern SCONF2_Reg_t SCONF2_Data ;
extern SCONF3_Reg_t SCONF3_Data ;
extern BFLAG1_Reg_t Flag1_Data ;
extern BFLAG2_Reg_t Flag2_Data ;
extern BSTATUS_Reg_t BSTATUS_Data ;


GT_TYPE SH_GetCellVolt(uint16_t * VoltBuff);
GT_TYPE SH_GetCUR(int32_t * CUR);

GT_TYPE SH_GetCellTemp(uint16_t * TempBuff);
/*************负载状态读取****************/

GT_TYPE  SH_CHGR_CHECK(uint8_t mod);

GT_TYPE  SH_LOAD_CHECK(uint8_t mod);



GT_TYPE SH_GetBSTATUS(void);
GT_TYPE SH_GetSFlag(void);


GT_TYPE SH_EnterLowPower(void);

GT_TYPE SH_CloseAD(void);

GT_TYPE SH_MOSCtrl(uint8_t CHGCtrl,uint8_t DSGCtrl);

GT_TYPE SH_BALANCESET(uint32_t BalanceFlag);
GT_TYPE SH_Sleep_C(void);


GT_TYPE SH30x_init(SIMIIC_name_t IICNum );


GT_TYPE SH_OpenAlarmInt(void);

GT_TYPE  SH_ClrAlarm(void);
GT_TYPE SH_StrlVCAD(uint8_t comd);

#endif
