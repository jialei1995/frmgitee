#ifndef __BOOT_FLASH_H_
#define __BOOT_FLASH_H_
 #include "main.h"
 
#define FLASH_SIZE                  0x10000       //FLASH总大小64K
#define FLASH_PAGE_SIZE             0x800         //数据存储空间分配大小2K
#define FLASH_START_ADDR            0x8000000 //FLASH的起始地址

typedef struct
{
  uint32_t TypeErase;   /*!< Mass erase or page erase.
                             This parameter can be a value of @ref FLASH_Type_Erase */
  uint32_t Page;        /*!< Initial Flash page to erase when page erase is enabled
                             This parameter must be a value between 0 and (FLASH_PAGE_NB - 1) */
  uint32_t NbPages;     /*!< Number of pages to be erased.
                             This parameter must be a value between 1 and (FLASH_PAGE_NB - value of initial page)*/
} FLASH_EraseInitTypeDef;

typedef enum
{
  HAL_OK       = 0x00U,
  HAL_ERROR    = 0x01U,
  HAL_BUSY     = 0x02U,
  HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;

#define FLASH_FLAG_EOP                  FLASH_SR_EOP      /*!< FLASH End of operation flag */
#define FLASH_FLAG_OPERR                FLASH_SR_OPERR    /*!< FLASH Operation error flag */
#define FLASH_FLAG_PROGERR              FLASH_SR_PROGERR  /*!< FLASH Programming error flag */
#define FLASH_FLAG_WRPERR               FLASH_SR_WRPERR   /*!< FLASH Write protection error flag */
#define FLASH_FLAG_PGAERR               FLASH_SR_PGAERR   /*!< FLASH Programming alignment error flag */
#define FLASH_FLAG_SIZERR               FLASH_SR_SIZERR   /*!< FLASH Size error flag  */
#define FLASH_FLAG_PGSERR               FLASH_SR_PGSERR   /*!< FLASH Programming sequence error flag */
#define FLASH_FLAG_MISERR               FLASH_SR_MISERR   /*!< FLASH Fast programming data miss error flag */
#define FLASH_FLAG_FASTERR              FLASH_SR_FASTERR  /*!< FLASH Fast programming error flag */
#define FLASH_FLAG_OPTVERR              FLASH_SR_OPTVERR  /*!< FLASH Option validity error flag */
#define FLASH_FLAG_BSY                  FLASH_SR_BSY1     /*!< FLASH Operation Busy flag */
#define FLASH_FLAG_CFGBSY               FLASH_SR_CFGBSY   /*!< FLASH Configuration Busy flag */
#define FLASH_FLAG_ECCC                 FLASH_ECCR_ECCC   /*!< FLASH ECC correction */
#define FLASH_FLAG_ECCD                 FLASH_ECCR_ECCD   /*!< FLASH ECC detection */

#define FLASH_FLAG_SR_ERROR             (FLASH_FLAG_OPERR  | FLASH_FLAG_PROGERR | FLASH_FLAG_WRPERR |  \
                                         FLASH_FLAG_PGAERR | FLASH_FLAG_SIZERR  | FLASH_FLAG_PGSERR |   \
                                         FLASH_FLAG_MISERR | FLASH_FLAG_FASTERR |   \
                                         FLASH_FLAG_OPTVERR) 

                                         
#define FLASH_FLAG_SR_CLEAR             (FLASH_FLAG_SR_ERROR | FLASH_SR_EOP)

#define __HAL_FLASH_GET_FLAG(__FLAG__)          ((((__FLAG__) & (FLASH_ECCR_ECCC | FLASH_ECCR_ECCD)) != 0U) ? \
                                                 (READ_BIT(FLASH->ECCR, (__FLAG__)) == (__FLAG__))  : \
                                                 (READ_BIT(FLASH->SR,   (__FLAG__)) == (__FLAG__)))

#define FIRMWARE_MAXSIZE   (FLASH_PAGE_SIZE*12-0xF8)	//固件允许最大空间
#define ADDR_BKP  		(ADDR_APP) //程序备份地址

#define FLASH_KEY1                      0x45670123U   /*!< Flash key1 */
#define FLASH_KEY2                      0xCDEF89ABU   /*!< Flash key2: used with FLASH_KEY1
                                                           to unlock the FLASH registers access */
#define FLASH_TYPEERASE_PAGES           FLASH_CR_PER    /*!< Pages erase only */
#define FLASH_TYPEERASE_MASS            FLASH_CR_MER1   /*!< Flash mass erase activation */

#define FLASH_TIMEOUT_VALUE 1000

uint32_t GetPage(uint32_t Addr);

int BootFlash_Write(uint32_t Addr,uint8_t* Buf,uint32_t num);
HAL_StatusTypeDef HAL_FLASHEx_Erase(FLASH_EraseInitTypeDef *pEraseInit, uint32_t *PageError);
HAL_StatusTypeDef HAL_FLASH_Program(uint32_t TypeProgram, uint32_t Address, uint64_t Data);
HAL_StatusTypeDef HAL_FLASH_Lock(void);
HAL_StatusTypeDef HAL_FLASH_Unlock(void);
#endif


