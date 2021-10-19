#include "main.h"
uint8_t	Ctrl_CH_FET= 0;
uint8_t	Ctrl_DS_FET= 0;

void MOS_Ctrl(void)
{
	S16_BITS dsgMosOffFlag;
	S16_BITS chgMosOffFlag;
	static uint8_t DSG_OpenDelay = 0;
	static uint8_t CHG_OpenDelay = 0;
	static uint8_t CHG_time = 0;
	static uint8_t DSG_time = 0;
	
	dsgMosOffFlag.bytes = 0;
	dsgMosOffFlag.bits.bit0 = userAlarm.bits.U_V_2700;//电芯<2700
	dsgMosOffFlag.bits.bit1 = userAlarm.bits.U_T_D;//放电低温
	dsgMosOffFlag.bits.bit3 = userAlarm.bits.O_T_D;//放电高温
	dsgMosOffFlag.bits.bit4 = userAlarm.bits.O_T_M;//mos高温
	dsgMosOffFlag.bits.bit5 = D_O_C.alarmFlag;//放电过流1级
	dsgMosOffFlag.bits.bit6 = D_O_C2.alarmFlag;//放电过流二级报警
	dsgMosOffFlag.bits.bit7 = S_C_D.alarmFlag;//短路报警
	dsgMosOffFlag.bits.bit8 = C_O_C.alarmFlag;//充电过流
	
	if(syspara.batStat == STATE_CHG)//如果充电
	{
		if(CHG_time>3)
		{
			dsgMosOffFlag.bytes = 0;//放电flag参数=0
		}
		else
		{
			CHG_time++;
		}
	}
	else
	{
		CHG_time=0;
	}
	if(dsgMosOffFlag.bytes) 	//有放电报警触发
	{
		syspara.IntegralPower=1050;//不能放电了把放电系数恢复
		Ctrl_DS_FET= 0;
	}
	else //没有放电报警，可以放电
	{
		//最小电压>2300 && 最大温度<105 && 最小温度>5 || 当前处于充电状态
		if(((syspara.voltMin>2300)&&(syspara.tempMax<(Def_SH_OTD+TEMP_OFFSET))&&(syspara.tempMin>(Def_SH_UTD+TEMP_OFFSET)))||(syspara.batStat == STATE_CHG))
		{
			Ctrl_DS_FET=1;//开启放电mos
		}
		else
		{
			//不进行操作
		}
	}
#ifdef DBG_Printf_MOS
	printf("DSG_Ctrl: %x %d\r\n",dsgMosOffFlag.bytes,Ctrl_DS_FET);
#endif	
	//CHG
	chgMosOffFlag.bytes = 0;
	chgMosOffFlag.bits.bit0 = userAlarm.bits.O_V;//电压过高
	chgMosOffFlag.bits.bit1 = userAlarm.bits.U_T_C;//充电温度过低
	chgMosOffFlag.bits.bit2 = userAlarm.bits.C_P_Count;//充电过压保护
	chgMosOffFlag.bits.bit3 = userAlarm.bits.O_T_C;//充电高温
	chgMosOffFlag.bits.bit4 = userAlarm.bits.O_T_M;//mos高温
	chgMosOffFlag.bits.bit5 = C_O_C.alarmFlag;//充电过流
	chgMosOffFlag.bits.bit6 = D_O_C.alarmFlag;//放电过流1级
	chgMosOffFlag.bits.bit7 = S_C_D.alarmFlag;//短路
	chgMosOffFlag.bits.bit8 = D_O_C2.alarmFlag;//放电过流2级
	chgMosOffFlag.bits.bit9 = U_V_1500.alarmFlag;//电压过低
	chgMosOffFlag.bits.bit10 = (syspara.voltMin<=1500)?1:0;//电压过低
	
	if(syspara.batStat == STATE_DSG)
	{
		if(DSG_time>3)
		{
			chgMosOffFlag.bytes = 0;
		}
		else
		{
			DSG_time++;
		}
	}
	else
	{
		DSG_time=0;
	}
	
	if(chgMosOffFlag.bytes)
	{
		CHG_OpenDelay= 0;
		syspara.IntegralPower_C=1000;//不能充电了把充电系数恢复
		Ctrl_CH_FET= 0; 	
	}
	else 
	{		
		if(CHG_OpenDelay<1)
		{
			CHG_OpenDelay++;
		}
		else
		{	//放电mos与充电mos是串联的，放电的时候要把充电mos打开，充电的时候也要把放电mos打开
			if(((syspara.voltMax<CFG_ST_OV)&&(syspara.tempMax<(Def_SH_OTC+TEMP_OFFSET))&&(syspara.tempMin>(Def_SH_UTC+TEMP_OFFSET)))||(syspara.batStat == STATE_DSG))	
			{ 
				Ctrl_CH_FET=1;
			}
		}
	}

#ifdef DBG_Printf_MOS
printf("CHG_Ctrl: %x  %d \r\n",chgMosOffFlag.bytes,Ctrl_CH_FET);
#endif
	if((syspara.dsgMos!=Ctrl_DS_FET)||(syspara.chgMos!=Ctrl_CH_FET))//如果充电mos或放电mos闭合
	{
		if((Ctrl_DS_FET==1)&&(syspara.dsgMos!=Ctrl_DS_FET))//放电mos闭合并且当前不是放电状态
		{
			if(DSG_OpenDelay<1)
			{
				DSG_OpenDelay++;
				return ;
			}
		}
		SH_MOSCtrl(Ctrl_CH_FET,Ctrl_DS_FET);
	}
	else
	{
		DSG_OpenDelay= 0;
	}
}





