/**
  ******************************************************************************
  * File Name          : TIM.c
  * Description        : This file provides code for the configuration
  *                      of the TIM instances.
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
#include "tim.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* TIM14 init function */
void MX_TIM14_Init(void)
{
  LL_TIM_InitTypeDef TIM_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM14);

  /* TIM14 interrupt Init */
  NVIC_SetPriority(TIM14_IRQn, 0);
  NVIC_EnableIRQ(TIM14_IRQn);

  TIM_InitStruct.Prescaler = 7;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 1000;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM14, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM14);

}
/* TIM16 init function */
void MX_TIM16_Init(void)
{
	LL_TIM_InitTypeDef TIM_InitStruct = {0};

	/* Peripheral clock enable */
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM16);

	/* TIM16 interrupt Init */
	NVIC_SetPriority(TIM16_IRQn, 0);
	NVIC_EnableIRQ(TIM16_IRQn);

	TIM_InitStruct.Prescaler = 15;
	TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
	TIM_InitStruct.Autoreload = 500;
	TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	TIM_InitStruct.RepetitionCounter = 0;
	LL_TIM_Init(TIM16, &TIM_InitStruct);
	//  LL_TIM_DisableARRPreload(TIM16);
	//GT_TIM16_CMD(ENABLE);
}

/* USER CODE BEGIN 1 */
void GT_TIM16_CMD(FunctionalState State)
{
	if(State==DISABLE)
	{
	 	LL_TIM_DisableCounter(TIM16);
	 	LL_TIM_DisableIT_UPDATE(TIM16);
   	}
   	else
   	{
	 	LL_TIM_EnableIT_UPDATE(TIM16);
	 	LL_TIM_EnableCounter(TIM16);
   	}
}
void GT_TIM_SET_COUNTER(uint32_t value)
{
	LL_TIM_SetCounter(TIM16, value);
}
void GT_TIM_SET_AUTORELOAD(uint32_t value)
{
	LL_TIM_SetAutoReload(TIM16, value);
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
