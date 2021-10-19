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
	dsgMosOffFlag.bits.bit0 = userAlarm.bits.U_V_2700;//��о<2700
	dsgMosOffFlag.bits.bit1 = userAlarm.bits.U_T_D;//�ŵ����
	dsgMosOffFlag.bits.bit3 = userAlarm.bits.O_T_D;//�ŵ����
	dsgMosOffFlag.bits.bit4 = userAlarm.bits.O_T_M;//mos����
	dsgMosOffFlag.bits.bit5 = D_O_C.alarmFlag;//�ŵ����1��
	dsgMosOffFlag.bits.bit6 = D_O_C2.alarmFlag;//�ŵ������������
	dsgMosOffFlag.bits.bit7 = S_C_D.alarmFlag;//��·����
	dsgMosOffFlag.bits.bit8 = C_O_C.alarmFlag;//������
	
	if(syspara.batStat == STATE_CHG)//������
	{
		if(CHG_time>3)
		{
			dsgMosOffFlag.bytes = 0;//�ŵ�flag����=0
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
	if(dsgMosOffFlag.bytes) 	//�зŵ籨������
	{
		syspara.IntegralPower=1050;//���ܷŵ��˰ѷŵ�ϵ���ָ�
		Ctrl_DS_FET= 0;
	}
	else //û�зŵ籨�������Էŵ�
	{
		//��С��ѹ>2300 && ����¶�<105 && ��С�¶�>5 || ��ǰ���ڳ��״̬
		if(((syspara.voltMin>2300)&&(syspara.tempMax<(Def_SH_OTD+TEMP_OFFSET))&&(syspara.tempMin>(Def_SH_UTD+TEMP_OFFSET)))||(syspara.batStat == STATE_CHG))
		{
			Ctrl_DS_FET=1;//�����ŵ�mos
		}
		else
		{
			//�����в���
		}
	}
#ifdef DBG_Printf_MOS
	printf("DSG_Ctrl: %x %d\r\n",dsgMosOffFlag.bytes,Ctrl_DS_FET);
#endif	
	//CHG
	chgMosOffFlag.bytes = 0;
	chgMosOffFlag.bits.bit0 = userAlarm.bits.O_V;//��ѹ����
	chgMosOffFlag.bits.bit1 = userAlarm.bits.U_T_C;//����¶ȹ���
	chgMosOffFlag.bits.bit2 = userAlarm.bits.C_P_Count;//����ѹ����
	chgMosOffFlag.bits.bit3 = userAlarm.bits.O_T_C;//������
	chgMosOffFlag.bits.bit4 = userAlarm.bits.O_T_M;//mos����
	chgMosOffFlag.bits.bit5 = C_O_C.alarmFlag;//������
	chgMosOffFlag.bits.bit6 = D_O_C.alarmFlag;//�ŵ����1��
	chgMosOffFlag.bits.bit7 = S_C_D.alarmFlag;//��·
	chgMosOffFlag.bits.bit8 = D_O_C2.alarmFlag;//�ŵ����2��
	chgMosOffFlag.bits.bit9 = U_V_1500.alarmFlag;//��ѹ����
	chgMosOffFlag.bits.bit10 = (syspara.voltMin<=1500)?1:0;//��ѹ����
	
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
		syspara.IntegralPower_C=1000;//���ܳ���˰ѳ��ϵ���ָ�
		Ctrl_CH_FET= 0; 	
	}
	else 
	{		
		if(CHG_OpenDelay<1)
		{
			CHG_OpenDelay++;
		}
		else
		{	//�ŵ�mos����mos�Ǵ����ģ��ŵ��ʱ��Ҫ�ѳ��mos�򿪣�����ʱ��ҲҪ�ѷŵ�mos��
			if(((syspara.voltMax<CFG_ST_OV)&&(syspara.tempMax<(Def_SH_OTC+TEMP_OFFSET))&&(syspara.tempMin>(Def_SH_UTC+TEMP_OFFSET)))||(syspara.batStat == STATE_DSG))	
			{ 
				Ctrl_CH_FET=1;
			}
		}
	}

#ifdef DBG_Printf_MOS
printf("CHG_Ctrl: %x  %d \r\n",chgMosOffFlag.bytes,Ctrl_CH_FET);
#endif
	if((syspara.dsgMos!=Ctrl_DS_FET)||(syspara.chgMos!=Ctrl_CH_FET))//������mos��ŵ�mos�պ�
	{
		if((Ctrl_DS_FET==1)&&(syspara.dsgMos!=Ctrl_DS_FET))//�ŵ�mos�պϲ��ҵ�ǰ���Ƿŵ�״̬
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





