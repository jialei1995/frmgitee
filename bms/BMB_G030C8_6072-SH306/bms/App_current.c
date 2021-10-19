#include "main.h"

//ʵ�ʵ���
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

//�����궨
int32_t GetClibrationDiff_Current(void)
{
	int32_t 	CurrentValue = 0;
	SH_GetCUR(&CurrentValue);
	return CurrentValue;
}

//����س�ŵ�״̬
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
			dsgTime = TimeOut(2)+2;//��ֹ�������
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
			chgTime = TimeOut(2)+2;//��ֹ�������
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
			syspara.current = 0;//���Ǿ���״̬����ǰ������ֵΪ0
		}
		if(SticTime > 1)
		{
			SticTime = TimeOut(2)+2;//��ֹ�������
			sta = STATE_STATIC;
		}
	}
	
	return sta;
}

//ZHQL_1013

void CurrAlarmProcess(void)
{
	uint8_t  AlarmCount= 0;
	//���������ж� �˴������������ָ����ڱ����ļ���ͳһ�ָ�
	if(syspara.batStat==STATE_CHG)//����ǳ��״̬
	{
		AlarmFilter(abs(syspara.current),CFG_ST_OCC,0,MORE,&C_O_C,DEF_OCC_TIME,100);
	}
	else
	{
		C_O_C.Times= 0;
	}
#if 1
	if(syspara.batStat==STATE_DSG)//����Ƿŵ�״̬
	{
		AlarmFilter(abs(syspara.current),CFG_ST_ODC,0,MORE,&D_O_C,DEF_ODC_TIME,100);
	}
	else
	{
		D_O_C.Times= 0;
	}
	//�ŵ����2��
	while(((abs(syspara.current ))>=CFG_ST_ODC2)&&(D_O_C2.alarmFlag==0)&&(AlarmCount<20)&&(syspara.batStat==STATE_DSG))
	{
		//�����ɼ�
		syspara.current = Update_Current();
		//����״̬�ж�
		syspara.batStat = CurrentStateDetect(syspara.current);	
		AlarmFilter(abs(syspara.current),CFG_ST_ODC2,0,MORE,&D_O_C2,DEF_ODC2_TIME,200);
		LL_mDelay(10);
		AlarmCount++;
	}
	D_O_C2.Times=0;
#endif
}
//�ŵ���������ָ�����
void DCAlarm_RProcess()
{
	static uint32_t R_ODC_T= 0;

#ifdef DBG_Printf_ALARM
	printf("DCAlarm_RProcess: %x %d\r\n",sysflag.bits.LoadFlag,R_ODC_T);
#endif	
	//�����ڳ��״̬�Ҳ����ڷŵ�״̬
	if((syspara.dsgMos==0)&&(syspara.chgMos==0))
	{	
		//����ŵ����һ������||�ŵ������������||��·����
		if((D_O_C.alarmFlag==1)||(D_O_C2.alarmFlag==1)||(S_C_D.alarmFlag==1))
		{
			SH_LOAD_CHECK(1);//�������ؼ�⹦��
		}
		else
		{
			SH_LOAD_CHECK(0);//�رո��ؼ�⹦��
			R_ODC_T= 0;
		}
		if(sysflag.bits.LoadFlag==0)//����δ����
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
				SH_ClrAlarm();//�����־�Ĵ���(Flag1)��������־λ��sh367306ֻ�зŵ��·������û�г���������
				sysflag.bits.LoadFlag=1;//���bit��0����Ϊ���屨����־�ģ���1��ûɶ���ˣ����걨����־��1�������bit������
			}
		}	
		else
		{
			R_ODC_T= 0;
		}
	}
	else//��ǰ���ڳ����߷ŵ�״̬
	{
		SH_LOAD_CHECK(0);//�رո��ؼ�⹦��
		R_ODC_T= 0;
	}
}

void CCAlarm_RProcess()
{
	static uint32_t R_OCC_T= 0;

#ifdef DBG_Printf_ALARM
	printf("CCAlarm_RProcess: %x %d\r\n",sysflag.bits.LoadFlag,R_OCC_T);
#endif	
	//��ǰ�����ڳ��Ҳ�����ڷŵ�״̬
	if((syspara.dsgMos==0)&&(syspara.chgMos==0))
	{
		if(C_O_C.alarmFlag==1)//����������
		{
			SH_CHGR_CHECK(1);//�����������
		}
		else
		{
			SH_CHGR_CHECK(0);//�رճ������
		}
		if(sysflag.bits.DET_Flag==0)//����δ����
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
				sysflag.bits.DET_Flag= 1;//���bit��0����Ϊ���屨����־λ�ģ���1������������
				////�����־�Ĵ���(Flag1)��������־λ��sh367306ֻ�зŵ��·������û�г���������
				//���Բ���������ķŵ���������־������
			}
		}
		else
		{
			R_OCC_T= 0;
		}
	}
	else
	{
		SH_CHGR_CHECK(0);//�رճ������
		R_OCC_T= 0;
	}
}


