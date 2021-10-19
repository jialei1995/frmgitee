/**
  ******************************************************************************
  * File Name          : RTC.c
  * Description        : This file provides code for the configuration
  *                      of the RTC instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "rtc.h"
#define   DEF_RTCFISTUSEMASK  0x5a5a0000
 uint32_t NowTime= 0;

/* USER CODE BEGIN 0 */
/**
  * @brief  Convert a 2 digit decimal to BCD format.
  * @param  Value Byte to be converted
  * @retval Converted byte
  */
uint8_t RTC_ByteToBcd2(uint8_t Value)
{
  uint32_t bcdhigh = 0U;
  uint8_t Param = Value;

  while(Param >= 10U)
  {
    bcdhigh++;
    Param -= 10U;
  }

  return  ((uint8_t)(bcdhigh << 4U) | Param);
}

/**
  * @brief  Convert from 2 digit BCD to Binary.
  * @param  Value BCD value to be converted
  * @retval Converted word
  */
uint8_t RTC_Bcd2ToByte(uint8_t Value)
{
  uint32_t tmp;
  tmp = (((uint32_t)Value & 0xF0U) >> 4U) * 10U;
  return (uint8_t)(tmp + ((uint32_t)Value & 0x0FU));
}

/* USER CODE END 0 */

/* RTC init function */
void MX_RTC_Init(void)
{
	static uint8_t  initFalg= 0;
	LL_RTC_TimeTypeDef sTime;	//
	LL_RTC_DateTypeDef sDate;	//
	LL_RTC_InitTypeDef RTC_InitStruct = {0};
	SET_BIT(PWR->CR1, PWR_CR1_DBP);
	/* Peripheral clock enable */
	LL_RCC_EnableRTC();	//RCC里面reg，RTCEN使能
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_RTC);	//RTC APBEN使能
	/* RTC interrupt Init */
	NVIC_SetPriority(RTC_TAMP_IRQn, 2);
	NVIC_EnableIRQ(RTC_TAMP_IRQn);

	RTC_InitStruct.HourFormat = LL_RTC_HOURFORMAT_24HOUR;
	RTC_InitStruct.AsynchPrescaler = 127;
	RTC_InitStruct.SynchPrescaler = 255;
	LL_RTC_Init(RTC, &RTC_InitStruct);
	/** Enable the WakeUp
	*/
	// LL_RTC_WAKEUP_SetClock(RTC, LL_RTC_WAKEUPCLOCK_DIV_16);

	/* USER CODE BEGIN Check_RTC_BKUP */
	if(initFalg==1)
	{
		LL_RTC_DisableWriteProtection(RTC);
	    if (LL_RTC_IsShadowRegBypassEnabled(RTC) == 0U)
	    {
	        LL_RTC_WaitForSynchro(RTC);//
	    }
	    
		LL_RTC_EnableWriteProtection(RTC);
	return ;
	}
	//printf("MX_RTC_ RESET!!!\r\n");
	initFalg=1;

	LL_RTC_TIME_StructInit(&sTime);
	LL_RTC_DATE_StructInit(&sDate);

	/* USER CODE END Check_RTC_BKUP */

	/** Initialize RTC and set the Time and Date
	*/
	sTime.Hours = 14;
	sTime.Minutes = 50;
	sTime.Seconds = 0;
	sTime.TimeFormat= 0;
	if (LL_RTC_TIME_Init(RTC,LL_RTC_FORMAT_BIN, &sTime) != SUCCESS)
	{
	
	}
	sDate.WeekDay = 2;
	sDate.Month = LL_RTC_MONTH_JUNE;
	sDate.Day = 1;
	sDate.Year = 21;

	if (LL_RTC_DATE_Init(RTC,LL_RTC_FORMAT_BIN, &sDate) != SUCCESS)
	{
	}

}

/* USER CODE BEGIN 1 */


/* USER CODE BEGIN 1 */
uint32_t  GT_GET_RTC(void )
{
	RTC_DateTypeDef date;	//
	RTC_TimeTypeDef time;	//
	uint32_t tmpreg;
	uint32_t datetmpreg;



	/* Get subseconds structure field from the corresponding register*/
	time.SubSeconds = (uint32_t)(RTC->SSR);

	/* Get SecondFraction structure field from the corresponding register field*/
	time.SecondFraction = (uint32_t)(RTC->PRER & RTC_PRER_PREDIV_S);
	/* Get the TR register */
	tmpreg = (uint32_t)(RTC->TR & RTC_TR_RESERVED_MASK);
	/* Fill the structure fields with the read parameters */
	time.Hours = (uint8_t)((tmpreg & (RTC_TR_HT | RTC_TR_HU)) >> RTC_TR_HU_Pos);
	time.Minutes = (uint8_t)((tmpreg & (RTC_TR_MNT | RTC_TR_MNU)) >> RTC_TR_MNU_Pos);
	time.Seconds = (uint8_t)((tmpreg & (RTC_TR_ST | RTC_TR_SU)) >> RTC_TR_SU_Pos);
	time.TimeFormat = (uint8_t)((tmpreg & (RTC_TR_PM)) >> RTC_TR_PM_Pos);


	/* Convert the time structure parameters to Binary format */
	time.Hours = (uint8_t)RTC_Bcd2ToByte(time.Hours);
	time.Minutes = (uint8_t)RTC_Bcd2ToByte(time.Minutes);
	time.Seconds = (uint8_t)RTC_Bcd2ToByte(time.Seconds);

	/* Check the parameters */

	/* Get the DR register */
	datetmpreg = (uint32_t)(RTC->DR & RTC_DR_RESERVED_MASK);

	/* Fill the structure fields with the read parameters */
	date.Year = (uint8_t)((datetmpreg & (RTC_DR_YT | RTC_DR_YU)) >> RTC_DR_YU_Pos);
	date.Month = (uint8_t)((datetmpreg & (RTC_DR_MT | RTC_DR_MU)) >> RTC_DR_MU_Pos);
	date.Date = (uint8_t)((datetmpreg & (RTC_DR_DT | RTC_DR_DU)) >> RTC_DR_DU_Pos);
	date.WeekDay = (uint8_t)((datetmpreg & (RTC_DR_WDU)) >> RTC_DR_WDU_Pos);

	/* Check the input parameters format */

	/* Convert the date structure parameters to Binary format */
	date.Year = (uint8_t)RTC_Bcd2ToByte(date.Year);
	date.Month = (uint8_t)RTC_Bcd2ToByte(date.Month);
	date.Date = (uint8_t)RTC_Bcd2ToByte(date.Date);
#ifdef DBG_Printf_RTC
	printf("GT_GET_RTC %d:%d:%d\r\n",time.Hours,time.Minutes,time.Seconds);
	printf("GT_GET_Date %d:%d:%d\r\n",date.Year,date.Month,date.Date);
#endif
	return time.Hours*3600+time.Minutes*60+time.Seconds;
}

/**
  * @brief  Set wake up timer with interrupt.
  * @param  hrtc RTC handle
  * @param  WakeUpCounter Wake up counter
  * @param  WakeUpClock Wake up clock
  * @retval HAL status
  */
HAL_StatusTypeDef SetWakeUpTimer_IT( uint32_t WakeUpCounter, uint32_t WakeUpClock)
{
	uint32_t tickstart;


	/* Disable the write protection for RTC registers */
	LL_RTC_DisableWriteProtection(RTC);
	/* Disable the Wake-Up timer */
	LL_RTC_WAKEUP_Disable(RTC);
	/* Clear flag Wake-Up */
	LL_RTC_ClearFlag_WUT(RTC);

	/* Poll WUTWF until it is set in RTC_ICSR to make sure the access to wakeup autoreload
	counter and to WUCKSEL[2:0] bits is allowed. This step must be skipped in
	calendar initialization mode. */
	if (READ_BIT(RTC->ICSR, RTC_ICSR_INITF) == 0U)
	{
		tickstart=systickCount;
		while((__HAL_RTC_WAKEUPTIMER_GET_FLAG(RTC, RTC_FLAG_WUTWF) == 0U)&&((abs(systickCount-tickstart))<100))
		{

		}
	}

	/* Configure the Wakeup Timer counter */
	RTC->WUTR = (uint32_t)WakeUpCounter;

	/* Clear the Wakeup Timer clock source bits in CR register */
	RTC->CR &= (uint32_t)~RTC_CR_WUCKSEL;

	/* Configure the clock source */
	RTC->CR |= (uint32_t)WakeUpClock;

	/* RTC WakeUpTimer Interrupt Configuration: EXTI configuration */
	LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_19);
	/* Configure the Interrupt in the RTC_CR register */
	LL_RTC_EnableIT_WUT(RTC);

	/* Enable the Wakeup Timer */
	LL_RTC_WAKEUP_Enable(RTC);

	/* Enable the write protection for RTC registers */
	LL_RTC_EnableWriteProtection(RTC);


	return HAL_OK;
}


/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
