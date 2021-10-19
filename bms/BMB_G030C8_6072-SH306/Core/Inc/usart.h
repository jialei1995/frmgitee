/**
  ******************************************************************************
  * File Name          : USART.h
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
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
#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */





#define DBG_Printf_FUN		1
#ifdef 	DBG_Printf_FUN
//#define DBG_Printf_SLEEP	1
//#define DBG_Printf_SIF		1
//#define DBG_Printf_CURR	1
//#define DBG_Printf_LPS		1
//#define DBG_Printf_ALARM	1
//#define DBG_Printf_MOS	1
//#define DBG_Printf_Volt	1
//#define DBG_Printf_Temp	1
//#define DBG_Printf_SH30X	1

//#define DBG_Printf_SLEEP	1
//#define DBG_Printf_RTC		1
#define DBG_Printf_ProTIME		1
//#define DBG_Printf_CYC		1
//#define DBG_Printf_Balance		1
#define DBG_Printf_PCprotocol		1
#define DBG_Printf_Flash		1

#endif





/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */
#ifdef DBG_Printf_FUN
void MX_USART1_UART_Init(void);
#endif
void MX_USART2_UART_Init(void);

/* USER CODE BEGIN Prototypes */
void GT_UART2_CMD(FunctionalState State);
void RS485_SendByte(uint8_t *txdBuf,uint16_t len);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


