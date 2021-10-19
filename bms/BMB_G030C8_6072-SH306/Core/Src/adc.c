/**
  ******************************************************************************
  * File Name          : ADC.c
  * Description        : This file provides code for the configuration
  *                      of the ADC instances.
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
#include "adc.h"
typedef uint64_t  U64;
typedef uint32_t  U32;
typedef uint16_t U16;
typedef uint8_t  U8;

/* USER CODE BEGIN 0 */
uint16_t  ADC_VREFINT_CAL= 0;
uint16_t  ADC_VREFINT_DATA= 0;
uint16_t  ADC_VDD_Vref= 0;

/* USER CODE END 0 */

/*********************************************************************
*函数名:VolFilter
*描      述:数据过滤
*参      数：U16* pVolBuf - 数据
             U8 BufLenth - 数据长度
                         U8 MaxCount - 高
                         U8 MinCount - 低
*返      回:无
**********************************************************************/
uint16_t VolFilter( U16* pVolBuf, U8 BufLenth, U8 MaxCount, U8 MinCount )
{
	U8 i = 0, j = 0;
	U16 tShort = 0;
	U32 LongD= 0;
	if(BufLenth<=0)
	{
		return 0;
	}
	//find the max，将MaxCout个最大放在最后
	for( i = 0; i < MaxCount; i++ )
	{
		for( j = 0; j < BufLenth - 1 - i; j++ )
			if( pVolBuf[j] > pVolBuf[j + 1] )
			{
				//交换记录
				tShort = pVolBuf[j + 1]; //R[0]不是哨兵，仅做暂存单元
				pVolBuf[j + 1] = pVolBuf[j];
				pVolBuf[j] = tShort;
			}
	} //endfor(外循环)

	//find the min，将MinCount个最小放在最后
	for( i = 0; i < MinCount; i++ )
	{
		for( j = 0; j < BufLenth - 3 - i; j++ )
			if( pVolBuf[j] < pVolBuf[j + 1] )
			{
				//交换记录
				tShort = pVolBuf[j + 1]; //R[0]不是哨兵，仅做暂存单元
				pVolBuf[j + 1] = pVolBuf[j];
				pVolBuf[j] = tShort;
			}
	} //endfor(外循环)
	LongD= 0;
	for(i= 0;i<(BufLenth-(MaxCount+MinCount));i++)
	{
		LongD+= pVolBuf[i];
	}
	if(i>0)
	{
		return (uint16_t)(LongD/i);
	}
	else
	{
		return pVolBuf[0];
	}
}

/* ADC1 init function */

void ADC_Calibration(void)
{
	uint32_t 	Timeout= 0;
	if((ADC1->CR&ADC_CR_ADSTART) == (ADC_CR_ADSTART))
	{
		LL_ADC_REG_StopConversion(ADC1);
		
	}
	Timeout= 0;
	while((ADC1->CR&ADC_CR_ADSTP) == (ADC_CR_ADSTP))
	{
		if(Timeout <0x3FFFF)
		{
			Timeout++;
		}
		else
		{
		//printf("ADC_StopConvn TimeOut \r\n");
			break ;
		}
	}
	if((ADC1->CR&ADC_CR_ADEN) == (ADC_CR_ADEN))
	{
		LL_ADC_Disable(ADC1 );
		
	}
	Timeout= 0;
	while((ADC1->CR&ADC_CR_ADDIS) == (ADC_CR_ADDIS))
	{
		if(Timeout <0x3FFFF)
		{
			Timeout++;
		}
		else
		{
			//printf("ADC_Disable TimeOut \r\n");
			break ;
		}
	}
	//printf("ADC-CR  %x \r\n",ADC1->CR);
	//printf("ADC-ISR  %x \r\n",ADC1->ISR);
	//printf("ADC-IER  %x \r\n",ADC1->IER);
	//printf("ADC-Cfg1  %x \r\n",ADC1->CFGR1);
	LL_ADC_StartCalibration(ADC1);
	Timeout= 0;
	while((ADC1->CR&ADC_CR_ADCAL) == (ADC_CR_ADCAL))
	{
		if(Timeout <0x3FFFF)
		{
			Timeout++;
		}
		else
		{
			//printf("ADC_Calibration TimeOut \r\n");
			break ;
		}
	}
	
	ADC_VREFINT_DATA= LL_ADC_GetCalibrationFactor(ADC1);
}

void MX_ADC1_Init(void)
{
//	uint32_t 	Timeout= 0;
	LL_ADC_InitTypeDef ADC_InitStruct = {0};
	LL_ADC_REG_InitTypeDef ADC_REG_InitStruct = {0};

	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
	LL_ADC_DeInit(ADC1);
	//printf("ADC-CR  %x \r\n",ADC1->CR);
	//printf("ADC-ISR  %x \r\n",ADC1->ISR);
	//printf("ADC-IER  %x \r\n",ADC1->IER);
	//printf("ADC-Cfg1  %x \r\n",ADC1->CFGR1);
	/* Peripheral clock enable */
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC);

	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
	/**ADC1 GPIO Configuration
	PA5   ------> ADC1_IN5	Tmos
	*/
	GPIO_InitStruct.Pin = LL_GPIO_PIN_5;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = LL_GPIO_PIN_1;

	/** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
	*/
	LL_ADC_REG_SetSequencerConfigurable(ADC1, 0);//设置为 CHSELRMOD  0
  	ADC_InitStruct.Clock = LL_ADC_CLOCK_SYNC_PCLK_DIV2;
	ADC_InitStruct.Resolution = LL_ADC_RESOLUTION_12B;
	ADC_InitStruct.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
	ADC_InitStruct.LowPowerMode = LL_ADC_LP_MODE_NONE;
	LL_ADC_Init(ADC1, &ADC_InitStruct);
	ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
	ADC_REG_InitStruct.SequencerLength = LL_ADC_REG_SEQ_SCAN_DISABLE;
	ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
	ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_SINGLE;
	ADC_REG_InitStruct.DMATransfer = LL_ADC_REG_DMA_TRANSFER_NONE;
	ADC_REG_InitStruct.Overrun = LL_ADC_REG_OVR_DATA_PRESERVED;
	LL_ADC_REG_Init(ADC1, &ADC_REG_InitStruct);
	LL_ADC_SetOverSamplingScope(ADC1, LL_ADC_OVS_DISABLE);
	LL_ADC_SetTriggerFrequencyMode(ADC1, LL_ADC_CLOCK_FREQ_MODE_HIGH);
	LL_ADC_SetSamplingTimeCommonChannels(ADC1, LL_ADC_SAMPLINGTIME_COMMON_1, LL_ADC_SAMPLINGTIME_160CYCLES_5);
	LL_ADC_SetSamplingTimeCommonChannels(ADC1, LL_ADC_SAMPLINGTIME_COMMON_2, LL_ADC_SAMPLINGTIME_160CYCLES_5);

	/* Enable ADC internal voltage regulator */

	//软件校准ADC
	/*
	1.Ensure that ADEN = 0, ADVREGEN = 1 and DMAEN = 0.
	2. Set ADCAL = 1.
	3. Wait until ADCAL = 0 (or until EOCAL = 1). This can be handled by interrupt if the interrupt is enabled by setting the EOCALIE bit in the ADC_IER register
	4. The calibration factor can be read from bits 6:0 of ADC_DR or ADC_CALFACT registers.
	*/
	LL_ADC_EnableInternalRegulator(ADC1);
	
	//printf("ADC-CR  %x \r\n",ADC1->CR);
	//printf("ADC-ISR  %x \r\n",ADC1->ISR);
	//printf("ADC-IER  %x \r\n",ADC1->IER);
	//printf("ADC-Cfg1  %x \r\n",ADC1->CFGR1);

	/* Delay for ADC internal voltage regulator stabilization. */
	ADC_VREFINT_CAL= *(uint16_t*)0x1FFF75AA;
	//ADC_VREFINT_DATA= LL_ADC_GetCalibrationFactor(ADC1);
	//printf("ADC_VREFINT_CAL=%d\r\n",ADC_VREFINT_CAL);
	
	ADC_Calibration();
	/* Compute number of CPU cycles to wait for, from delay in us. */
	/* Note: Variable divided by 2 to compensate partially */
	/* CPU processing cycles (depends on compilation optimization). */
	/* Note: If system core clock frequency is below 200kHz, wait time */
	/* is only a few CPU processing cycles. */
	uint32_t wait_loop_index;
	wait_loop_index = ((LL_ADC_DELAY_INTERNAL_REGUL_STAB_US * (SystemCoreClock / (100000 * 2))) / 10);
	while(wait_loop_index != 0)
	{
		wait_loop_index--;
	}
	//使能内部基准电压
	LL_ADC_SetCommonClock(ADC1_COMMON,LL_ADC_CLOCK_ASYNC_DIV12);
	LL_ADC_SetCommonPathInternalCh(ADC1_COMMON,LL_ADC_PATH_INTERNAL_VREFINT);
/* Configure Regular Channel
	*/
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, DEF_CH_T1);
  LL_ADC_SetChannelSamplingTime(ADC1, DEF_CH_T1, LL_ADC_SAMPLINGTIME_COMMON_1);
}
/* USER CODE BEGIN 1 */
void ADC_SelecteChannel(uint32_t ChannalNum)
{
	uint32_t 	Timeout= 0;
	ADC1->CHSELR= ChannalNum;
	while((ADC1->ISR & LL_ADC_FLAG_CCRDY) == 0UL)
	{
		if(Timeout <0xFFFF)
		{
			Timeout++;
		}
		else
		{
			//printf("ADC_SelecteChannel TimeOut \r\n");
			return ;
		}
	}
}
void ADC_Enable(void)
{
	uint32_t 	Timeout= 0;
	LL_ADC_ClearFlag_ADRDY(ADC1);
	LL_ADC_Enable(ADC1 );
	while((ADC1->ISR & LL_ADC_FLAG_ADRDY) == 0UL)
	{
		if(Timeout <0xFFFF)
		{
			Timeout++;
		}
		else
		{
			//printf("ADC_Start TimeOut \r\n");
			return ;
		}
	}
}
void ADC_Disable(void)
{
	uint32_t Timeout= 0;
	LL_ADC_Disable(ADC1 );
	while((ADC1->CR&ADC_CR_ADDIS) == (ADC_CR_ADDIS))
	{
		if(Timeout <0x3FFFF)
		{
			Timeout++;
		}
		else
		{
			//printf("ADC_Disable TimeOut \r\n");
			break ;
		}
	}
}
void ADC_WaitSampleOver(void)
{
	uint32_t 	Timeout= 0;
	/* Wait until End of unitary conversion or sequence conversions flag is raised */
	while((ADC1->ISR & LL_ADC_FLAG_EOC) == 0UL)
	{
		/* Check if timeout is disabled (set to infinite wait) */
		if(Timeout <0xFFFF)
		{
			Timeout++;
		}
		else
		{
			//printf("ADC_CONVERTING TimeOut \r\n");
			return ;
		}
	}
}
uint16_t ADC_GetValue(void)
{
	uint8_t j= 0;
	uint16_t tULong[32]={0};
	uint16_t 	Value= 0;	
		//采集32次
		for(j= 0;j<32;j++)
		{
			//清除标志
			LL_ADC_ClearFlag_EOC(ADC1);
			//开启ADC 采样
			LL_ADC_REG_StartConversion(ADC1);
			//等待采样结束
			ADC_WaitSampleOver();
			tULong[j]=LL_ADC_REG_ReadConversionData12(ADC1);
		}
		Value=VolFilter(tULong,32,8,8);
		return (uint16_t)(Value&0xffff);

}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
