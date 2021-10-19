/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
static const uint8_t VERINFO[8] __attribute__((at(0x80018f8)))={((uint8_t)(PROJECTCODE&0xff)^'G'),(PROJECTCODE>>8)^'T',((uint8_t)(FACTURECODE&0xff)^'D'),HARDCODE_M^'S',HARDCODE_S^'L',RWSERVIN^'O',FACTURECODE^'N','G'};


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DEF_500_CYC 5

__IO uint32_t   systickCount= 0;

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

void Process_500ms_CYC2(void)
{
	//�ڵ�400 ����رվ��⣬����500����ɵ�ѹ�¶���׼��
	SH_BALANCESET(0);
}

void bsp_init(uint8_t MOD)
{
	MX_GPIO_Init();
	MX_USART1_UART_Init();
	MX_USART2_UART_Init();
	MX_ADC1_Init();
	MX_TIM16_Init();
	//if(MOD==0)
	//{
	MX_RTC_Init();
	//}
	//MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
 	//LPS5238SPI_Init(MOD);
 	SH30x_init(SIMIIC_1);
 	SH30x_init(SIMIIC_2);
 	SH30x_init(SIMIIC_3);
 }
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	/* USER CODE BEGIN 1 */
	uint32_t  Cyc100mstime= 0;
	uint32_t  Cyc100msCount= 0;
	uint32_t  CyctickmsCount= 0;
	uint8_t Flag= 0;
	//uint16_t  testBuff[50]={0};
	/* USER CODE END 1 */
	__enable_irq();
	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */

	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

	/* System interrupt init*/

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config(0);

	/* USER CODE BEGIN SysInit */
	NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); /* set Priority for Systick Interrupt */
	LL_SYSTICK_EnableIT();

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	bsp_init(0);
	FlashInit();//�ȳ�ʼ��Flash
	SysParaInit();
	CycleCount_Init();//�ٳ�ʼ��������ѭ���������жϳ�������

	/* USER CODE END 2 */
	//printf("Start Run\r\n");
	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		CyctickmsCount= systickCount;
		
		if(RS485Buf.rcvFlag==1) //485/uart�������ݣ�rcvflag����λ
		{
#if(DBG_Printf_PCprotocol==1)
			printf("RS485Buf--%d  %d %d \r\n",RS485Buf.rcvFlag,RS485Buf.headPosition,RS485Buf.tailPosition);
#endif
			RS485Buf.rcvFlag = 0;//�ж�����1��������0��Ȼ��������
			RS485_Process();	
		}

		SIFProcess(CyctickmsCount);

		if(((abs(CyctickmsCount-Cyc100mstime))>CFG_CURR_COUNT))
		{
			if(RS485Buf.NodataTime<100)
			{
				RS485Buf.NodataTime++;
			}
#ifdef DBG_Printf_ProTIME
			printf("***systickCount***%d***\r\n",systickCount);
#endif
			Cyc100mstime= CyctickmsCount;
			Process_100ms_CYC();
			Cyc100msCount++;//100ms��ѭ��ִ��һ��,���ֵ++,�ӵ�5�ͻ����500msѭ��,�ӵ�4�ͻ����500msѭ��2
			if((Cyc100msCount%DEF_500_CYC==0))//
			{
				GPIO_WritePin(LED_PORT,LED_Pin,(GPIO_PinState)Flag);
				Flag=!Flag;
				Process_500ms_CYC1();
				Cyc100msCount=0;//
			}
			if(Cyc100msCount%DEF_500_CYC==4)
			{
				Process_500ms_CYC2();
			}
			if(sysflag.bits.flashSave_Flag==1)  //ֻ�б���ѭ������485ͨѶ����һ��ͨͨ�Ų���
			{   
				sysflag.bits.flashSave_Flag= 0;
				Flash_WritePara();
			}
		}
		/* USER CODE END WHILE */
		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(uint8_t mod)
{
  /* HSI configuration and activation */
  LL_RCC_HSI_Enable();
  while(LL_RCC_HSI_IsReady() != 1)
  {
  }

  /* LSI configuration and activation */
  LL_RCC_LSI_Enable();
  while(LL_RCC_LSI_IsReady() != 1)
  {
  }

  LL_PWR_EnableBkUpAccess();
  if(mod==0)
  {
	 LL_RCC_ForceBackupDomainReset();
	 LL_RCC_ReleaseBackupDomainReset();
  }
  /* Set AHB prescaler*/
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

  /* Sysclk activation on the HSI */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {
  }

  /* Set APB1 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);

  LL_Init1msTick(16000000);//�൱��������systickԤ��Ƶϵ����16m��1\16m=1s��

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(16000000);
  LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSI);
  LL_RCC_EnableRTC();
  LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK1);
  LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSOURCE_SYSCLK);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: //printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
