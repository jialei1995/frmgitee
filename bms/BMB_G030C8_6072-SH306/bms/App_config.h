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
//boot �汾
#define BOOTSERVIN  	101
//Э��汾
#define CFG_XIEYI_NUM CFG_XIEYI_12
//�������
#define BATTERY_TYPE	B_YTPE_LiFe

#define TestRWSERVIN  25

//��Ŀ����
#define PROJECTCODE 	BLX_6020



#if(TEST_CYC==0)
#if 0
	#if(PROJECTCODE== CF_48Z12T5_40)
		#define 		CFG_DESIGN_CAPACITY		1200L	 //����������
		#define 		CFG_CURR_RES	141 //(417/3)	//������������  0.01m��
		#define		CFG_Cell_NUM	15
		#define		CFG_TEMP_NUM 2
		#define	FACTURECODE 	GTDS_FAC_CF
		#define	HARDCODE_M  (XIN_G*100+X_F6P6*10+S_LPS)
		#define	HARDCODE_S 	(DEF_48V_NUM*100+DEF_12_NUM)
	#endif

	#if(PROJECTCODE== CF_48N12T5_40A)
		#define 		CFG_DESIGN_CAPACITY		1200L	 //����������
		#define 		CFG_CURR_RES	253 //(417/3)	//������������  0.01m��
		#define		CFG_Cell_NUM	15
		#define		CFG_TEMP_NUM 1
		#define FACTURECODE 	GTDS_FAC_CF
		#define HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS)
		#define HARDCODE_S	(DEF_48V_NUM*100+DEF_12_NUM)
	#endif

	#if(PROJECTCODE==CF_48N12T5_40)
		#define 		CFG_DESIGN_CAPACITY		1200L	 //����������
		#define 		CFG_CURR_RES	139 //(417/3)	//������������  0.01m��
		#define		CFG_Cell_NUM	15
		#define		CFG_TEMP_NUM 2
		#define FACTURECODE 	GTDS_FAC_CF
		#define HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS)
		#define HARDCODE_S	(DEF_48V_NUM*100+DEF_12_NUM)
	#endif
					
	#if(PROJECTCODE==CF_48T20T5_40A)
		#define 		CFG_DESIGN_CAPACITY		2000L	 //����������
		#define 		CFG_CURR_RES	139 //(417/3)	//������������  0.01m��
		#define		CFG_Cell_NUM	15
		#define		CFG_TEMP_NUM 2
		#define FACTURECODE 	GTDS_FAC_CF
		#define HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS)
		#define HARDCODE_S	(DEF_48V_NUM*100+DEF_20_NUM)
	#endif
	

	
	#if(PROJECTCODE== CF_48Z24T5_40)
		#define 		CFG_DESIGN_CAPACITY		2400L	 //����������
		#define 		CFG_CURR_RES	137 //(400/4)	//������������  0.01m��
		#define		CFG_Cell_NUM	15
		#define		CFG_TEMP_NUM 2
		#define FACTURECODE 	GTDS_FAC_CF
		#define HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS)
		#define HARDCODE_S	(DEF_48V_NUM*100+DEF_24_NUM)

	#endif
	#if(PROJECTCODE== CF_48Z16T5_40A)
		#define		CFG_DESIGN_CAPACITY 	1600L	 //����������
		#define 		CFG_CURR_RES	137//(417/3)	//������������  0.01m��
		#define		CFG_Cell_NUM	15
		#define		CFG_TEMP_NUM 2
		#define FACTURECODE 	GTDS_FAC_CF
		#define HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS)
		#define HARDCODE_S	(DEF_48V_NUM*100+DEF_16_NUM)
	#endif
	
	
	
	#if(PROJECTCODE==CF_48Z16T5_40)
		#define 	CFG_DESIGN_CAPACITY 	1600L	 //����������
		#define 	CFG_CURR_RES	139//(417/3)	//������������	0.01m��
		#define 	CFG_Cell_NUM	15
		#define	CFG_TEMP_NUM 2
		#define FACTURECODE 	GTDS_FAC_CF
		#define HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS)
		#define HARDCODE_S	(DEF_48V_NUM*100+DEF_16_NUM)
	#endif
	
	#if(PROJECTCODE== CF_48Z16T5_40B)
		#define	CFG_DESIGN_CAPACITY 	1600L	 //����������
		#define 	CFG_CURR_RES	141//(417/3)	//������������  0.01m��
		#define	CFG_Cell_NUM	15
		#define	CFG_TEMP_NUM 2
		#define 	CFG_SET_IDFUN  1
		#define FACTURECODE 	GTDS_FAC_CF
		#define HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS)
		#define HARDCODE_S	(DEF_48V_NUM*100+DEF_16_NUM)
	#endif
	#if(PROJECTCODE==CF_48Z20T5_40)
		#define		CFG_DESIGN_CAPACITY 	2000L	 //����������
		#define 		CFG_CURR_RES	139 //(417/3)	//������������  0.01m��
		#define		CFG_Cell_NUM	15
		#define		CFG_TEMP_NUM 2
		#define FACTURECODE 	GTDS_FAC_CF
		#define HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS)
		#define HARDCODE_S	(DEF_48V_NUM*100+DEF_20_NUM)
	#endif

	#if(PROJECTCODE==CF_48T16T5_40)
		#define		CFG_DESIGN_CAPACITY 	1600L	 //����������
		#define 		CFG_CURR_RES	141//(417/3)	//������������  0.01m��
		#define		CFG_Cell_NUM	15
		#define		CFG_TEMP_NUM 2
		#define FACTURECODE 	GTDS_FAC_CF
		#define HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS)
		#define HARDCODE_S	(DEF_48V_NUM*100+DEF_16_NUM)
	#endif
	#if(PROJECTCODE==CF_48T20T5_40)
		#define		CFG_DESIGN_CAPACITY 	2000L	 //����������
		#define 		CFG_CURR_RES	139 //(417/3)	//������������  0.01m��
		#define		CFG_Cell_NUM	15
		#define		CFG_TEMP_NUM 2
		#define FACTURECODE 	GTDS_FAC_CF
		#define HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS)
		#define HARDCODE_S	(DEF_48V_NUM*100+DEF_20_NUM)
	#endif


	#if(PROJECTCODE==ND_4820_4M_5O)

		#define 		CFG_DESIGN_CAPACITY 	2000L	//����������
		#define 		CFG_CURR_RES	137 //(417/3)	//������������	0.01m��
		#define 		CFG_Cell_NUM	15
		#define		CFG_TEMP_NUM 	2
		#define FACTURECODE 	GTDS_FAC_ND									//02
		#define HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS)      //	//1 + 00 + 0
		#define HARDCODE_S	(DEF_48V_NUM*100+DEF_20_NUM)	//0 + 2
	#endif


	#if(PROJECTCODE==ND_4816)

		#define 		CFG_DESIGN_CAPACITY 	1600L	//����������
		#define 		CFG_CURR_RES	106 //(417/3)	//������������	0.01m��
		#define 		CFG_Cell_NUM	15
		#define 	  CFG_TEMP_NUM	2
		#define     FACTURECODE 	GTDS_FAC_ND									//02
		#define     HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS)//	//1 + 00 + 0
		#define     HARDCODE_S	(DEF_48V_NUM*100+DEF_16_NUM)	//0 + 1
	#endif

	#if(PROJECTCODE==BLX_48T18)

		#define 		CFG_DESIGN_CAPACITY 	1800L	//����������
		#define 		CFG_CURR_RES	133 //(417/3)	//������������	0.01m��
		#define 		CFG_Cell_NUM	15
		#define 	  	CFG_TEMP_NUM	2
		#define 		FACTURECODE 	GTDS_FAC_BLX 								//02
		#define 		HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS)//	//1 + 00 + 0
		#define 		HARDCODE_S	(DEF_48V_NUM*100+DEF_18_NUM)	//0 + 1
	#endif

	#if(PROJECTCODE==BLX_4824)

		#define 	 	CFG_DESIGN_CAPACITY 	2400L	 //����������
		#define 	 	CFG_CURR_RES	100 //(417/3)	//������������	0.01m��
		#define 	 	CFG_Cell_NUM	15
		#define		CFG_TEMP_NUM 	2
		#define 		FACTURECODE 	GTDS_FAC_BLX   							//01
		#define 		HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS)//	//1 + 00 + 0
		#define 		HARDCODE_S		(DEF_48V_NUM*100+DEF_24_NUM)	//0 + 3
	//����汾
#ifndef TestRWSERVIN
#define RWSERVIN  102
#else
#define RWSERVIN  TestRWSERVIN
#endif
#endif
#endif
	
	#if(PROJECTCODE==BLX_6020)
		#define 		CFG_BTAUSEFLAG	 0x00FFFFF
		#define 	 	CFG_DESIGN_CAPACITY 	2000L	 //����������2Ah
		#define 	 	CFG_CURR_RES	100 //(417/3)	//������������	0.01m��
		#define 	 	CFG_Cell_NUM	20
		#define		CFG_TEMP_NUM 	2
		#define 		FACTURECODE 	GTDS_FAC_BLX   							//01
		#define 		HARDCODE_M	(XIN_G*100+X_C8T6*10+S_SINO)//	//1 + 00 + 0
		#define 		HARDCODE_S		(DEF_60V_NUM*100+DEF_20_NUM)	//0 + 3
		//����汾
	#ifndef TestRWSERVIN
		#define RWSERVIN  100
	#else
		#define RWSERVIN  TestRWSERVIN
	#endif
		//ǰ��оƬ����
		#define CFG_SHUSENUM  2

#endif
	
		
	#if(PROJECTCODE==BLX_7220)
		#define 		CFG_BTAUSEFLAG   0x1CFFFFF
		#define 	 	CFG_DESIGN_CAPACITY 	2000L	 //����������
		#define 	 	CFG_CURR_RES	100//(417/3)	//������������	0.01m��
		#define 	 	CFG_Cell_NUM	23
		#define		CFG_TEMP_NUM 	2
		#define 		FACTURECODE 	GTDS_FAC_BLX   							//01
		#define 		HARDCODE_M	(XIN_G*100+X_C8T6*10+S_SINO)//	//1 + 00 + 0
		#define 		HARDCODE_S		(DEF_72V_NUM*100+DEF_20_NUM)	//0 + 3
		//����汾
	#ifndef TestRWSERVIN
		#define RWSERVIN  100
	#else
		#define RWSERVIN  TestRWSERVIN
	#endif
		//ǰ��оƬ����
		#define CFG_SHUSENUM  3
#endif
#if 0
	#if(PROJECTCODE==BLX_48T30)

		#define 		CFG_DESIGN_CAPACITY 	3000L	 //����������
		#define 		CFG_CURR_RES	133 //(417/3)	//������������	0.01m��
		#define 		CFG_Cell_NUM	15
		#define 		CFG_TEMP_NUM	2
		#define 		FACTURECODE 	GTDS_FAC_BLX							//01
		#define 		HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS)//	//1 + 00 + 0
		#define 		HARDCODE_S		(DEF_48V_NUM*100+DEF_30_NUM)	//0 + 3
	#endif
	#if((PROJECTCODE==BLX_48T20)||(PROJECTCODE==BLX_48Z20))

		#define 	 	CFG_DESIGN_CAPACITY 	2000L	 //����������
		#define 	 	CFG_CURR_RES	133 //(417/3)	//������������	0.01m��
		#define 	 	CFG_Cell_NUM	15
		#define		  CFG_TEMP_NUM 	2
		#define     FACTURECODE 	GTDS_FAC_BLX
		#define     HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS)
		#define     HARDCODE_S	(DEF_48V_NUM*100+DEF_20_NUM)
	#endif


	#if(PROJECTCODE==BLX_48n12)

		#define 	 	CFG_DESIGN_CAPACITY 	1200L	 //����������
		#define 	 	CFG_CURR_RES	176 //5mO4P//������������	0.01m��
		#define 	 	CFG_Cell_NUM	15
		#define		CFG_TEMP_NUM 	2
		#define FACTURECODE 	GTDS_FAC_BLX          //  1
		#define HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS) //         1+0+0
		#define HARDCODE_S	(DEF_48V_NUM*100+DEF_12_NUM) //   0 +0

	#endif

	#if(PROJECTCODE==BLX_48Z12)

		#define 	 	CFG_DESIGN_CAPACITY 	1200L	 //����������
		#define 	 	CFG_CURR_RES	176 //(417/3)	//������������	0.01m��
		#define 	 	CFG_Cell_NUM	15
		#define		CFG_TEMP_NUM 	2
		#define 		CFG_SET_IDFUN  1
		#define FACTURECODE 	GTDS_FAC_BLX           //       1
		#define HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS) //       1
		#define HARDCODE_S	(DEF_48V_NUM*100+DEF_X1_NUM) //       0+55
	#endif

	
	#if(PROJECTCODE==PH_4816_3M_5O)

		#define 		CFG_DESIGN_CAPACITY 	1600L	 //����������
		#define 		CFG_CURR_RES	178 //(417/3)	//������������	0.01m��
		#define 		CFG_Cell_NUM	15
		#define 		CFG_TEMP_NUM	2
		#define 		FACTURECODE 	GTDS_FAC_PH                   //   6
		#define 		HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS)		//   1   0    0
		#define 		HARDCODE_S		(DEF_48V_NUM*100+DEF_16_NUM)  //   1   0    1
	#endif
	#if(PROJECTCODE==PH_4820)

		#define 		CFG_DESIGN_CAPACITY 	2000L	 //����������
		#define 		CFG_CURR_RES	137 //(417/3)	//������������	0.01m��
		#define 		CFG_Cell_NUM	15
		#define 		CFG_TEMP_NUM	2
		#define 		FACTURECODE 	GTDS_FAC_PH
		#define 		HARDCODE_M	(XIN_G*100+X_F6P6*10+S_LPS)
		#define 		HARDCODE_S		(DEF_48V_NUM*100+DEF_20_NUM)
	#endif
#endif
#else
		#define		CFG_DESIGN_CAPACITY 	2	 //����������
		#define 		CFG_CURR_RES	137 //(417/3)	//������������  0.01m��
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
#define DEF_CAP (CFG_DESIGN_CAPACITY/100L)//��������  20
#define DESIGN_CAP (DEF_CAP*3600*50*100)//20*3600*50*100

// 
#define  CFG_ST_OCC  20000
#define  DEF_OCC_TIME		20 //100ms ����

#define  CFG_ST_ODC  60000
#define  DEF_ODC_TIME		20 //100ms ����
#define  CFG_ST_ODC2  70000
#define  DEF_ODC2_TIME		10 //10ms ����


//2.7������ʱ
#if(FACTURECODE==GTDS_FAC_BLX)
	#define CFG_2700_T 600 //   
#else
	#define CFG_2700_T 300 //1800// 
#endif
//������ʱ
#define DEF_SLEEP_sec  (30)

#define RTC_WAKE_SEC  300 //RTC��������20 // 2  // 




//�������
#if(FACTURECODE == GTDS_FAC_ND)
#define  CFG_BalanceOpenDiV  50
#define CFG_BalanceCloseDiV  20
#define CFG_BalanceOpenVol  3510
#else
#define  CFG_BalanceOpenDiV  10
#define CFG_BalanceCloseDiV  5
#define CFG_BalanceOpenVol  3410
#endif
#define			CFG_BAL_TMAX	  (60+40)			/*���⿪����ߵ���¶�*/

//SOC�����������
#define			CFG_SOC_SET_SV					3600	/*SOC������ߵ����ѹ(mV)ֹͣ����*/
#define			CFG_SOC_SET_VMAX					5000	/*SOC������ߵ����ѹ��Χ��(mV)*/
#define			CFG_SOC_SET_VMIN					3620	/*SOC������ߵ����ѹ��Χ��(mV)*/

#define			CFG_SOC_CLEAR_SV				2350	/*SOC������͵����ѹ(mV)ֹͣ����*/
#define			CFG_SOC_CLEAR_VMAX					2350	/*SOC������ߵ����ѹ��Χ��(mV)*/
#define			CFG_SOC_CLEAR_VMIN					1000	/*SOC������ߵ����ѹ��Χ��(mV)*/

//��ͬ����ģʽ�µ���������--
//�ⲿ�������ģʽ��0x02������
//�ⲿ����+����ȷ��ģʽ��0x03������

#define			CFG_SLP_VMAX			3550	/*��ߵ����ѹ(mV)*/
#define			CFG_SLP_VMIN			2600	/*��͵����ѹ(mV)*/
#define			CFG_SLP_C				150	/*��ŵ����(mA)*/
#define			CFG_SLP_VMUTANT		200	/*��ѹͻ��(mV)*/
#define			DEF_SMALL_NoShow		150
//���������͵����ѹ--
#define			CFG_DSLP_VMIN			2300	/*��ѹͻ��(mV)*/

/*************************��������ֵ*******************************/


#define			CFG_NOTALLOWCHG_UV				1500  // ��ѹ���������ѹ��ֵ
#define			CFG_VOLTDIFF_RANGE_L				2900  //ѹ�����������ж�
#define			CFG_VOLTDIFF_RANGE_H				3500 


#if(FACTURECODE==GTDS_FAC_ND)
	#define 	CFG_ST_OV 		3700
	#define 	CFG_ST_OVR 		3350
	#define 	CFG_Sof_UV		2700//����Ƿѹ����
	#define 	CFG_Sof_UVR		3000//����Ƿѹ�����ָ�
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
	#define 	CFG_Sof_UV		2700//����Ƿѹ����
	#define 	CFG_Sof_UVR		3000//����Ƿѹ�����ָ�
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
	#define 	CFG_Sof_UV		2700//����Ƿѹ����
	#define 	CFG_Sof_UVR		2800//����Ƿѹ�����ָ�
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
#define 		CFG_ST_UV3				1500//��ֹ��籣��mV
#if(PROJECTCODE==BLX_48T30)
	#define 		CFG_ST_OTM				150//(105+40)MOS���±���
	#define 		CFG_ST_OTMR			120//(70+40)MOS���±����ָ�
#else
	#define 		CFG_ST_OTM				130//(85+40)MOS���±���
	#define 		CFG_ST_OTMR			115//(70+40)MOS���±����ָ�
#endif
#define 		CFG_ST_CME				500//����·MOS�Ͽ������г�������	
#define 		CFG_ST_DME				500//fang���·MOS�Ͽ������г�������	

/************************System Macro End************************************/

void SysParaInit(void);
#endif




