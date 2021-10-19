#include "APP_bms.h"

SysPara syspara;

SysFlag sysflag;

void Soft_Delay(uint16_t time)
{    
   uint16_t t=time*10;  
   while(t>0)
   {
   	t--;
   }
}

uint8_t CRC8(uint8_t *ptr,uint8_t len)
{
	uint8_t crc = 0;
	uint8_t i = 0;
	while(len--)
	{
		 crc ^= *ptr++;
		 for(i = 0;i < 8;i++)
		 {
				 if(crc & 0x80)
				 {
						 crc = (crc << 1) ^ 0x07;
				 }
				 else crc <<= 1;
		 }
	}
	return crc;
}

/******


100ms 周期任务

******/
void Process_100ms_CYC(void)
{
	static uint32_t currperiodtime= 0;

	//电流采集
	syspara.current = Update_Current();
	if(syspara.CurrCailFalg==0x55)
	{
		syspara.CurrCailFalg= 0;
		flashpara.currentRate=GetClibrationDiff_Current();
		Flash_WritePara();	
	}
	//电流状态判断
	syspara.batStat = CurrentStateDetect(syspara.current);	
	//电流报警判断
	CurrAlarmProcess();
	//中颖状态获取存到BSTATUS_Data与Flag1_Data中
	SH_GetBSTATUS();
	SH_GetSFlag();
	syspara.chgMos= BSTATUS_Data.Status_Bits.CHG;
	syspara.dsgMos= BSTATUS_Data.Status_Bits.DSG;
	S_C_D.alarmFlag |= Flag1_Data.Flag_Bits.SC_FLG;
	O_V.alarmFlag |= Flag1_Data.Flag_Bits.OV_FLG;
	sysflag.bits.DET_Flag = BSTATUS_Data.Status_Bits.CHGR;
	sysflag.bits.LoadFlag = BSTATUS_Data.Status_Bits.LOAD;

#ifdef DBG_Printf_CURR
	printf("Curr  %c = %d %d DC= %d SC= %d DC2= %d CC= %d\r\n",syspara.batStat,syspara.current,syspara.Period_Integral,D_O_C.alarmFlag,S_C_D.alarmFlag,D_O_C2.alarmFlag,C_O_C.alarmFlag);
#endif
	//MOS控制获取
	MOS_Ctrl();

	//放电电流报警恢复，充电电流报警恢复
	DCAlarm_RProcess();
	CCAlarm_RProcess();


	//根据电流状态选择充电或者放电积分系数
	if(syspara.batStat==STATE_DSG)
	{
		syspara.IntegralPowerjisuan=syspara.IntegralPower;
	}
	else if(syspara.batStat==STATE_CHG)
	{
		syspara.IntegralPowerjisuan=syspara.IntegralPower_C;
	}
	else
	{
		syspara.IntegralPowerjisuan=1050;
	}
	//SOC积分
	syspara.Period_Integral =  abs(systickCount- currperiodtime);//运行时间长度
	currperiodtime= systickCount;
	RemainCap_Integral(syspara.current,syspara.Period_Integral,syspara.IntegralPowerjisuan);	
	//循环次数，充放电次数  
	Cycle_Integral(syspara.current,syspara.Period_Integral);		
}



void Process_500ms_CYC1(void)
{
//	uint32_t testtime[8]={0}; 
	//电压
//	testtime[0]=systickCount;
	Update_CellVolt();
//	testtime[1]=systickCount;
	//均衡控制
	BalanceCtrl();
//	testtime[2]=systickCount;
	//温度
	Update_CellTemps();
//	testtime[3]=systickCount;


	//报警判断
	AlarmCheck();
//	testtime[4]=systickCount;


	//SOC  初始化电压对应一个初始SOC 值
	Soc_Init();
//	testtime[5]=systickCount;
	//SOC 校准系数计算
	RealRemainCap_Calibration();
//	testtime[6]=systickCount;
	
	//SOC 值
	syspara.SOC = Get_Percent(syspara.RemainCap,syspara.FullChargeCap);
	//SOH 值
	syspara.SOH = Get_Percent(syspara.FullChargeCap,DESIGN_CAP);
	//SOC 清零置满
	Soc_ClrOrSetCtrl();
	//深度休眠
	DeepSleepCheck();
	//休眠检测
	SleepCheck();
//	printf("Process_500ms_CYC1 %d %d %d %d %d %d %d \r\n",testtime[0],testtime[1],testtime[2],testtime[3],testtime[4],testtime[5],testtime[6]);
}



