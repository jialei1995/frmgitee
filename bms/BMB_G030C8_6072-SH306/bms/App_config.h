#ifndef __CONFIG_H
#define __CONFIG_H

#include "main.h"

/************************Project  Config************************************/

/************************Project  Config End************************************/


#define  CFG_BALANCE_USE 1
#define  JISUAN_DASTVALU  1
#define  TEST_CYC 		0
#define  TEST_MOD_LT 0
#define	 CFG_StartSOC  20

#define ADD_BOOT_FUN
//boot 版本
#define BOOTSERVIN  	101
//协议版本
#define CFG_XIEYI_NUM CFG_XIEYI_12
//电池类型
#define BATTERY_TYPE	B_YTPE_LiFe

#define TestRWSERVIN  25

//项目编码
#define PROJECTCODE 	BLX_6020



#if(TEST_CYC==0)
#if 0
	#if(PROJECTCODE== CF_48Z12T5_40)
		#define 		CFG_DESIGN_CAPACITY		1200L	 //电池设计容量
		#define 		CFG_CURR_RES	141 //(417/3)	//电流采样电阻  0.01mΩ
		#define		CFG_Cell_NUM	15
		#define		CFG_TEMP_NUM 2
		#define	FACTURECODE 	GTDS_FAC_CF
		#define	HARDCODE_M  (XIN_G*100+X_F6P6*10+S_LPS)
		#define	HARDCODE_S 	(DEF_48V_NUM*100+DEF_12_NUM)
	#endif

	#if(PROJECTCODE== CF_48N12T5_40A)
		#define 		CFG_DESIGN_CAPACITY		1200L	 //电池设计容量
		#define 		CFG_CURR_RES	253 //(417/3)	//电流采样电阻  0.01mΩ
		#define		CFG_Cell_NUM	15
		#define		CFG_TEMP_NUM 1
		#define FACTURECODE 	GTDS_FAC_CF
		#define HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS)
		#define HARDCODE_S	(DEF_48V_NUM*100+DEF_12_NUM)
	#endif

	#if(PROJECTCODE==CF_48N12T5_40)
		#define 		CFG_DESIGN_CAPACITY		1200L	 //电池设计容量
		#define 		CFG_CURR_RES	139 //(417/3)	//电流采样电阻  0.01mΩ
		#define		CFG_Cell_NUM	15
		#define		CFG_TEMP_NUM 2
		#define FACTURECODE 	GTDS_FAC_CF
		#define HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS)
		#define HARDCODE_S	(DEF_48V_NUM*100+DEF_12_NUM)
	#endif
					
	#if(PROJECTCODE==CF_48T20T5_40A)
		#define 		CFG_DESIGN_CAPACITY		2000L	 //电池设计容量
		#define 		CFG_CURR_RES	139 //(417/3)	//电流采样电阻  0.01mΩ
		#define		CFG_Cell_NUM	15
		#define		CFG_TEMP_NUM 2
		#define FACTURECODE 	GTDS_FAC_CF
		#define HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS)
		#define HARDCODE_S	(DEF_48V_NUM*100+DEF_20_NUM)
	#endif
	

	
	#if(PROJECTCODE== CF_48Z24T5_40)
		#define 		CFG_DESIGN_CAPACITY		2400L	 //电池设计容量
		#define 		CFG_CURR_RES	137 //(400/4)	//电流采样电阻  0.01mΩ
		#define		CFG_Cell_NUM	15
		#define		CFG_TEMP_NUM 2
		#define FACTURECODE 	GTDS_FAC_CF
		#define HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS)
		#define HARDCODE_S	(DEF_48V_NUM*100+DEF_24_NUM)

	#endif
	#if(PROJECTCODE== CF_48Z16T5_40A)
		#define		CFG_DESIGN_CAPACITY 	1600L	 //电池设计容量
		#define 		CFG_CURR_RES	137//(417/3)	//电流采样电阻  0.01mΩ
		#define		CFG_Cell_NUM	15
		#define		CFG_TEMP_NUM 2
		#define FACTURECODE 	GTDS_FAC_CF
		#define HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS)
		#define HARDCODE_S	(DEF_48V_NUM*100+DEF_16_NUM)
	#endif
	
	
	
	#if(PROJECTCODE==CF_48Z16T5_40)
		#define 	CFG_DESIGN_CAPACITY 	1600L	 //电池设计容量
		#define 	CFG_CURR_RES	139//(417/3)	//电流采样电阻	0.01mΩ
		#define 	CFG_Cell_NUM	15
		#define	CFG_TEMP_NUM 2
		#define FACTURECODE 	GTDS_FAC_CF
		#define HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS)
		#define HARDCODE_S	(DEF_48V_NUM*100+DEF_16_NUM)
	#endif
	
	#if(PROJECTCODE== CF_48Z16T5_40B)
		#define	CFG_DESIGN_CAPACITY 	1600L	 //电池设计容量
		#define 	CFG_CURR_RES	141//(417/3)	//电流采样电阻  0.01mΩ
		#define	CFG_Cell_NUM	15
		#define	CFG_TEMP_NUM 2
		#define 	CFG_SET_IDFUN  1
		#define FACTURECODE 	GTDS_FAC_CF
		#define HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS)
		#define HARDCODE_S	(DEF_48V_NUM*100+DEF_16_NUM)
	#endif
	#if(PROJECTCODE==CF_48Z20T5_40)
		#define		CFG_DESIGN_CAPACITY 	2000L	 //电池设计容量
		#define 		CFG_CURR_RES	139 //(417/3)	//电流采样电阻  0.01mΩ
		#define		CFG_Cell_NUM	15
		#define		CFG_TEMP_NUM 2
		#define FACTURECODE 	GTDS_FAC_CF
		#define HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS)
		#define HARDCODE_S	(DEF_48V_NUM*100+DEF_20_NUM)
	#endif

	#if(PROJECTCODE==CF_48T16T5_40)
		#define		CFG_DESIGN_CAPACITY 	1600L	 //电池设计容量
		#define 		CFG_CURR_RES	141//(417/3)	//电流采样电阻  0.01mΩ
		#define		CFG_Cell_NUM	15
		#define		CFG_TEMP_NUM 2
		#define FACTURECODE 	GTDS_FAC_CF
		#define HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS)
		#define HARDCODE_S	(DEF_48V_NUM*100+DEF_16_NUM)
	#endif
	#if(PROJECTCODE==CF_48T20T5_40)
		#define		CFG_DESIGN_CAPACITY 	2000L	 //电池设计容量
		#define 		CFG_CURR_RES	139 //(417/3)	//电流采样电阻  0.01mΩ
		#define		CFG_Cell_NUM	15
		#define		CFG_TEMP_NUM 2
		#define FACTURECODE 	GTDS_FAC_CF
		#define HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS)
		#define HARDCODE_S	(DEF_48V_NUM*100+DEF_20_NUM)
	#endif


	#if(PROJECTCODE==ND_4820_4M_5O)

		#define 		CFG_DESIGN_CAPACITY 	2000L	//电池设计容量
		#define 		CFG_CURR_RES	137 //(417/3)	//电流采样电阻	0.01mΩ
		#define 		CFG_Cell_NUM	15
		#define		CFG_TEMP_NUM 	2
		#define FACTURECODE 	GTDS_FAC_ND									//02
		#define HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS)      //	//1 + 00 + 0
		#define HARDCODE_S	(DEF_48V_NUM*100+DEF_20_NUM)	//0 + 2
	#endif


	#if(PROJECTCODE==ND_4816)

		#define 		CFG_DESIGN_CAPACITY 	1600L	//电池设计容量
		#define 		CFG_CURR_RES	106 //(417/3)	//电流采样电阻	0.01mΩ
		#define 		CFG_Cell_NUM	15
		#define 	  CFG_TEMP_NUM	2
		#define     FACTURECODE 	GTDS_FAC_ND									//02
		#define     HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS)//	//1 + 00 + 0
		#define     HARDCODE_S	(DEF_48V_NUM*100+DEF_16_NUM)	//0 + 1
	#endif

	#if(PROJECTCODE==BLX_48T18)

		#define 		CFG_DESIGN_CAPACITY 	1800L	//电池设计容量
		#define 		CFG_CURR_RES	133 //(417/3)	//电流采样电阻	0.01mΩ
		#define 		CFG_Cell_NUM	15
		#define 	  	CFG_TEMP_NUM	2
		#define 		FACTURECODE 	GTDS_FAC_BLX 								//02
		#define 		HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS)//	//1 + 00 + 0
		#define 		HARDCODE_S	(DEF_48V_NUM*100+DEF_18_NUM)	//0 + 1
	#endif

	#if(PROJECTCODE==BLX_4824)

		#define 	 	CFG_DESIGN_CAPACITY 	2400L	 //电池设计容量
		#define 	 	CFG_CURR_RES	100 //(417/3)	//电流采样电阻	0.01mΩ
		#define 	 	CFG_Cell_NUM	15
		#define		CFG_TEMP_NUM 	2
		#define 		FACTURECODE 	GTDS_FAC_BLX   							//01
		#define 		HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS)//	//1 + 00 + 0
		#define 		HARDCODE_S		(DEF_48V_NUM*100+DEF_24_NUM)	//0 + 3
	//软件版本
#ifndef TestRWSERVIN
#define RWSERVIN  102
#else
#define RWSERVIN  TestRWSERVIN
#endif
#endif
#endif
	
	#if(PROJECTCODE==BLX_6020)
		#define 		CFG_BTAUSEFLAG	 0x00FFFFF
		#define 	 	CFG_DESIGN_CAPACITY 	2000L	 //电池设计容量2Ah
		#define 	 	CFG_CURR_RES	100 //(417/3)	//电流采样电阻	0.01mΩ
		#define 	 	CFG_Cell_NUM	20
		#define		CFG_TEMP_NUM 	2
		#define 		FACTURECODE 	GTDS_FAC_BLX   							//01
		#define 		HARDCODE_M	(XIN_G*100+X_C8T6*10+S_SINO)//	//1 + 00 + 0
		#define 		HARDCODE_S		(DEF_60V_NUM*100+DEF_20_NUM)	//0 + 3
		//软件版本
	#ifndef TestRWSERVIN
		#define RWSERVIN  100
	#else
		#define RWSERVIN  TestRWSERVIN
	#endif
		//前端芯片个数
		#define CFG_SHUSENUM  2

#endif
	
		
	#if(PROJECTCODE==BLX_7220)
		#define 		CFG_BTAUSEFLAG   0x1CFFFFF
		#define 	 	CFG_DESIGN_CAPACITY 	2000L	 //电池设计容量
		#define 	 	CFG_CURR_RES	100//(417/3)	//电流采样电阻	0.01mΩ
		#define 	 	CFG_Cell_NUM	23
		#define		CFG_TEMP_NUM 	2
		#define 		FACTURECODE 	GTDS_FAC_BLX   							//01
		#define 		HARDCODE_M	(XIN_G*100+X_C8T6*10+S_SINO)//	//1 + 00 + 0
		#define 		HARDCODE_S		(DEF_72V_NUM*100+DEF_20_NUM)	//0 + 3
		//软件版本
	#ifndef TestRWSERVIN
		#define RWSERVIN  100
	#else
		#define RWSERVIN  TestRWSERVIN
	#endif
		//前端芯片个数
		#define CFG_SHUSENUM  3
#endif
#if 0
	#if(PROJECTCODE==BLX_48T30)

		#define 		CFG_DESIGN_CAPACITY 	3000L	 //电池设计容量
		#define 		CFG_CURR_RES	133 //(417/3)	//电流采样电阻	0.01mΩ
		#define 		CFG_Cell_NUM	15
		#define 		CFG_TEMP_NUM	2
		#define 		FACTURECODE 	GTDS_FAC_BLX							//01
		#define 		HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS)//	//1 + 00 + 0
		#define 		HARDCODE_S		(DEF_48V_NUM*100+DEF_30_NUM)	//0 + 3
	#endif
	#if((PROJECTCODE==BLX_48T20)||(PROJECTCODE==BLX_48Z20))

		#define 	 	CFG_DESIGN_CAPACITY 	2000L	 //电池设计容量
		#define 	 	CFG_CURR_RES	133 //(417/3)	//电流采样电阻	0.01mΩ
		#define 	 	CFG_Cell_NUM	15
		#define		  CFG_TEMP_NUM 	2
		#define     FACTURECODE 	GTDS_FAC_BLX
		#define     HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS)
		#define     HARDCODE_S	(DEF_48V_NUM*100+DEF_20_NUM)
	#endif


	#if(PROJECTCODE==BLX_48n12)

		#define 	 	CFG_DESIGN_CAPACITY 	1200L	 //电池设计容量
		#define 	 	CFG_CURR_RES	176 //5mO4P//电流采样电阻	0.01mΩ
		#define 	 	CFG_Cell_NUM	15
		#define		CFG_TEMP_NUM 	2
		#define FACTURECODE 	GTDS_FAC_BLX          //  1
		#define HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS) //         1+0+0
		#define HARDCODE_S	(DEF_48V_NUM*100+DEF_12_NUM) //   0 +0

	#endif

	#if(PROJECTCODE==BLX_48Z12)

		#define 	 	CFG_DESIGN_CAPACITY 	1200L	 //电池设计容量
		#define 	 	CFG_CURR_RES	176 //(417/3)	//电流采样电阻	0.01mΩ
		#define 	 	CFG_Cell_NUM	15
		#define		CFG_TEMP_NUM 	2
		#define 		CFG_SET_IDFUN  1
		#define FACTURECODE 	GTDS_FAC_BLX           //       1
		#define HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS) //       1
		#define HARDCODE_S	(DEF_48V_NUM*100+DEF_X1_NUM) //       0+55
	#endif

	
	#if(PROJECTCODE==PH_4816_3M_5O)

		#define 		CFG_DESIGN_CAPACITY 	1600L	 //电池设计容量
		#define 		CFG_CURR_RES	178 //(417/3)	//电流采样电阻	0.01mΩ
		#define 		CFG_Cell_NUM	15
		#define 		CFG_TEMP_NUM	2
		#define 		FACTURECODE 	GTDS_FAC_PH                   //   6
		#define 		HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS)		//   1   0    0
		#define 		HARDCODE_S		(DEF_48V_NUM*100+DEF_16_NUM)  //   1   0    1
	#endif
	#if(PROJECTCODE==PH_4820)

		#define 		CFG_DESIGN_CAPACITY 	2000L	 //电池设计容量
		#define 		CFG_CURR_RES	137 //(417/3)	//电流采样电阻	0.01mΩ
		#define 		CFG_Cell_NUM	15
		#define 		CFG_TEMP_NUM	2
		#define 		FACTURECODE 	GTDS_FAC_PH
		#define 		HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS)
		#define 		HARDCODE_S		(DEF_48V_NUM*100+DEF_20_NUM)
	#endif
#endif
#else
		#define		CFG_DESIGN_CAPACITY 	2	 //电池设计容量
		#define 		CFG_CURR_RES	137 //(417/3)	//电流采样电阻  0.01mΩ
		#define		CFG_Cell_NUM	15
		#define		CFG_TEMP_NUM 2
		#define FACTURECODE 	0
		#define HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS)
		#define HARDCODE_S	255
#endif

#ifndef RWSERVIN 
#define RWSERVIN 101
#endif
/************************System Macro************************************/
#define DEF_CAP (CFG_DESIGN_CAPACITY/100L)//充满容量  20
#define DESIGN_CAP (DEF_CAP*3600*50*100)//20*3600*50*100

// 
#define  CFG_ST_OCC  20000
#define  DEF_OCC_TIME		20 //100ms 周期

#define  CFG_ST_ODC  60000
#define  DEF_ODC_TIME		20 //100ms 周期
#define  CFG_ST_ODC2  70000
#define  DEF_ODC2_TIME		10 //10ms 周期


//2.7保护延时
#if(FACTURECODE==GTDS_FAC_BLX)
	#define CFG_2700_T 600 //   
#else
	#define CFG_2700_T 300 //1800// 
#endif
//休眠延时
#define DEF_SLEEP_sec  (30)

#define RTC_WAKE_SEC  300 //RTC唤醒秒数20 // 2  // 




//均衡参数
#if(FACTURECODE == GTDS_FAC_ND)
#define  CFG_BalanceOpenDiV  50
#define CFG_BalanceCloseDiV  20
#define CFG_BalanceOpenVol  3510
#else
#define  CFG_BalanceOpenDiV  10
#define CFG_BalanceCloseDiV  5
#define CFG_BalanceOpenVol  3410
#endif
#define			CFG_BAL_TMAX	  (60+40)			/*均衡开启最高电池温度*/

//SOC置满清零参数
#define			CFG_SOC_SET_SV					3600	/*SOC置满最高单体电压(mV)停止积分*/
#define			CFG_SOC_SET_VMAX					5000	/*SOC置满最高单体电压范围高(mV)*/
#define			CFG_SOC_SET_VMIN					3620	/*SOC置满最高单体电压范围低(mV)*/

#define			CFG_SOC_CLEAR_SV				2350	/*SOC清零最低单体电压(mV)停止积分*/
#define			CFG_SOC_CLEAR_VMAX					2350	/*SOC清零最高单体电压范围高(mV)*/
#define			CFG_SOC_CLEAR_VMIN					1000	/*SOC清零最高单体电压范围低(mV)*/

//不同工作模式下的休眠条件--
//外部激活输出模式（0x02）休眠
//外部激活+命令确认模式（0x03）休眠

#define			CFG_SLP_VMAX			3550	/*最高单体电压(mV)*/
#define			CFG_SLP_VMIN			2600	/*最低单体电压(mV)*/
#define			CFG_SLP_C				150	/*充放电电流(mA)*/
#define			CFG_SLP_VMUTANT		200	/*电压突变(mV)*/
#define			DEF_SMALL_NoShow		150
//深度休眠最低单体电压--
#define			CFG_DSLP_VMIN			2300	/*电压突变(mV)*/

/*************************报警参数值*******************************/


#define			CFG_NOTALLOWCHG_UV				1500  // 低压不允许充电电压阈值
#define			CFG_VOLTDIFF_RANGE_L				2900  //压差在这区间判断
#define			CFG_VOLTDIFF_RANGE_H				3500 


#if(FACTURECODE==GTDS_FAC_ND)
	#define 	CFG_ST_OV 		3700
	#define 	CFG_ST_OVR 		3350
	#define 	CFG_Sof_UV		2700//单体欠压保护
	#define 	CFG_Sof_UVR		3000//单体欠压保护恢复
	#define 	Def_Sof_UTC		(-10)
	#define 	Def_Sof_UTCR	(-5) 
	#define 	Def_Sof_UTD		(-25) 
	#define 	Def_Sof_UTDR	(-20) 
	
	#define 	Def_SH_OTC 		(55)
	#define 	Def_SH_OTCR		(50)
	#define 	Def_SH_UTC		(-35)
	#define 	Def_SH_UTCR	 	(-30) 
	#define 	Def_SH_OTD	 	(65) 
	#define 	Def_SH_OTDR  	(60) 
	#define 	Def_SH_UTD	 	(-35) 
	#define 	Def_SH_UTDR 	(-30) 
	#define 	CFG_UV_T		3
	#define 	CFG_OV_T		2

#elif(FACTURECODE==GTDS_FAC_BLX)
	#define 	CFG_ST_OV 		3750
	#define 	CFG_ST_OVR 		3450
	#define 	CFG_Sof_UV		2700//单体欠压保护
	#define 	CFG_Sof_UVR		3000//单体欠压保护恢复
	#define 	Def_Sof_UTC		(-10)
	#define 	Def_Sof_UTCR	(-7) 
	#define 	Def_Sof_UTD		(-25) 
	#define 	Def_Sof_UTDR	(-15) 
	
	#define 	Def_SH_OTD		(70) 
	#define 	Def_SH_OTDR		(60) 
	#define 	Def_SH_OTC 		(60)
	#define 	Def_SH_OTCR		(50)
	#define 	Def_SH_UTC		(-35)
	#define 	Def_SH_UTCR		(-30) 
	#define 	Def_SH_UTD		(-35) 
	#define 	Def_SH_UTDR		(-30) 
	#define 	CFG_UV_T		4
	#define 	CFG_UV3_T		4
	#define 	CFG_OV_T		2
#else
	#define 	CFG_ST_OV 		3650
	#define 	CFG_ST_OVR 		3450
	#define 	CFG_Sof_UV		2700//单体欠压保护
	#define 	CFG_Sof_UVR		2800//单体欠压保护恢复
	#define 	Def_Sof_UTC		(-5)
	#define 	Def_Sof_UTCR	(0) 
	#define 	Def_Sof_UTD		(-25) 
	#define 	Def_Sof_UTDR	(-15) 
	
	#define 	Def_SH_OTC 		(55)
	#define 	Def_SH_OTCR		(45)
	#define 	Def_SH_UTC		(-35) 
	#define 	Def_SH_UTCR		(-30)  
	
	#define 	Def_SH_OTD		(65) 
	#define 	Def_SH_OTDR		(60) 
	#define 	Def_SH_UTD	 	(-35) 
	#define 	Def_SH_UTDR 	(-30)  
	#define 	CFG_UV_T		5
	#define 	CFG_OV_T		2
#endif

#define 		CFG_ST_UV     2300
#define 		CFG_ST_UVR    2400
#define 		CFG_S0f_UVR   2800
#define 		CFG_ST_UV3				1500//禁止充电保护mV
#if(PROJECTCODE==BLX_48T30)
	#define 		CFG_ST_OTM				150//(105+40)MOS高温保护
	#define 		CFG_ST_OTMR			120//(70+40)MOS高温保护恢复
#else
	#define 		CFG_ST_OTM				130//(85+40)MOS高温保护
	#define 		CFG_ST_OTMR			115//(70+40)MOS高温保护恢复
#endif
#define 		CFG_ST_CME				500//充电回路MOS断开后，仍有充电电流＞	
#define 		CFG_ST_DME				500//fang电回路MOS断开后，仍有充电电流＞	

/************************System Macro End************************************/

void SysParaInit(void);
#endif




