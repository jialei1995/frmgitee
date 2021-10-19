#ifndef __ALARM_H
#define __ALARM_H   
#include "main.h"

typedef enum{
	LESS = 0,
	MORE
}COMPARE;
typedef union
{
	uint16_t bytes;
	struct 	
	{
		uint16_t O_V:1;//0
		uint16_t U_V_2700:1;// 1
		uint16_t O_C_D:1;// 2
		uint16_t S_C_D:1;// 3
		
		uint16_t U_T_D:1;// 4
		uint16_t O_T_M:1;// 5
		uint16_t O_C_C:1;// 6
		uint16_t U_V_1500:1;// 7
		
		uint16_t O_T_C:1;// 8
		uint16_t O_T_D:1;// 9
		uint16_t U_T_C:1;// 10
		uint16_t C_M_E:1;// 11
		
		uint16_t D_M_E:1;
		uint16_t C_P_Count:1; 
		uint16_t RESREVE:2; 
	}bits;
}AlarmSt;

#define R_HV_Bitnum 0
#define R_UV_Bitnum 1
#define R_OCD_Bitnum 2
#define R_UTD_Bitnum 4
#define R_OTM_Bitnum 5
#define R_OTC_Bitnum 8
#define R_OTD_Bitnum 9
#define R_UTC_Bitnum 10
#define R_OCC_Bitnum 6
#define R_HV_N_Bitnum 16




#define NotSleepAlarmMast  0x036D  //过流报警不休眠
#define SleepAlarmMast	(NotSleepAlarmMast^0xffff)	



typedef struct
{
	uint8_t              alarmFlag;
	uint8_t              Times;
	uint8_t              ReTimes;
	//回调函数
	//void    ( *Callback )( void );
} ALARM;
extern ALARM    U_V_2700;
extern ALARM    U_V_2300;
extern ALARM    U_V_1500;
extern ALARM    C_O_C;
extern ALARM    D_O_C;

extern ALARM    O_T_M;
extern ALARM    U_T_C;
extern ALARM    U_T_D;
extern ALARM    C_M_E;

extern ALARM    D_M_E;
extern ALARM    CHG_OV_P_Count;
extern ALARM    D_O_C2;
extern ALARM    S_C_D;

extern ALARM    O_T_C;
extern ALARM    O_T_D;
extern ALARM    O_V;

extern AlarmSt   userAlarm;

void    AlarmFilter( int32_t Value,
					 int32_t FilterValue,
					 int32_t RecoverValue,
					 COMPARE  CompareType,
					 ALARM*  Buff,
					 uint8_t  FilterTimes,
					 uint8_t  FilterReTimes);
uint16_t Alarm_Com_Count(void);

void AlarmCheck(void);
#endif


