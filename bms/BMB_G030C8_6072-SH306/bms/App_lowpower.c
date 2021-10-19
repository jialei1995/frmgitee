#include "main.h"
S8_BITS deepSleepFlag;

void WakeUpInit(void)
{
	//NVIC_SystemReset();
}

void Enter_Stop_ENable(void)
{
	/* Stop mode with Main Regulator */
	MODIFY_REG(PWR->CR1, PWR_CR1_LPMS, LL_PWR_MODE_STOP1);
	/* Set SLEEPDEEP bit of Cortex System Control Register */
	SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));
	/* Request Wait For Interrupt */
	__WFI();
	/* Reset SLEEPDEEP bit of Cortex System Control Register */
	CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));
 }


void Enter_StopMode(void)
{

	//GPIO_InitTypeDef GPIO_InitStruct = {0};
	//HAL_NVIC_DisableIRQ(TIM14_IRQn);
	//GT_TIM16_CMD(DISABLE);

	SH_OpenAlarmInt();
	Sleep_PinInit();//��ʼ���Ǽ������Ի��Ѱ��ӵ�����Ϊ���Դ����жϵ�����
	LL_SYSTICK_DisableIT();

	//����֮ǰ����rtc���жϣ�main�����ʼ����rtc��û�п��жϡ�
	SetWakeUpTimer_IT( RTC_WAKE_SEC, LL_RTC_WAKEUPCLOCK_CKSPRE);

	Enter_Stop_ENable();//����ͣ�������ˣ����߲����ǳ������ˣ����ǳ���������stop�ˣ�һ�������ѻ�����һ������
	//NVIC_SystemReset();
	GPIO_CloseWEAK_EXTI();//��������Ѿ��������ˣ�Ӧ�ý�ֹ����Щ�жϣ���Щ�ж�ֻ�н�������֮ǰ�Żᱻ�õ�
	
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);//��������Ѱ����ϵ�����ʱ�Ӷ������ˡ�
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
	/* Configure the system clock */
	SystemClock_Config(1);
	
	/* USER CODE BEGIN SysInit */
	NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); /* set Priority for Systick Interrupt */
	LL_SYSTICK_EnableIT();
	bsp_init(1);
}



void DeepSleep(void)
{
	if(0 != Flash_WritePara())	//�������֮ǰ��Ҫдһ��flash
	{
		//printf("Flash_WritePara err!!\r\n");
	}
	//printf("*****GT_ForcePowerDown*****\r\n");
	SH_EnterLowPower();		//stm32�ĵ���sh367306����,���ִ����ͻ�
	//����,����sh367306�����ӵĵ��,�������Լ����ɼ�.ֻ���г���źŲ��ܻ��Ѱ���
}


//���߼��
void SleepCheck(void)
{
	static uint16_t timeout = 0;//
	static uint8_t SmallCurrTime= 0;
	S16_BITS sleepFlag;
	sleepFlag.bytes = 0;
#ifdef USESIF_FUNCTION
	if((abs(syspara.current))>CFG_SLP_C)
	{
		if(SmallCurrTime<10)
		{
			SmallCurrTime++;
		}
	}
	else
	{
		SmallCurrTime=0;
	}
	//һ��ͨ�Ƿ���connect״̬
	sleepFlag.bits.bit0 = (SIFBUSData.SIFConnectFlag==SIF_CONNECT)?1:0;
#endif
	//�ŵ�״̬���¶ȹ���
	sleepFlag.bits.bit1 = (((syspara.tempMin<=(Def_Sof_UTD+TEMP_OFFSET))&&(syspara.dsgMos==1))?1:0);
	//�¶ȹ���
	sleepFlag.bits.bit2 = ((syspara.tempMax>=(Def_SH_OTC+TEMP_OFFSET))?1:0);
	//���״̬������ѹ����
	sleepFlag.bits.bit3 = (((syspara.voltMax>3500)&&(syspara.chgMos==1))?1:0);
	//�����Ƚϴ�Ͳ����ߣ�����С�˲�����
	sleepFlag.bits.bit4 = ((SmallCurrTime>=1)?1:0);
	//
	sleepFlag.bits.bit5 = ((userAlarm.bytes&NotSleepAlarmMast)== 0)?0:1;
	//���״̬���¶ȹ���
	sleepFlag.bits.bit6 = (((syspara.tempMin<=(Def_Sof_UTC+TEMP_OFFSET))&&(syspara.chgMos==1))?1:0);
	//��ǰ�����������
	sleepFlag.bits.bit8 = (deepSleepFlag.bytes==0)?0:1;
	//�ŵ�״̬����С��ѹ����
	sleepFlag.bits.bit9 = (((syspara.voltMin<2330)&&(syspara.dsgMos==1))?1:0);
	//�˳���δ�õ����ֵ������=0
	sleepFlag.bits.bit10 = (sysflag.bits.VoltChargeBigFlag== 0)?0:1;
	//485������ʱ��
	sleepFlag.bits.bit11 = (RS485Buf.NodataTime<50)?1:0;
#ifdef DBG_Printf_SLEEP
	printf("SleepCheck : %d %x %x\r\n ",timeout,sleepFlag.bytes,sysflag.bytes);
#endif
	if(sleepFlag.bytes == 0)
	{
		timeout++;
		if((timeout>TimeOut(DEF_SLEEP_sec))||(sysflag.bits.SysWeakFlag==1))
		{
		//�ж����潫��Щ��־λ��1֮���û�ط��ģ�����������ߵ�ʱ����0�����ж���������
			sysflag.bits.CHGWeak= 0;
			sysflag.bits.DSGWeak= 0;
			sysflag.bits.SIFWeak= 0;	
			sysflag.bits.RTCWeak= 0;
			sysflag.bits.SysWeakFlag= 0;
	
			Enter_StopMode();

#ifdef DBG_Printf_SLEEP
			printf("weakFlag : %x \r\n ",sysflag.bytes);
#endif

//			sysflag.bits.SysWeakFlag=1;
			//������ܻ���Enter_StopMode�����ʱ���⼸����־λ��1
			if(sysflag.bits.RTCWeak!=0)
			{
#if(DEF_CAP==12)
			CutCapValue= 1250;
#elif(DEF_CAP==24)
			CutCapValue= 2500;
#elif(DEF_CAP==20)
			CutCapValue= 2100;
#elif(DEF_CAP==16)
			CutCapValue= 1650;
#elif(DEF_CAP==30)
			CutCapValue= 3200;
#elif(DEF_CAP==14)
			CutCapValue= 1450;
#else
			CutCapValue= (DEF_CAP*10+DEF_CAP/10*50);
#endif

#if(FACTURECODE==GTDS_FAC_BLX)
				timeout= TimeOut(DEF_SLEEP_sec)-TimeOut(5);
#elif (FACTURECODE==GTDS_FAC_ND)
				timeout= TimeOut(DEF_SLEEP_sec)-TimeOut(10);
#elif (FACTURECODE==GTDS_FAC_CF)
				timeout= TimeOut(DEF_SLEEP_sec)-TimeOut(3);
#elif (FACTURECODE==GTDS_FAC_PH)
				timeout= TimeOut(DEF_SLEEP_sec)-TimeOut(5);
#else
				timeout = 0;

#endif		
			}
			else
			{
				SIFBUSData.SIFConnectFlag= sysflag.bits.SIFWeak;
			}	
		}
	}
	else
	{
		timeout = 0;	
		sysflag.bits.SysWeakFlag= 0;
		if(deepSleepFlag.bytes==0)
		{
			sysflag.bits.RTCWeak= 0;
		}
	}
}
//������߼��
void DeepSleepCheck(void)
{
	
	static uint8_t timeout = 0;
//	static uint8_t boardErrCount= 0;
	deepSleepFlag.bytes = 0;
	// Ƿѹ�Żᴥ���������
	deepSleepFlag.bits.bit0 = (syspara.voltMin<CFG_DSLP_VMIN)?1:0;

#ifdef DBG_Printf_SLEEP
	printf("DeepSleepCheck : %x %d %d\r\n ",timeout,deepSleepFlag.bytes,sysflag.bits.RTCWeak);
#endif	
	if(deepSleepFlag.bytes!=0)
	{
		timeout++;
		if(timeout>TimeOut(20))//����20s������deepSleepFlag!=0
		{
			DeepSleep();
		}
	}
	else 
	{
		sysflag.bits.RTCWeak= 0; 
		timeout = 0;
	}

}
		




