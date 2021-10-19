#include "main.h"

ALARM    O_V;
ALARM    U_V_2700;
ALARM    U_V_2300;
ALARM    U_V_1500;
ALARM    O_T_M;  //mos
ALARM    O_T_C;  //���
ALARM    O_T_D;
ALARM    U_T_C;
ALARM    U_T_D;
ALARM    C_M_E;   //���mos���ϣ�err
ALARM    D_M_E;
ALARM    CHG_OV_P_Count;  //����ѹpro-����
ALARM    C_O_C;   //������
ALARM    D_O_C; 
ALARM    D_O_C2;  //�ŵ����2��
ALARM    S_C_D;   //��·

AlarmSt userAlarm;


void    AlarmFilter( int32_t Value,      //��ǰֵ������б���ֵ�Ƚϣ��Ƚϵ���Ŀ�ǵ������ǵ�ѹ��Buff���ƣ�more��ʾǰ��Ⱥ������С��
					 int32_t AlarmValue,
					 int32_t RecoverValue,  //��0��ʾ���ظ�������ֵ�Ż�ָ�
					 COMPARE  CompareType,
					 ALARM*  Buff,        //���յı���flag��д��buf����flag��
					 uint8_t  Timeout,   //����100��ǰ�涼�Ⱥ��������++��
					 uint8_t  ReTimeout  //����n�κ����ǰ�������++
					 )
{
    
	if( CompareType == LESS )
	{
		if( Value <= AlarmValue )
		{
			Buff->Times++;
			Buff->ReTimes = 0;
		}
		else if((Value > RecoverValue )&&(0 != RecoverValue))
		{
			Buff->ReTimes++;
			Buff->Times = 0;
		}
		else
		{
			Buff->Times = 0;
			Buff->ReTimes = 0;
		}
	}
	else if( CompareType == MORE )
	{
		if( Value >= AlarmValue )
		{
			Buff->Times++;
			Buff->ReTimes = 0;
		}
		else if((Value < RecoverValue )&&(0 != RecoverValue))
		{
			Buff->ReTimes++;
			Buff->Times = 0;
		}
		else//0==RecoverValue��û���ָ�
		{
			Buff->Times = 0;
			Buff->ReTimes = 0;
		}
	}
	if( Buff->Times >= Timeout )
	{
		Buff->alarmFlag = 1;   //�����ǰ���������涨�澯��ʱʱ��ͱ���
		Buff->Times = Timeout + 10;//ΪɶҪ��10
	}
	if( Buff->ReTimes > ReTimeout )
	{
		Buff->alarmFlag = 0; //����ָ�����ʱ�������涨�Ļָ�����
		Buff->ReTimes= ReTimeout + 10;
	}
}

void CHGDSG_MOS_CheckOK(void)
{
	
	//��ŵ�MOS�𻵼��
	if((syspara.chgMos == 0)&&(syspara.current>=CFG_ST_CME))
	{
		C_M_E.Times++;
		if(C_M_E.Times>TimeOut(2))
		{
			C_M_E.Times = TimeOut(2)+2;
			C_M_E.alarmFlag = 1;
		}
	}
	else 
	{
		C_M_E.Times = 0;
		C_M_E.alarmFlag = 0;
	}
	
	if((syspara.dsgMos == 0)&&(syspara.current<=(0-CFG_ST_DME)))
	{
		D_M_E.Times++;
		if(D_M_E.Times>TimeOut(2))
		{
			D_M_E.Times = TimeOut(2)+2;
			D_M_E.alarmFlag = 1;
		}
	}
	else 
	{
		D_M_E.Times = 0;
		D_M_E.alarmFlag = 0;
	}
}

void AlarmCheck(void)
{
	static uint8_t CHGProFlag= 0;
	static uint32_t SmallCurrTime= 0;
	static uint32_t OVcountRTime= 0;
	

	
	//��ѹǷѹ����
	
	AlarmFilter(syspara.voltMax, CFG_ST_OV, CFG_ST_OVR, MORE, &(O_V), TimeOut(CFG_OV_T),TimeOut(1));
	AlarmFilter(syspara.voltMin, CFG_ST_UV, CFG_S0f_UVR, LESS, &(U_V_2300), TimeOut(CFG_UV_T),TimeOut(1));
	AlarmFilter(syspara.voltMin, CFG_ST_UV3, CFG_S0f_UVR, LESS, &(U_V_1500), TimeOut(CFG_UV_T),TimeOut(1));
	//MOS ���±���
	AlarmFilter(syspara.mosTemp, CFG_ST_OTM, CFG_ST_OTMR, MORE, &(O_T_M), TimeOut(2),TimeOut(1));
	//������
	AlarmFilter(syspara.tempMin, (Def_Sof_UTC+TEMP_OFFSET), (Def_Sof_UTCR+TEMP_OFFSET), LESS, &(U_T_C), TimeOut(2),TimeOut(1));
	//������
	AlarmFilter(syspara.tempMax, (Def_SH_OTC+TEMP_OFFSET), (Def_SH_OTCR+TEMP_OFFSET), MORE, &(O_T_C), TimeOut(2),TimeOut(1));
	//�ŵ����
	AlarmFilter(syspara.tempMin, (Def_Sof_UTD+TEMP_OFFSET), (Def_Sof_UTDR+TEMP_OFFSET), LESS, &(U_T_D), TimeOut(2),TimeOut(1));
	//�ŵ����
	AlarmFilter(syspara.tempMax, (Def_SH_OTD+TEMP_OFFSET), (Def_SH_OTDR+TEMP_OFFSET), MORE, &(O_T_D), TimeOut(2),TimeOut(1));
	//����500mAʱ���ж�
	NowTime=GT_GET_RTC();
	
	if(SmallCurrTime== 0)  
	{
		SmallCurrTime= NowTime;
	}
	if(syspara.current< (-500))
	{
		SmallCurrTime= NowTime;
	}
	if(NowTime<SmallCurrTime)
	{
		NowTime += 24*3600;//��һ��
	}
#ifdef DBG_Printf_ALARM
		printf("UV2.7: %d %d %d %c\r\n",NowTime,SmallCurrTime,syspara.voltMin,syspara.batStat);
#endif
	if((syspara.voltMin<CFG_Sof_UV)
	&&((NowTime-SmallCurrTime)>CFG_2700_T)
	&&(syspara.batStat!=STATE_CHG))//�����ѹ<2700 && ���ǳ��״̬
	{
		U_V_2700.ReTimes= 0;
		if(U_V_2700.Times>10)
		{
			U_V_2700.alarmFlag=1;
			SmallCurrTime= NowTime;
		}
		else
		{
			U_V_2700.Times ++;
		}
	}
	else if(syspara.voltMin>CFG_Sof_UVR)
	{
		U_V_2700.Times= 0;
		if(U_V_2700.ReTimes<2)
		{
			U_V_2700.ReTimes++;
		}
		else
		{
			U_V_2700.alarmFlag=0;
		}
		
	}
	else
	{
		U_V_2700.Times= 0;
		U_V_2700.ReTimes= 0;
	}
	//��ŵ�mos���ϼ��
	CHGDSG_MOS_CheckOK();

	/*MOS ���ڵ����ѹ�߱��������ü�����־��	MOS �ָ���ָ�����+1 ��	����ָ���������Ԥ��ֵ����MOS���ٻָ���
	ֱ������������߷ŵ�*/
	NowTime=GT_GET_RTC();
	if(CHG_OV_P_Count.alarmFlag==0)
	{
		if((syspara.chgMos==0)&&(userAlarm.bits.O_V==1))
		{
			CHGProFlag=1;
			CHG_OV_P_Count.ReTimes= 0;
		}
		else if((userAlarm.bits.O_V==0)&&(syspara.chgMos==1))
		{
			CHG_OV_P_Count.ReTimes= 0;
			if(CHGProFlag==1)
			{
				CHG_OV_P_Count.Times++;
				CHGProFlag= 0;
			}
		}
		if(CHG_OV_P_Count.Times>=3)
		{
			CHG_OV_P_Count.Times= 5;
			CHG_OV_P_Count.alarmFlag=1;
		}
		OVcountRTime= NowTime;
	}
	else
	{
		if((NowTime<OVcountRTime)&&(NowTime < 24*3600))
		{
			NowTime+=24*3600;
		}
		if((NowTime-OVcountRTime)>1800)
		{
			CHG_OV_P_Count.Times= 0;
			CHG_OV_P_Count.alarmFlag=0;
		}
	}
	if(syspara.batStat==STATE_DSG)
	{
		CHG_OV_P_Count.Times= 0;
		CHG_OV_P_Count.alarmFlag=0;
	}
	if(O_V.alarmFlag==0)
	{
		if((Flag1_Data.Flag_Bits.OV_FLG==1)&&(Flag1_Data.Flag_Bits.SC_FLG==0))
		{
			SH_ClrAlarm();
		}
	}

	userAlarm.bits.O_V = O_V.alarmFlag;
	userAlarm.bits.U_V_2700 = U_V_2700.alarmFlag|U_V_1500.alarmFlag|U_V_2300.alarmFlag;
	userAlarm.bits.O_T_M = O_T_M.alarmFlag;
	userAlarm.bits.C_M_E = C_M_E.alarmFlag;
	userAlarm.bits.D_M_E = D_M_E.alarmFlag;
	userAlarm.bits.U_T_D = U_T_D.alarmFlag;
	userAlarm.bits.U_T_C = U_T_C.alarmFlag;
	userAlarm.bits.O_T_D = O_T_D.alarmFlag;
	userAlarm.bits.O_T_C = O_T_C.alarmFlag;
	userAlarm.bits.S_C_D = S_C_D.alarmFlag;
	userAlarm.bits.O_C_C = C_O_C.alarmFlag;
	userAlarm.bits.O_C_D = D_O_C.alarmFlag|D_O_C2.alarmFlag;
	userAlarm.bits.C_P_Count = CHG_OV_P_Count.alarmFlag;
#ifdef DBG_Printf_ALARM
	printf("Alarm: %x\r\n",userAlarm.bytes);
#endif
}




