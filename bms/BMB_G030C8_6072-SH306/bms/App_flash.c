#include "main.h"
#define FLASH_USER_PARA_ADDR   (FLASH_USER_END_ADDR-FLASH_PAGE_SIZE)

FlashPara flashpara;
#define FistUseFlag   0x1619 //年+20        月十六进制     日日BCD码


uint16_t GetCheck(void)
{
	uint16_t i = 0;
	uint32_t val = 0;
	val += flashpara.appmask;
	val += flashpara.yuliu;

	for(i=0;i<16;i++)
	{
		val += flashpara.id[i];
	}
	val += flashpara.chgMode;
	val += flashpara.workMode;
	val += flashpara.CycleCount;
	val += flashpara.currentRate;
	val += flashpara.FistUseData;
	val += flashpara.cyc_CHG;
	val += flashpara.shortCnt;
	for(i=0;i<32;i++)
	{
		val += flashpara.VoltRate[i];
	}
	val += flashpara.BKP3_Data;
	val += flashpara.P_UV_Count;
	return (uint16_t)(val&0xFFff);  //返回的是uint16只取16bit，所以要&一下
}

void Flash_ReadPara()
{
	int i = 0;
	uint32_t Address = FLASH_USER_PARA_ADDR;
//	S_SDWORD sdword;
	S_DWORD dword;
	S_HALFWORD halfword;
	S_SHALFWORD shalfword;
	S_WORD sword;
	dword.dword = *(__IO uint64_t *)Address;

	sword.bytes.byte_1 = dword.bytes.byte_1;	
	sword.bytes.byte_2 = dword.bytes.byte_2;	
	sword.bytes.byte_3 = dword.bytes.byte_3;	
	sword.bytes.byte_4 = dword.bytes.byte_4;	
	flashpara.appmask= sword.word;
	
	sword.bytes.byte_1 = dword.bytes.byte_5;	
	sword.bytes.byte_2 = dword.bytes.byte_6;	
	sword.bytes.byte_3 = dword.bytes.byte_7;	
	sword.bytes.byte_4 = dword.bytes.byte_8;	
	flashpara.yuliu= sword.word;

	Address += 8;
	for(i=0;i<2;i++)
	{
		Address += i*8;
		dword.dword = *(__IO uint64_t *)Address;
		flashpara.id[i*8+0] = dword.bytes.byte_1;	
		flashpara.id[i*8+1] = dword.bytes.byte_2;
		flashpara.id[i*8+2] = dword.bytes.byte_3;
		flashpara.id[i*8+3] = dword.bytes.byte_4;
		flashpara.id[i*8+4] = dword.bytes.byte_5;
		flashpara.id[i*8+5] = dword.bytes.byte_6;
		flashpara.id[i*8+6] = dword.bytes.byte_7;
		flashpara.id[i*8+7] = dword.bytes.byte_8;
	}
	
	Address += 8;
	dword.dword = *(__IO uint64_t *)Address;
	flashpara.chgMode = dword.bytes.byte_1;	
	flashpara.workMode = dword.bytes.byte_2;
	halfword.bytes.byte_h = dword.bytes.byte_3;	
	halfword.bytes.byte_l = dword.bytes.byte_4;
	flashpara.CycleCount = halfword.halfword;
	shalfword.bytes.byte_h = dword.bytes.byte_5;	
	shalfword.bytes.byte_l = dword.bytes.byte_6;
	flashpara.currentRate = shalfword.shalfword;
	shalfword.bytes.byte_h = dword.bytes.byte_7;	
	shalfword.bytes.byte_l = dword.bytes.byte_8;
	flashpara.FistUseData = shalfword.shalfword;
	Address += 8;
	dword.dword = *(__IO uint64_t *)Address;

	sword.bytes.byte_1 = dword.bytes.byte_1;	
	sword.bytes.byte_2 = dword.bytes.byte_2;	
	sword.bytes.byte_3 = dword.bytes.byte_3;	
	sword.bytes.byte_4 = dword.bytes.byte_4;	
	flashpara.cyc_CHG= sword.word;
	
	sword.bytes.byte_1 = dword.bytes.byte_5;	
	sword.bytes.byte_2 = dword.bytes.byte_6;	
	sword.bytes.byte_3 = dword.bytes.byte_7;	
	sword.bytes.byte_4 = dword.bytes.byte_8;	
	flashpara.shortCnt= sword.word;
	Address += 8;
	for(i=0;i<4;i++)
	{
		Address += i*8;
		dword.dword = *(__IO uint64_t *)Address;
		flashpara.VoltRate[i*8+0] = dword.bytes.byte_1;	
		flashpara.VoltRate[i*8+1] = dword.bytes.byte_2;
		flashpara.VoltRate[i*8+2] = dword.bytes.byte_3;
		flashpara.VoltRate[i*8+3] = dword.bytes.byte_4;
		flashpara.VoltRate[i*8+4] = dword.bytes.byte_5;
		flashpara.VoltRate[i*8+5] = dword.bytes.byte_6;
		flashpara.VoltRate[i*8+6] = dword.bytes.byte_7;
		flashpara.VoltRate[i*8+7] = dword.bytes.byte_8;
	}
	Address += 8;
	dword.dword = *(__IO uint64_t *)Address;

	sword.bytes.byte_1 = dword.bytes.byte_1;	
	sword.bytes.byte_2 = dword.bytes.byte_2;	
	sword.bytes.byte_3 = dword.bytes.byte_3;	
	sword.bytes.byte_4 = dword.bytes.byte_4;	
	flashpara.BKP3_Data= sword.word;
	shalfword.bytes.byte_h = dword.bytes.byte_5;	
	shalfword.bytes.byte_l = dword.bytes.byte_6;
	flashpara.P_UV_Count= shalfword.shalfword;
	shalfword.bytes.byte_h = dword.bytes.byte_7;	
	shalfword.bytes.byte_l = dword.bytes.byte_8;
	flashpara.checkh= shalfword.shalfword;
	
}

uint8_t Flash_WritePara(void)
{
	int i = 0;
	uint32_t PageError = 0;
	uint32_t Address = FLASH_USER_PARA_ADDR;

	S_SDWORD sdword;
	S_HALFWORD halfword;
	S_SHALFWORD shalfword;
	S_WORD Sword;
	FLASH_EraseInitTypeDef EraseInitStruct;
	flashpara.checkh = GetCheck();
	//return 0;
	__disable_irq(); 
	HAL_FLASH_Unlock();
	EraseInitStruct.TypeErase  = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.Page        = GetPage(FLASH_USER_PARA_ADDR);
	EraseInitStruct.NbPages     = 1;
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
	{
		HAL_FLASH_Lock();
		__enable_irq(); 
		return 1;
	}
  	
	Sword.word= flashpara.appmask;
	sdword.bytes.byte_1 = Sword.bytes.byte_1;
	sdword.bytes.byte_2 = Sword.bytes.byte_2;
	sdword.bytes.byte_3 = Sword.bytes.byte_3;
	sdword.bytes.byte_4 = Sword.bytes.byte_4;
	Sword.word= flashpara.yuliu;
	sdword.bytes.byte_5 = Sword.bytes.byte_1;
	sdword.bytes.byte_6 = Sword.bytes.byte_2;
	sdword.bytes.byte_7 = Sword.bytes.byte_3;
	sdword.bytes.byte_8 = Sword.bytes.byte_4;
	if (HAL_FLASH_Program(FLASH_CR_PG, Address, sdword.sdword) == HAL_OK)
	{
		if (*(__IO uint64_t *)Address != sdword.sdword)
		{
			HAL_FLASH_Lock();
			__enable_irq(); 
			return 1;
		}
		
	}
	else
	{
		HAL_FLASH_Lock();
		__enable_irq();
		return 1;
	}
	
	Address+=8;
	for(i=0;i<2;i++)
	{
		sdword.bytes.byte_1 = flashpara.id[8*i+0];
		sdword.bytes.byte_2 = flashpara.id[8*i+1];
		sdword.bytes.byte_3 = flashpara.id[8*i+2];
		sdword.bytes.byte_4 = flashpara.id[8*i+3];
		sdword.bytes.byte_5 = flashpara.id[8*i+4];
		sdword.bytes.byte_6 = flashpara.id[8*i+5];
		sdword.bytes.byte_7 = flashpara.id[8*i+6];
		sdword.bytes.byte_8 = flashpara.id[8*i+7];
		Address += 8*i;
		if (HAL_FLASH_Program(FLASH_CR_PG, Address, sdword.sdword) == HAL_OK)
		{
			if (*(__IO uint64_t *)Address != sdword.sdword)
			{
				HAL_FLASH_Lock();
				__enable_irq(); 
				return 1;
			}
			
		}
		else
		{
			HAL_FLASH_Lock();
			__enable_irq();
			return 1;
		}
	}
	Address+=8;
	sdword.bytes.byte_1 = flashpara.chgMode;
	sdword.bytes.byte_2 = flashpara.workMode;
	halfword.halfword = flashpara.CycleCount;
	sdword.bytes.byte_3 = halfword.bytes.byte_h;
	sdword.bytes.byte_4 = halfword.bytes.byte_l;
	shalfword.shalfword = flashpara.currentRate;
	sdword.bytes.byte_5 = shalfword.bytes.byte_h;
	sdword.bytes.byte_6 = shalfword.bytes.byte_l;
	halfword.halfword = flashpara.FistUseData;
	sdword.bytes.byte_7 = halfword.bytes.byte_h;
	sdword.bytes.byte_8 = halfword.bytes.byte_l;

	if (HAL_FLASH_Program(FLASH_CR_PG, Address, sdword.sdword) == HAL_OK)
	{
		if (*(__IO uint64_t *)Address != sdword.sdword)
		{
			HAL_FLASH_Lock();
			__enable_irq(); 
			return 1;
		}
		
	}
	else
	{
		HAL_FLASH_Lock();
		__enable_irq();
		return 1;
	}

	Address+=8;
	Sword.word= flashpara.cyc_CHG;
	sdword.bytes.byte_1 = Sword.bytes.byte_1;
	sdword.bytes.byte_2 = Sword.bytes.byte_2;
	sdword.bytes.byte_3 = Sword.bytes.byte_3;
	sdword.bytes.byte_4 = Sword.bytes.byte_4;
	Sword.word= flashpara.shortCnt;
	sdword.bytes.byte_5 = Sword.bytes.byte_1;
	sdword.bytes.byte_6 = Sword.bytes.byte_2;
	sdword.bytes.byte_7 = Sword.bytes.byte_3;
	sdword.bytes.byte_8 = Sword.bytes.byte_4;

	if (HAL_FLASH_Program(FLASH_CR_PG, Address, sdword.sdword) == HAL_OK)
	{
		if (*(__IO uint64_t *)Address != sdword.sdword)
		{
			HAL_FLASH_Lock();
			__enable_irq(); 
			return 1;
		}
		
	}
	else
	{
		HAL_FLASH_Lock();
		__enable_irq();
		return 1;
	}

	Address+=8;
	for(i=0;i<4;i++)
	{
		sdword.bytes.byte_1 = flashpara.VoltRate[8*i+0];
		sdword.bytes.byte_2 = flashpara.VoltRate[8*i+1];
		sdword.bytes.byte_3 = flashpara.VoltRate[8*i+2];
		sdword.bytes.byte_4 = flashpara.VoltRate[8*i+3];
		sdword.bytes.byte_5 = flashpara.VoltRate[8*i+4];
		sdword.bytes.byte_6 = flashpara.VoltRate[8*i+5];
		sdword.bytes.byte_7 = flashpara.VoltRate[8*i+6];
		sdword.bytes.byte_8 = flashpara.VoltRate[8*i+7];
		Address += 8*i;
		if (HAL_FLASH_Program(FLASH_CR_PG, Address, sdword.sdword) == HAL_OK)
		{
			if (*(__IO uint64_t *)Address != sdword.sdword)
			{
				HAL_FLASH_Lock();
				__enable_irq(); 
				return 1;
			}
			
		}
		else
		{
			HAL_FLASH_Lock();
			__enable_irq();
			return 1;
		}
	}

	Address+=8;
	Sword.word= flashpara.BKP3_Data;
	sdword.bytes.byte_1 = Sword.bytes.byte_1;
	sdword.bytes.byte_2 = Sword.bytes.byte_2;
	sdword.bytes.byte_3 = Sword.bytes.byte_3;
	sdword.bytes.byte_4 = Sword.bytes.byte_4;
	halfword.halfword = flashpara.P_UV_Count;
	sdword.bytes.byte_5 = halfword.bytes.byte_h;
	sdword.bytes.byte_6 = halfword.bytes.byte_l;
	halfword.halfword = flashpara.checkh;
	sdword.bytes.byte_7 = halfword.bytes.byte_h;
	sdword.bytes.byte_8 = halfword.bytes.byte_l;
	if (HAL_FLASH_Program(FLASH_CR_PG, Address, sdword.sdword) == HAL_OK)
	{
		if (*(__IO uint64_t *)Address != sdword.sdword)
		{
			HAL_FLASH_Lock();
			__enable_irq(); 
			return 1;
		}
		
	}
	else
	{
		HAL_FLASH_Lock();
		__enable_irq();
		return 1;
	}

	HAL_FLASH_Lock();
	__enable_irq();
	Flash_ReadPara();
	
	
	return 0;

}

void FlashParaSetDefault(void)
{
	int i = 0;
	char idchar[16];
	uint8_t IDErrFlag=0;
	memset((void*)idchar,0,16);
	//flash里面刚开始不存数据的时候就都是0xff，一旦发现都是ff说明这个flash之前没用过，现在就初始化它
	for(i=0;i<16;i++)
	{
		if((flashpara.id[i]==0xFF )||(IDErrFlag== 1))
		{
			IDErrFlag= 1;
			flashpara.id[i] = idchar[i];
		}
	}
	for(i=0;i<32;i++)
	{
		flashpara.VoltRate[i] = 0;
	}
	flashpara.appmask= DEF_APP_MASK;
	flashpara.yuliu= 0;
	flashpara.chgMode = UNCOM;
	flashpara.workMode = EXITWAKECMD;
	flashpara.CycleCount = 0;
	flashpara.cyc_CHG= 0;
	flashpara.shortCnt= 0;
	flashpara.BKP3_Data= 0;
	flashpara.P_UV_Count= 0;
	flashpara.FistUseData=FistUseFlag;
}

uint8_t CheckFlash(void)
{
	uint16_t cks;
	Flash_ReadPara();
	
	cks = GetCheck();
	if((flashpara.FistUseData!=FistUseFlag)||(cks != flashpara.checkh))
	{

		return 1;
	}
 	else if(DEF_APP_MASK!= flashpara.appmask)
	{
		flashpara.appmask= DEF_APP_MASK;
		return 2;
	}
	return 0;
}

void FlashInit(void)
{
	uint8_t ret= 0;
	ret= CheckFlash();
	if(0 != ret)
	{
		if(ret==1)
		{
#if (DBG_Printf_Flash==1)	
			printf(" FlashInit  Err !!!!!!!! \r\n");
#endif
			FlashParaSetDefault();
			
		}
		if(0 == Flash_WritePara())
		{
		}
		else
		{	
		}
	}
	else 
	{
	}
}






