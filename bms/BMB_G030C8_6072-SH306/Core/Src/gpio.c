/**
  ******************************************************************************
  * File Name          : gpio.c
  * Description        : This file provides code for the configuration
  *                      of all used GPIO pins.
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
#include "main.h"
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins
*/
void MX_GPIO_Init(void)
{
	
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* Peripheral clock enable */
	LL_GPIO_DeInit(GPIOA);
	LL_GPIO_DeInit(GPIOB);
	LL_GPIO_DeInit(GPIOC);
	LL_GPIO_DeInit(GPIOD);
	LL_RCC_LSE_Disable();
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);

	/**SPI1 GPIO Configuration
	PB6------> TVCC
	*/
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	TVCC_0;

	GPIO_InitStruct.Pin = LL_GPIO_PIN_15;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);



	
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Pin = LL_GPIO_PIN_15;
	GPIO_InitStruct.Pin |= LL_GPIO_PIN_0;
	GPIO_InitStruct.Pin |= LL_GPIO_PIN_1;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);


#ifdef USESIF_FUNCTION
	//Ò»ÏßÍ¨
  	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStruct.Pin = SIF_OUT_Pin;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = LED_Pin;
	LL_GPIO_Init(LED_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = LL_GPIO_PIN_14;	
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	HIGPOWER_0;
	LED_0;
#endif
}
void MX_EXTI_Init(void)
{


	LL_EXTI_InitTypeDef EXTI_InitStruct = {0};
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
	LL_RCC_LSE_Disable();
	/* GPIO Ports Clock Enable */
	
	/**/
	
	LL_EXTI_SetEXTISource(LL_EXTI_CONFIG_PORTA, LL_EXTI_CONFIG_LINE3);
	LL_EXTI_SetEXTISource(LL_EXTI_CONFIG_PORTA, LL_EXTI_CONFIG_LINE15);
	LL_EXTI_SetEXTISource(LL_EXTI_CONFIG_PORTA, LL_EXTI_CONFIG_LINE1);
	LL_EXTI_SetEXTISource(LL_EXTI_CONFIG_PORTA, LL_EXTI_CONFIG_LINE0);
	
	SIFBUS_H;

	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
	GPIO_InitStruct.Pin = SIF_In_Pin;
	LL_GPIO_Init(SIF_In_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LL_GPIO_PIN_15;
	GPIO_InitStruct.Pin |= LL_GPIO_PIN_0;
	GPIO_InitStruct.Pin |= LL_GPIO_PIN_1;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	/**/
	EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_0;
	EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
	EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
	EXTI_InitStruct.LineCommand = ENABLE;
	LL_EXTI_Init(&EXTI_InitStruct);
	
	EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_1;
	EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
	EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
	EXTI_InitStruct.LineCommand = ENABLE;
	LL_EXTI_Init(&EXTI_InitStruct);
	
	EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_15;
	EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
	EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
	EXTI_InitStruct.LineCommand = ENABLE;
	LL_EXTI_Init(&EXTI_InitStruct);
	
	EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_3;
	EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
	EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
	EXTI_InitStruct.LineCommand = ENABLE;
	LL_EXTI_Init(&EXTI_InitStruct);

	/**/
	LL_GPIO_SetPinPull(GPIOA, LL_EXTI_LINE_3, LL_GPIO_PULL_NO);
	LL_GPIO_SetPinPull(GPIOA, LL_EXTI_LINE_0, LL_GPIO_PULL_NO);
	LL_GPIO_SetPinPull(GPIOA, LL_EXTI_LINE_1, LL_GPIO_PULL_NO);
	LL_GPIO_SetPinPull(GPIOA, LL_EXTI_LINE_15, LL_GPIO_PULL_NO);
	
	/**/
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_3, LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_0, LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_1, LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_15, LL_GPIO_MODE_INPUT);


	/* EXTI interrupt init*/
	NVIC_SetPriority(EXTI0_1_IRQn, 0);
	NVIC_EnableIRQ(EXTI0_1_IRQn);
	NVIC_SetPriority(EXTI2_3_IRQn, 0);
	NVIC_EnableIRQ(EXTI2_3_IRQn);
	NVIC_SetPriority(EXTI4_15_IRQn, 0);
	NVIC_EnableIRQ(EXTI4_15_IRQn);


	}

/* USER CODE BEGIN 2 */

void Sleep_PinInit(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
	LL_TIM_DeInit(TIM16);
	LL_TIM_DisableCounter(TIM16);
	LL_USART_Disable(USART2);
#ifdef DBG_Printf_FUN
	LL_USART_Disable(USART1);
#endif
	TVCC_0;
	LED_1;
	GPIO_WritePin(GPIOA,LL_GPIO_PIN_14,(GPIO_PinState)1);
	LL_mDelay(1000);
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC);
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOD);
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pin = LL_GPIO_PIN_ALL;
	
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	LL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	LL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	MX_EXTI_Init();

}
void GPIO_OpenWEAK_EXTI(void )
{
	LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_0);
	LL_EXTI_DisableEvent_0_31(LL_EXTI_LINE_0);
	LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_1);
	LL_EXTI_DisableEvent_0_31(LL_EXTI_LINE_1);
	LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_15);
	LL_EXTI_DisableEvent_0_31(LL_EXTI_LINE_15);
	LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_3);
	LL_EXTI_DisableEvent_0_31(LL_EXTI_LINE_3);
	NVIC_EnableIRQ(EXTI4_15_IRQn);
	NVIC_EnableIRQ(EXTI2_3_IRQn);
	NVIC_EnableIRQ(EXTI0_1_IRQn);
}
void GPIO_CloseWEAK_EXTI(void )
{
	NVIC_DisableIRQ(EXTI0_1_IRQn);
	NVIC_DisableIRQ(EXTI2_3_IRQn);
	NVIC_DisableIRQ(EXTI4_15_IRQn);
	LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_0);
	LL_EXTI_DisableEvent_0_31(LL_EXTI_LINE_0);
	LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_1);
	LL_EXTI_DisableEvent_0_31(LL_EXTI_LINE_14);
	LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_15);
	LL_EXTI_DisableEvent_0_31(LL_EXTI_LINE_1);
	LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_3);
	LL_EXTI_DisableEvent_0_31(LL_EXTI_LINE_3);
}
 void GPIO_WritePin(GPIO_TypeDef *GPIOx, uint32_t PinMask,GPIO_PinState value)
 {
 	if(value==GPIO_PIN_RESET)
 	{
 		LL_GPIO_ResetOutputPin(GPIOx,PinMask);
 	}
 	else
 	{
		LL_GPIO_SetOutputPin(GPIOx,PinMask);
	}
 }

 uint32_t  GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint32_t PinMask)
 {
 	return LL_GPIO_IsInputPinSet(GPIOx,PinMask);
 }

/* USER CODE END 2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


