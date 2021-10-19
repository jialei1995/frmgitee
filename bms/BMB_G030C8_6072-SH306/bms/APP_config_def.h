#ifndef __CONFIG_DEF_H
#define __CONFIG_DEF_H





#define DEF_APP_MASK 0x1234

//电池类型
#define B_YTPE_LiFe 1//磷酸铁锂01 
#define B_YTPE_3Yuan  2//三元02 

#define CFG_XIEYI_11 11
#define CFG_XIEYI_12 12

//厂商编码预定义
#define GTDS_FAC_BLX		1	//保力新
#define GTDS_FAC_ND		2	//南都
//#define GTDS_FAC_NULL		3	// 3  GTDS 预留不使用 防止跟F 芯片的板子更新出错
#define GTDS_FAC_GBao	4	//瑰宝
#define GTDS_FAC_CF		5	// 0、5   预留给长风项目编码百位
#define GTDS_FAC_PH		8	//鹏辉

//硬件编码预定义

#define XIN_F		0  //
#define XIN_G		1		//


#define X_F6P6 	0
#define B_F6P6 	1
#define X_C8T6	2
#define X_K6T6 	4

#define  S_SINO	0
#define  S_TI		1
#define  S_LPS	2

//电压平台预定义
#define  DEF_48V_NUM  0
#define  DEF_60V_NUM  1
#define  DEF_72V_NUM  2
//AH数预定义

#define  DEF_12_NUM  0
#define  DEF_16_NUM  1
#define  DEF_20_NUM  2
#define  DEF_24_NUM  3
#define  DEF_30_NUM  4
#define  DEF_18_NUM  5
#define  DEF_23_NUM  23
#define  DEF_X1_NUM  55


//项目编码
#define	CF_48Z12T5_40		1	//	无补偿	XCMINI
#define	CF_48Z16T5_40		2	//	无补偿	XCMINI
#define	CF_48Z20S5_40		3	//		XCMINI
#define	CF_48Z20S3_26		4	//		L5_美团
#define	CF_48Z26S3_26		5	//		L5_美团
#define	CF_48N12T5_40		6	//		小多奇    有补偿
#define	CF_48N12T5_40A		7	//		模块4812
#define	CF_48T16T5_40		8	//	有补偿	CC小果汁
#define	CF_48T20T5_40		9	//	有补偿	CC小果汁
#define	CF_48N20T5_40		10	//		模块4820
#define	CF_48N12S3_20		11	//		XC1
#define	CF_48Z20S5_50		12	//		X1
#define	CF_48Z12S3_20A		13	//		L5
#define	CF_48Z20S3_25A		14	//		L5
#define	CF_QM48T25S3_25	15	//		青梅
#define	CF_QM48Z20S3_25	16	//		青梅
#define	CF_QM48Z16S3_20	17	//		青梅
#define	CF_QM48T20S3_20	18	//		青梅
#define	CF_48Z16S3_20		19	//		XCMINI
#define	CF_48Z12S3_20		20	//		XCMINI
#define	CF_48Z20S3_20		21	//		XCMINI
#define	CF_48Z22T5_45		22	//		校园共享
#define	CF_48Z12S3_20C		23	//		小可爱
#define	CF_48T20S5_40		25	//		载重王
#define	CF_48Z20S3_25		26	//		流星X8/X1
#define	CF_48Z20S3_20L		27	//		小威客
#define	CF_48Z16T5_40A		28	//	有补偿	小布朗/XC2/高档风
#define	CF_48Z24T5_40		29	//	有补偿	小布朗/XC2/高档风
#define	CF_48Z20T5_40		30	//	有补偿	小布朗/XC2/高档风
#define	CF_48Z20S3_25B		31	//		L5
#define	CF_24V60Ah			32	//		电动船
#define	CF_QM48N12S3_20	33	//		青梅
#define	CF_QM48N15S3_25	34	//		青梅
#define	CF_48Z45T5_45		35	//		锂享租电
#define	CF_48Z16T5_40B		40	//		X1 GONGXIANG
#define	CF_48T20T5_40A		43	//		Q2


#define  BLX_48n12   	112
#define  BLX_48Z12 	155
#define  BLX_48T18   	118
#define  BLX_48T20   	120
#define  BLX_48Z20   	121
#define  BLX_4824   	124
#define  BLX_48T30   	130


#define  BLX_6020  	620
#define  BLX_6024  	624
#define  BLX_7220   	720


#define  ND_4812  		212
#define  ND_4816  		216
#define  ND_4818  		218
#define  ND_4820_4M_5O  220
#define  ND_4824  		224



#define  GB_4815_15S  	415
#define  GB_4815_16S  	416

#define  GB_4830_15S  	430
#define  GB_4830_16S  	431




#define  PH_4812  		812
#define  PH_4816_3M_5O  		816
#define  PH_4818  		818
#define  PH_4820  		820
#define  PH_4824  		824

#define TEMP_OFFSET 40


#endif




