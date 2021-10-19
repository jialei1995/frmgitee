#include "main.h"



uint32_t GetPage(uint32_t Addr)
{
  return (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;
}

/**
  * @brief  Unlock the FLASH control register access.
  * @retval HAL Status
  */
HAL_StatusTypeDef HAL_FLASH_Unlock(void)
{
  HAL_StatusTypeDef status = HAL_OK;

  if (READ_BIT(FLASH->CR, FLASH_CR_LOCK) != 0x00U)
  {
    /* Authorize the FLASH Registers access */
    WRITE_REG(FLASH->KEYR, FLASH_KEY1);
    WRITE_REG(FLASH->KEYR, FLASH_KEY2);

    /* verify Flash is unlock */
    if (READ_BIT(FLASH->CR, FLASH_CR_LOCK) != 0x00U)
    {
      status = HAL_ERROR;
    }
  }

  return status;
}

/**
  * @brief  Lock the FLASH control register access.
  * @retval HAL Status
  */
HAL_StatusTypeDef HAL_FLASH_Lock(void)
{
  HAL_StatusTypeDef status = HAL_ERROR;

  /* Set the LOCK Bit to lock the FLASH Registers access */
  SET_BIT(FLASH->CR, FLASH_CR_LOCK);

  /* verify Flash is locked */
  if (READ_BIT(FLASH->CR, FLASH_CR_LOCK) != 0x00u)
  {
    status = HAL_OK;
  }

  return status;
}


/**
  * @brief  Wait for a FLASH operation to complete.
  * @param  Timeout maximum flash operation timeout
  * @retval HAL_StatusTypeDef HAL Status
  */
HAL_StatusTypeDef FLASH_WaitForLastOperation(uint32_t Timeout)
{
  uint32_t error;
  uint32_t eccerr;
  uint32_t timeout = systickCount+ Timeout;
  /* Wait for the FLASH operation to complete by polling on BUSY flag to be reset.
     Even if the FLASH operation fails, the BUSY flag will be reset and an error
     flag will be set */

  /* Wait if any operation is ongoing */
  while (__HAL_FLASH_GET_FLAG(FLASH_SR_BSY1) != 0x00U)
  {
    if (systickCount>= timeout)
    {
      return HAL_TIMEOUT;
    }
  }

  /* check flash errors. Only ECC correction can be checked here as ECCD
      generates NMI */
  error = (FLASH->SR & FLASH_FLAG_SR_ERROR);

  /* Clear SR register */
  FLASH->SR = FLASH_FLAG_SR_CLEAR;

  /* Update error with ECC error value */
  eccerr = (FLASH->ECCR & FLASH_FLAG_ECCC);

  if(eccerr != 0x00u)
  {
    FLASH->ECCR |= eccerr;
    error |= eccerr;
  }

  if (error != 0x00U)
  {
    /*Save the error code*/
    return HAL_ERROR;
  }

  /* Wait for control register to be written */
  timeout =systickCount+ Timeout;

  while (__HAL_FLASH_GET_FLAG(FLASH_SR_BSY1) != 0x00U)
  {
    if (systickCount >= timeout)
    {
      return HAL_TIMEOUT;
    }
  }

  return HAL_OK;
}

/**
  * @brief  Program double word or fast program of a row at a specified address.
  * @param  TypeProgram Indicate the way to program at a specified address.
  *                      This parameter can be a value of @ref FLASH_Type_Program
  * @param  Address Specifies the address to be programmed.
  * @param  Data Specifies the data to be programmed
  *               This parameter is the data for the double word program and the address where
  *               are stored the data for the row fast program.
  *
  * @retval HAL_StatusTypeDef HAL Status
  */
HAL_StatusTypeDef HAL_FLASH_Program(uint32_t TypeProgram, uint32_t Address, uint64_t Data)
{
  HAL_StatusTypeDef status;




  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);

  if (status == HAL_OK)
  {
    if (TypeProgram == FLASH_CR_PG)
    {

      /* Program double-word (64-bit) at a specified address */
	  SET_BIT(FLASH->CR, FLASH_CR_PG);
	  
	  /* Program first word */
	  *(uint32_t *)Address = (uint32_t)Data;
	  
	  __ISB();
	  
	  /* Program second word */
	  *(uint32_t *)(Address + 4U) = (uint32_t)(Data >> 32U);
    }
    

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);

    /* If the program operation is completed, disable the PG or FSTPG Bit */
    CLEAR_BIT(FLASH->CR, TypeProgram);
  }

  /* return status */
  return status;
}

/**
  * @brief  Perform a mass erase or erase the specified FLASH memory pages.
  * @param[in]  pEraseInit Pointer to an @ref FLASH_EraseInitTypeDef structure that
  *         contains the configuration information for the erasing.
  * @param[out]  PageError Pointer to variable that contains the configuration
  *         information on faulty page in case of error (0xFFFFFFFF means that all
  *         the pages have been correctly erased)
  * @retval HAL Status
  */
HAL_StatusTypeDef HAL_FLASHEx_Erase(FLASH_EraseInitTypeDef *pEraseInit, uint32_t *PageError)
{
  HAL_StatusTypeDef status;
  uint32_t index;
  uint32_t tmp;

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);

  if (status == HAL_OK)
  {

      /*Initialization of PageError variable*/
      *PageError = 0xFFFFFFFFU;

      for (index = pEraseInit->Page; index < (pEraseInit->Page + pEraseInit->NbPages); index++)
      {
        /* Start erase page */
		/* Get configuration register, then clear page number */
		tmp = (FLASH->CR & ~FLASH_CR_PNB);
		
		/* Set page number, Page Erase bit & Start bit */
		FLASH->CR = (tmp | (FLASH_CR_STRT | (index <<  FLASH_CR_PNB_Pos) | FLASH_CR_PER));

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);

        if (status != HAL_OK)
        {
          /* In case of error, stop erase procedure and return the faulty address */
          *PageError = index;
          break;
        }
      }

      /* If operation is completed or interrupted, disable the Page Erase Bit */
      CLEAR_BIT(FLASH->CR, FLASH_CR_PER);
    }

  /* return status */
  return status;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
