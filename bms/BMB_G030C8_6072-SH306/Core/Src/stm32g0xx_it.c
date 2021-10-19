/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32g0xx_it.c
  * @brief   Interrupt Service Routines.
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32g0xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M0+ Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */
	NVIC_SystemReset();
  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVC_IRQn 0 */

  /* USER CODE END SVC_IRQn 0 */
  /* USER CODE BEGIN SVC_IRQn 1 */

  /* USER CODE END SVC_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */
  	systickCount++;

  /* USER CODE END SysTick_IRQn 0 */

  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32G0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32g0xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles RTC and TAMP interrupts through EXTI lines 19 and 21.
  */
void RTC_TAMP_IRQHandler(void)
{
  /* USER CODE BEGIN RTC_TAMP_IRQn 0 */
	if(LL_RTC_IsActiveFlag_ALRA(RTC) != RESET)
	{
		LL_RTC_ClearFlag_ALRA(RTC);
	}
	if(LL_RTC_IsActiveFlag_WUT(RTC) != RESET)
	{
		LL_RTC_ClearFlag_WUT(RTC);
		sysflag.bits.RTCWeak=1;
	}
  /* USER CODE END RTC_TAMP_IRQn 0 */

  /* USER CODE BEGIN RTC_TAMP_IRQn 1 */

  /* USER CODE END RTC_TAMP_IRQn 1 */
}

/**
  * @brief This function handles EXTI line 0 and line 1 interrupts.
  */
void EXTI4_15_IRQHandler(void)
{
	if (LL_EXTI_IsActiveRisingFlag_0_31(LL_EXTI_LINE_15) != RESET)
	{
		LL_EXTI_ClearRisingFlag_0_31(LL_EXTI_LINE_15);
		sysflag.bits.DSGWeak=1;
	}
	if (LL_EXTI_IsActiveFallingFlag_0_31(LL_EXTI_LINE_15) != RESET)
	{
		LL_EXTI_ClearFallingFlag_0_31(LL_EXTI_LINE_15);
		sysflag.bits.DSGWeak=1;
	}
}

/**
  * @brief This function handles EXTI line 0 and line 1 interrupts.
  */
void EXTI2_3_IRQHandler(void)
{
	if (LL_EXTI_IsActiveRisingFlag_0_31(LL_EXTI_LINE_3) != RESET)
	{
		LL_EXTI_ClearRisingFlag_0_31(LL_EXTI_LINE_3);
		sysflag.bits.SIFWeak=1;
	}
	if (LL_EXTI_IsActiveFallingFlag_0_31(LL_EXTI_LINE_3) != RESET)
	{
		LL_EXTI_ClearFallingFlag_0_31(LL_EXTI_LINE_3);
		sysflag.bits.SIFWeak=1;
	}
  /* USER CODE END EXTI0_1_IRQn 1 */
}
/**
  * @brief This function handles EXTI line 0 and line 1 interrupts.
  */
void EXTI0_1_IRQHandler(void)
{
	if (LL_EXTI_IsActiveRisingFlag_0_31(LL_EXTI_LINE_0) != RESET)
	{
		LL_EXTI_ClearRisingFlag_0_31(LL_EXTI_LINE_0);
		sysflag.bits.SIFWeak=1;
	}
	if (LL_EXTI_IsActiveFallingFlag_0_31(LL_EXTI_LINE_0) != RESET)
	{
		LL_EXTI_ClearFallingFlag_0_31(LL_EXTI_LINE_0);
		sysflag.bits.SIFWeak=1;
	}
	if (LL_EXTI_IsActiveRisingFlag_0_31(LL_EXTI_LINE_1) != RESET)
	{
		LL_EXTI_ClearRisingFlag_0_31(LL_EXTI_LINE_1);
		sysflag.bits.CHGWeak=1;
	}
	if (LL_EXTI_IsActiveFallingFlag_0_31(LL_EXTI_LINE_1) != RESET)
	{
		LL_EXTI_ClearFallingFlag_0_31(LL_EXTI_LINE_1);
		sysflag.bits.CHGWeak=1;
	}
  /* USER CODE END EXTI0_1_IRQn 1 */
}

/**
  * @brief This function handles TIM14 global interrupt.
  */
void TIM14_IRQHandler(void)
{
  /* USER CODE BEGIN TIM14_IRQn 0 */
  LL_TIM_ClearFlag_UPDATE(TIM16);
  /* USER CODE END TIM14_IRQn 0 */
  /* USER CODE BEGIN TIM14_IRQn 1 */

  /* USER CODE END TIM14_IRQn 1 */
}

/**
  * @brief This function handles TIM16 global interrupt.
  */
void TIM16_IRQHandler(void)
{
  /* USER CODE BEGIN TIM16_IRQn 0 */
	LL_TIM_ClearFlag_UPDATE(TIM16);
  	SIF_TIM_IRQPro();
//	SIF_TIM_IRQPro();
  //	SIF_TIM_IRQPro();
  /* USER CODE END TIM16_IRQn 0 */
  /* USER CODE BEGIN TIM16_IRQn 1 */

  /* USER CODE END TIM16_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt / USART1 wake-up interrupt through EXTI line 25.
  */
void USART1_IRQHandler(void)
{
	/* USER CODE BEGIN USART1_IRQn 0 */
	uint32_t ISR_reg= 0;
	ISR_reg= LL_USART_ReadReg(USART1,ISR);//读串口中断状态reg

	/* USER CODE END USART1_IRQn 0 */
	/* USER CODE BEGIN USART1_IRQn 1 */
	if(LL_USART_IsActiveFlag_RXNE(USART1) != 0U) //uart的rxne中断触发，无需清标志位，读一下就自动清了
	{
		//LL_USART_ReceiveData8(USART1);
		RingBuf_Write(LL_USART_ReceiveData8(USART1));
	}
	if(LL_USART_IsActiveFlag_IDLE(USART1) != 0U)//uart的idel中断发生，rxne一byte一byte发数据存到485buf中后发完了就会触发idel中断
	{								//所以在idel里面置485recvflag为1，idel的flag需要自己清除
		LL_USART_ClearFlag_IDLE(USART1);
		RS485Buf.rcvFlag=1;
		//  UART1RxDataPro(0xFFFF);
	}
	if(LL_USART_IsActiveFlag_ORE(USART1) != 0U)//上次放的数据还没被取走新数据就被新数据冲掉了，会触发oreflag
	{									//以下的处理方式就是发生这种err的处理吗？
		LL_USART_ReceiveData8(USART1);
		LL_USART_ClearFlag_IDLE(USART1);
		LL_USART_ClearFlag_ORE(USART1);
	}
	LL_USART_WriteReg(USART1,ICR,ISR_reg);//将LL_USART_ReadReg读到的isr变量中的值写道icr寄存器，有啥就清啥
	/* USER CODE END USART1_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt / USART2 wake-up interrupt through EXTI line 26.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */
  uint32_t ISR_reg= 0;

  /* USER CODE END USART2_IRQn 0 */
  /* USER CODE BEGIN USART2_IRQn 1 */
	ISR_reg= LL_USART_ReadReg(USART2,ISR);
  /* USER CODE BEGIN USART2_IRQn 0 */
	if(LL_USART_IsActiveFlag_RXNE(USART2) != 0U)
	{
		UART2RxDataPro(LL_USART_ReceiveData8(USART2));//uart收到1byte就会到这里，将收到的数据存起来
	}
	if(LL_USART_IsActiveFlag_IDLE(USART2) != 0U)
	{
		LL_USART_ClearFlag_IDLE(USART2);//所有的uart收完没数据了才会到这里。
		UART2RxDataPro(0xFFFF);
	}
	if(LL_USART_IsActiveFlag_ORE(USART2) != 0U)
	{
			LL_USART_ReceiveData8(USART2);
			LL_USART_ClearFlag_IDLE(USART2);
			LL_USART_ClearFlag_ORE(USART2);
	}
	LL_USART_WriteReg(USART2,ICR,ISR_reg);

  /* USER CODE END USART2_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
