/**
  ******************************************************************************
  * File Name          : gpio.h
  * Description        : This file contains all the functions prototypes for
  *                      the gpio
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
#ifndef __gpio_H
#define __gpio_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

typedef enum
{
  GPIO_PIN_RESET = 0U,
  GPIO_PIN_SET
} GPIO_PinState;


#define LED_PORT	GPIOA
#define LED_Pin	LL_GPIO_PIN_13

/* USER CODE BEGIN Includes */
#define TVCC_1   LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_6)
#define TVCC_0   LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_6)

#define POWER_EN_1   LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_15)
#define POWER_EN_0   LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_15)


#define HIGPOWER_1   LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_14)
#define HIGPOWER_0   LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_14)


#define LED_1   LL_GPIO_SetOutputPin(LED_PORT, LED_Pin)
#define LED_0   LL_GPIO_ResetOutputPin(LED_PORT, LED_Pin)

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

/* USER CODE BEGIN Prototypes */

void MX_GPIO_Init(void);
void MX_EXTI_Init(void);
void Sleep_PinInit(void);
void GPIO_WritePin(GPIO_TypeDef *GPIOx, uint32_t PinMask,GPIO_PinState value);
uint32_t  GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint32_t PinMask);
void GPIO_OpenWEAK_EXTI(void );
void GPIO_CloseWEAK_EXTI(void );
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
