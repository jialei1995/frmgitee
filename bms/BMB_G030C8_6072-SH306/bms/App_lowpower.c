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
	Sleep_PinInit();//初始化那几个可以唤醒板子的引脚为可以触发中断的引脚
	LL_SYSTICK_DisableIT();

	//休眠之前开启rtc的中断，main里面初始化的rtc并没有开中断。
	SetWakeUpTimer_IT( RTC_WAKE_SEC, LL_RTC_WAKEUPCLOCK_CKSPRE);

	Enter_Stop_ENable();//程序停到这里了，休眠并不是程序不跑了，而是程序在这里stop了，一旦被唤醒会往下一行运行
	//NVIC_SystemReset();
	GPIO_CloseWEAK_EXTI();//这里板子已经被唤醒了，应该禁止掉这些中断，这些中断只有进入休眠之前才会被用到
	
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);//休眠里面把板子上的外设时钟都给关了。
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
	if(0 != Flash_WritePara())	//深度休眠之前需要写一次flash
	{
		//printf("Flash_WritePara err!!\r\n");
	}
	//printf("*****GT_ForcePowerDown*****\r\n");
	SH_EnterLowPower();		//stm32的电是sh367306给的,这句执行完就会
	//掉电,但是sh367306是连接的电池,它还可以继续采集.只有有充电信号才能唤醒板子
}


//休眠检测
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
	//一线通是否处于connect状态
	sleepFlag.bits.bit0 = (SIFBUSData.SIFConnectFlag==SIF_CONNECT)?1:0;
#endif
	//放电状态下温度过低
	sleepFlag.bits.bit1 = (((syspara.tempMin<=(Def_Sof_UTD+TEMP_OFFSET))&&(syspara.dsgMos==1))?1:0);
	//温度过高
	sleepFlag.bits.bit2 = ((syspara.tempMax>=(Def_SH_OTC+TEMP_OFFSET))?1:0);
	//充电状态下最大电压过大
	sleepFlag.bits.bit3 = (((syspara.voltMax>3500)&&(syspara.chgMos==1))?1:0);
	//电流比较大就不休眠，电流小了才休眠
	sleepFlag.bits.bit4 = ((SmallCurrTime>=1)?1:0);
	//
	sleepFlag.bits.bit5 = ((userAlarm.bytes&NotSleepAlarmMast)== 0)?0:1;
	//充电状态下温度过低
	sleepFlag.bits.bit6 = (((syspara.tempMin<=(Def_Sof_UTC+TEMP_OFFSET))&&(syspara.chgMos==1))?1:0);
	//当前不是深度休眠
	sleepFlag.bits.bit8 = (deepSleepFlag.bytes==0)?0:1;
	//放电状态下最小电压过低
	sleepFlag.bits.bit9 = (((syspara.voltMin<2330)&&(syspara.dsgMos==1))?1:0);
	//此程序未用到这个值，这里=0
	sleepFlag.bits.bit10 = (sysflag.bits.VoltChargeBigFlag== 0)?0:1;
	//485无数据时间
	sleepFlag.bits.bit11 = (RS485Buf.NodataTime<50)?1:0;
#ifdef DBG_Printf_SLEEP
	printf("SleepCheck : %d %x %x\r\n ",timeout,sleepFlag.bytes,sysflag.bytes);
#endif
	if(sleepFlag.bytes == 0)
	{
		timeout++;
		if((timeout>TimeOut(DEF_SLEEP_sec))||(sysflag.bits.SysWeakFlag==1))
		{
		//中断里面将这些标志位置1之后就没地方改，这里进入休眠的时候置0，等中断里面拉起
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
			//程序可能会在Enter_StopMode里面的时候将这几个标志位置1
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
//深度休眠检测
void DeepSleepCheck(void)
{
	
	static uint8_t timeout = 0;
//	static uint8_t boardErrCount= 0;
	deepSleepFlag.bytes = 0;
	// 欠压才会触发深度休眠
	deepSleepFlag.bits.bit0 = (syspara.voltMin<CFG_DSLP_VMIN)?1:0;

#ifdef DBG_Printf_SLEEP
	printf("DeepSleepCheck : %x %d %d\r\n ",timeout,deepSleepFlag.bytes,sysflag.bits.RTCWeak);
#endif	
	if(deepSleepFlag.bytes!=0)
	{
		timeout++;
		if(timeout>TimeOut(20))//持续20s都存在deepSleepFlag!=0
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
		




