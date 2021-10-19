#include "main.h"


#if(PROJECTCODE== CF_48N12T5_40 )
	#define  PACK_6012_TB
	int8_t LONGLineRArr[]={50,10,30,-5,0,-7,58,0,0,0,0,0,0,0,9,0,0,0,0,0,0,0,0,0,0};
#elif(PROJECTCODE== CF_48N12T5_40A )
	#define  PACK_6012_TB
	int8_t LONGLineRArr[]={12,0,0,0,0,0,0,-5,26,0,0,0,0,0,17,0,0,0,0,0,0,0,0,0,0};
#elif(PROJECTCODE== CF_48T20T5_40 )
	#define  PACK_6012_TB
	int8_t LONGLineRArr[]={9,0,0,-3,48,0,0,-3,62,0,0,11,13,-3,0,0,0,0,0,0,0,0,0,0};
#elif(PROJECTCODE== CF_48T16T5_40 )
	#define  PACK_6012_TB
	int8_t LONGLineRArr[]={7,0,0,0,43,0,0,0,47,0,0,0,27,0,0,0,0,0,0,0,0,0,0,0,0};
#elif(PROJECTCODE==CF_48Z16T5_40A)
	#define  PACK_6012_TB
	int8_t LONGLineRArr[]={7,0,0,0,43,0,0,0,47,0,0,0,27,0,0,0,0,0,0,0,0,0,0};
#elif(PROJECTCODE== CF_48Z20T5_40 )
	#define  PACK_6012_TB
	int8_t LONGLineRArr[]={-8,3,0,3,-8,17,0,0,0,4,6,3,0,3,-7,0,0,0,0,0,0,0,0,0,0};
#elif(PROJECTCODE==CF_48T20T5_40A)
	#define  PACK_6012_TB
	int8_t LONGLineRArr[]={0,0,0,18,17,0,0,0,0,0,0,17,18,0,0,0,0,0,0,0,0,0,0,0,0};
#elif(PROJECTCODE==CF_48Z24T5_40)
	#define  PACK_6012_TB
	int8_t LONGLineRArr[]={-11,0,-4,1,-10,14,-1,-1,-1,1,4,1,-4,1,-9,0,0,0,0,0,0,0,0,0,0};
#elif(PROJECTCODE==BLX_48n12)
	#define  PACK_6012_TB
	int8_t  LONGLineRArr[]={7,0,0,0,0,7,0,0,0,4,4,3,0,0,5,0,0,0,0,0,0,0,0,0,0};
#else 
	int8_t LONGLineRArr[]={ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
#endif



void Update_CellVolt(void)
{
	uint8_t i = 0,k=0;
	uint16_t tmpVolt[25];
	uint16_t max = 0;
	uint16_t min = 50000;
	uint32_t total = 0;
	SH_StrlVCAD(1);  //开采样
	SH_GetCellVolt(tmpVolt);
	SH_StrlVCAD(0);  //关采样
#ifdef DBG_Printf_Volt
	printf("ALL Cell : ");
#endif
	for( i = 0; i <25; i++ )
	{	
		if((CFG_BTAUSEFLAG&(1<<i))==0)  //20bit，本次需要采集的电芯只有20个
		{
			continue;
		}
		syspara.batVolt[k]= tmpVolt[i] ;   //不带标定的数据
#ifdef  PACK_6012_TB
		syspara.cellVolt[k] = tmpVolt[i]+ (int16_t)((syspara.current*LONGLineRArr[k]/10+500)/1000)+flashpara.VoltRate[k];	
#else
		syspara.cellVolt[k] = tmpVolt[i]+flashpara.VoltRate[k];	//带标定的数据 
#endif

#ifdef DBG_Printf_Volt
printf("%d ",syspara.cellVolt[k]);
#endif
		if (max < syspara.cellVolt[k])
		{
			max = syspara.cellVolt[k];
			syspara.voltMaxNum= i;
		}
		if (syspara.cellVolt[k] < min)
		{
			min = syspara.cellVolt[k];
		}
		total += syspara.cellVolt[k];
		k++;
	}
	syspara.totalSumVolt = total;
	syspara.voltMax = max;
	syspara.voltMin = min;
	syspara.voltDiff = max-min;
	syspara.voltAvg = syspara.totalSumVolt/CFG_Cell_NUM;

#ifdef DBG_Printf_Volt
printf("Tv= %d  Max= %d min= %d avg= %d ",syspara.totalSumVolt,syspara.voltMax,syspara.voltMin,syspara.voltAvg);
	
printf("\r\n");
#endif
}
void battstatusProcess(void)
{
		static uint16_t maxVol_value= 0;
		static uint16_t minVol_value= 0;
		static uint8_t VoltChargeBigTime= 0;
		if((abs(maxVol_value-syspara.voltMax)>200)||(abs(minVol_value-syspara.voltMin)>200))
		{
				sysflag.bits.VoltChargeBigFlag=1;
				if(VoltChargeBigTime<TimeOut(5))
				{
						VoltChargeBigTime++;
				}
				else
				{
					maxVol_value=syspara.voltMax;
					minVol_value=syspara.voltMin;
					VoltChargeBigTime= 0;
				}
		}
		else
		{
				sysflag.bits.VoltChargeBigFlag=0;
				VoltChargeBigTime=0;
				maxVol_value=syspara.voltMax;
				minVol_value=syspara.voltMin;
		}
}


#define  DEF_BALANCE_Mask1 0x9249
#define  DEF_BALANCE_Mask2 0x2492
#define  DEF_BALANCE_Mask3 0x4924
//1.充电状态；只有充电状态下，才可以开启均衡，均衡的过程是放高电压电给电阻
//2.电压最低的电芯电压≥3.05V；
//3.电芯电压＞平均电压+10mV；
//4.最高电池温度＜60℃。
uint8_t JudgeBalance(uint32_t* bFlag)
{
	uint8_t enBal = 0;
	uint8_t i = 0,cellVoltMoreFlag = 0,k=0;
	uint32_t tmpMask = 0;
	static uint8_t BalanceAllow= 0;
	//压差判断
	if(abs( syspara.voltMax- syspara.voltMin ) >= CFG_BalanceOpenDiV) 		
	{
		BalanceAllow = 1;
	}
	else if(abs( syspara.voltMax - syspara.voltMin ) <CFG_BalanceCloseDiV )
	{
		BalanceAllow = 0;
	}
	if(abs( syspara.voltMax - syspara.voltMin ) >= 1000 ) 
	{
		BalanceAllow = 0;  //压差超过1v也不均
	}
	//电压大于最小均衡电压
	for(i= 0;i<CFG_Cell_NUM;i++)
	{
		if((syspara.cellVolt[i] > CFG_BalanceOpenVol)&&(syspara.cellVolt[i] > syspara.voltAvg))
		{
			cellVoltMoreFlag = 1;
			SetBit(tmpMask,i);  //将高于平均电压的电芯置位，然后将高电压电芯往板子的电阻上放电均衡
		}			
	}

	//充电状态  温度小于均衡温度
	if((( syspara.batStat == STATE_CHG )/*||((sysflag.bits.DET_Flag == 1)&&(syspara.chgMos==0))*/)
		&&(syspara.tempMax < CFG_BAL_TMAX)
		&&(cellVoltMoreFlag==1)
		&&(BalanceAllow == 1))
	{
		enBal = 1;
	}
	*bFlag = 0;
	for(i= 0;i<25;i++)
	{
		if((CFG_BTAUSEFLAG&(1<<i))!=0)
		{
			if((tmpMask&(1<<k))!=0)
			{
				SetBit(*bFlag,i);
			}
			k++;
		}
	}
#ifdef DBG_Printf_Balance
	printf("balanceCtrl %x %d\r\n",*bFlag,enBal);
#endif
	return enBal;
}


//均衡控制
void BalanceCtrl( void )
{
	
	uint32_t balBitSet = 0;
	if(JudgeBalance(&balBitSet))
	{
		SH_BALANCESET(balBitSet);
	}
	else 
	{
		SH_BALANCESET(0);
	}
}

