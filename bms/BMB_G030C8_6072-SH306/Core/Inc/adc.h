/**
  ******************************************************************************
  * File Name          : ADC.h
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __adc_H
#define __adc_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#define    DEF_CH_Vol		LL_ADC_CHANNEL_1
#define    DEF_CH_Curl 		LL_ADC_CHANNEL_0
#define    DEF_CH_T1		LL_ADC_CHANNEL_9
#define    DEF_CH_T2		LL_ADC_CHANNEL_15
#define    DEF_CH_Tmos	LL_ADC_CHANNEL_5
#define    DEF_CH_Vint		LL_ADC_CHANNEL_VREFINT
#define    DEF_CH_Init		LL_ADC_CHANNEL_11
/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
extern uint16_t  ADC_VREFINT_CAL;
extern uint16_t  ADC_VREFINT_DATA;
extern uint16_t  ADC_VDD_Vref;

/* USER CODE END Private defines */

void MX_ADC1_Init(void);

/* USER CODE BEGIN Prototypes */
void ADC_SelecteChannel(uint32_t ChannalNum);

uint16_t ADC_GetValue(void);

void ADC_Enable(void);
void ADC_Disable(void);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ adc_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
