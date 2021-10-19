#include "main.h"

//实际电流
int32_t Update_Current(void)
{	  
	int32_t 	CurrentValue = 0;
	 SH_GetCUR(&CurrentValue);
	if((abs(CurrentValue))<=50)
	{
		CurrentValue= 0;
	}
	return 	(int32_t)(0-CurrentValue+flashpara.currentRate);
}

//电流标定
int32_t GetClibrationDiff_Current(void)
{
	int32_t 	CurrentValue = 0;
	SH_GetCUR(&CurrentValue);
	return CurrentValue;
}

//检测电池充放电状态
uint8_t  CurrentStateDetect( int32_t val )
{
	static uint16_t chgTime= 0;
	static uint16_t dsgTime= 0;
	static uint16_t SticTime= 0;
	static uint8_t sta = 0;
	if(val < (0-DEF_SMALL_NoShow)) 
	{
		chgTime = 0;
		dsgTime++;
		SticTime	=0;
		if(dsgTime > 1)
		{
			dsgTime = TimeOut(2)+2;//防止数字溢出
			syspara.IntegralPower_C= 1000;
			sta = STATE_DSG;
		}	
	}
	else if(val > DEF_SMALL_NoShow) 
	{
		chgTime++;
		dsgTime = 0;
		SticTime = 0;
		if(chgTime > 1)
		{
			chgTime = TimeOut(2)+2;//防止数字溢出
			syspara.IntegralPower = 1050;
			sta = STATE_CHG;			
		}	
	}
	else
	{
		chgTime = 0;
		dsgTime = 0;
		SticTime++;
		if((abs(syspara.current ))<DEF_SMALL_NoShow)
		{
			syspara.current = 0;//若是静置状态，当前电流赋值为0
		}
		if(SticTime > 1)
		{
			SticTime = TimeOut(2)+2;//防止数字溢出
			sta = STATE_STATIC;
		}
	}
	
	return sta;
}

//ZHQL_1013

void CurrAlarmProcess(void)
{
	uint8_t  AlarmCount= 0;
	//电流报警判断 此处电流报警不恢复，在报警文件里统一恢复
	if(syspara.batStat==STATE_CHG)//如果是充电状态
	{
		AlarmFilter(abs(syspara.current),CFG_ST_OCC,0,MORE,&C_O_C,DEF_OCC_TIME,100);
	}
	else
	{
		C_O_C.Times= 0;
	}
#if 1
	if(syspara.batStat==STATE_DSG)//如果是放电状态
	{
		AlarmFilter(abs(syspara.current),CFG_ST_ODC,0,MORE,&D_O_C,DEF_ODC_TIME,100);
	}
	else
	{
		D_O_C.Times= 0;
	}
	//放电过流2级
	while(((abs(syspara.current ))>=CFG_ST_ODC2)&&(D_O_C2.alarmFlag==0)&&(AlarmCount<20)&&(syspara.batStat==STATE_DSG))
	{
		//电流采集
		syspara.current = Update_Current();
		//电流状态判断
		syspara.batStat = CurrentStateDetect(syspara.current);	
		AlarmFilter(abs(syspara.current),CFG_ST_ODC2,0,MORE,&D_O_C2,DEF_ODC2_TIME,200);
		LL_mDelay(10);
		AlarmCount++;
	}
	D_O_C2.Times=0;
#endif
}
//放电电流报警恢复流程
void DCAlarm_RProcess()
{
	static uint32_t R_ODC_T= 0;

#ifdef DBG_Printf_ALARM
	printf("DCAlarm_RProcess: %x %d\r\n",sysflag.bits.LoadFlag,R_ODC_T);
#endif	
	//不处于充电状态且不处于放电状态
	if((syspara.dsgMos==0)&&(syspara.chgMos==0))
	{	
		//如果放电过流一级报警||放电过流二级报警||短路报警
		if((D_O_C.alarmFlag==1)||(D_O_C2.alarmFlag==1)||(S_C_D.alarmFlag==1))
		{
			SH_LOAD_CHECK(1);//开启负载检测功能
		}
		else
		{
			SH_LOAD_CHECK(0);//关闭负载检测功能
			R_ODC_T= 0;
		}
		if(sysflag.bits.LoadFlag==0)//负载未连接
		{
			if(R_ODC_T<2)
			{
				R_ODC_T++;
			}
			else
			{
				D_O_C.alarmFlag= 0;
				D_O_C.Times= 0;
				D_O_C2.alarmFlag= 0;
				D_O_C2.Times= 0;
				S_C_D.alarmFlag= 0;
				S_C_D.Times= 0;
				SH_ClrAlarm();//清除标志寄存器(Flag1)各保护标志位，sh367306只有放电短路保护，没有充电过流保护
				sysflag.bits.LoadFlag=1;//这个bit置0就是为了清报警标志的，置1就没啥用了，清完报警标志置1不让这个bit起作用
			}
		}	
		else
		{
			R_ODC_T= 0;
		}
	}
	else//当前处于充电或者放电状态
	{
		SH_LOAD_CHECK(0);//关闭负载检测功能
		R_ODC_T= 0;
	}
}

void CCAlarm_RProcess()
{
	static uint32_t R_OCC_T= 0;

#ifdef DBG_Printf_ALARM
	printf("CCAlarm_RProcess: %x %d\r\n",sysflag.bits.LoadFlag,R_OCC_T);
#endif	
	//当前不处于充电也不处于放电状态
	if((syspara.dsgMos==0)&&(syspara.chgMos==0))
	{
		if(C_O_C.alarmFlag==1)//充电过流报警
		{
			SH_CHGR_CHECK(1);//开启充电机检测
		}
		else
		{
			SH_CHGR_CHECK(0);//关闭充电机检测
		}
		if(sysflag.bits.DET_Flag==0)//充电机未连接
		{
			if(R_OCC_T<2)
			{
				R_OCC_T++;
			}
			else
			{
				C_O_C.alarmFlag= 0;
				C_O_C.Times= 0;
				CHG_OV_P_Count.alarmFlag=0;
				CHG_OV_P_Count.Times=0;
				sysflag.bits.DET_Flag= 1;//这个bit置0就是为了清报警标志位的，置1不让他起作用
				////清除标志寄存器(Flag1)各保护标志位，sh367306只有放电短路保护，没有充电过流保护
				//所以不会像上面的放电过流的清标志操作。
			}
		}
		else
		{
			R_OCC_T= 0;
		}
	}
	else
	{
		SH_CHGR_CHECK(0);//关闭充电机检测
		R_OCC_T= 0;
	}
}


