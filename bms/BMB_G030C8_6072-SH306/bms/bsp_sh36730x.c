#include "bsp_sh36730x.h"
#define SH_ADDRESS              (0x36)


SCONF1_Reg_t SCONF1_Data ;
SCONF2_Reg_t SCONF2_Data ;
SCONF3_Reg_t SCONF3_Data ;
BFLAG1_Reg_t Flag1_Data ;
BFLAG2_Reg_t Flag2_Data ;
BSTATUS_Reg_t BSTATUS_Data ;
//�����ʾreg�ľ���λ�ã��е�reg��������ݸ��������¶�������Ľṹ������ȥ��������������Ӧ��reg����д�������Ϊ����
//���������Ͷ���ı���
#define REG_BFLAG1       			0
#define REG_BFLAG2       			1
#define REG_BSTATUS          			2
#define REG_INTEN				3

#define REG_CONF1				4
#define REG_CONF2				5
#define REG_CONF3				6
#define REG_BALANCE_CH			7
#define REG_BALANCE_CL			8
#define REG_SCD					9       //����    ��·   ����
#define REG_OV_T					10	//��ѹʱ������ 
#define REG_OV_VH				11
#define REG_OV_VL				12	//��ѹ ����    *5.86
#define REG_CONF10				13

#define REG_Cell1H				0x0e
#define REG_Temp1H				0x22
#define REG_CurrH					0x2a

uint8_t  SampVoltFlag= 0;
GT_TYPE SIIC_ReadData(SIMIIC_name_t IICNum, uint8_t ReadAddr,uint8_t* ReadBuf)
{				  
	uint8_t crc=0;	
	uint8_t checkBuf[5] = {0};
	ReadBuf[0]= 0;
	ReadBuf[1]= 0;
	if(IICNum>=CFG_SHUSENUM)
	{
		return GT_OK;
	}
	checkBuf[0] = SH_ADDRESS;
	checkBuf[1] = ReadAddr;
	checkBuf[2] = SH_ADDRESS|0X01;
	
	SH30x_I2C_Start(IICNum);
	SH30x_I2C_SendByte(IICNum,checkBuf[0]) ;   //����������ַд���� 	 
	SH30x_I2C_WaitAck(IICNum);
	SH30x_I2C_SendByte(IICNum,checkBuf[1] );   
	SH30x_I2C_WaitAck(IICNum);    
	SH30x_I2C_Start(IICNum);  	 	   
	SH30x_I2C_SendByte(IICNum,checkBuf[2]);  	   
	SH30x_I2C_WaitAck(IICNum); 

	checkBuf[3] = SH30x_I2C_ReadByte(IICNum);	
	SH30x_I2C_Ack(IICNum); 				
	checkBuf[4] = SH30x_I2C_ReadByte(IICNum);	
	SH30x_I2C_Ack(IICNum); 				
	crc = SH30x_I2C_ReadByte(IICNum);	
	SH30x_I2C_NAck(IICNum); 	
	
	SH30x_I2C_Stop(IICNum);
	
	if(crc == CRC8(checkBuf,5))
	{
		ReadBuf[0]= checkBuf[3];
		ReadBuf[1]= checkBuf[4];
		return GT_OK;
	}
	else 
	{
#ifdef DBG_Printf_SH30X
	printf("SH30x_ReadData<< %d -%x  D1= %x D2= %x >>  ERR \r\n",IICNum,ReadAddr,checkBuf[3],checkBuf[4]);
#endif
		return GT_CRCERR;
	}
}



#define DEF_READ_TIMES 5

//һ�λ��16bit��ֻ&�õ��Լ���Ҫ������
GT_TYPE SH30x_ReadData(SIMIIC_name_t IICNum, uint8_t ReadAddr,uint8_t* ReadBuf)
{
	GT_TYPE ret =GT_OK;
	uint8_t Times= 0;
	while(Times<DEF_READ_TIMES)
	{
		ret= SIIC_ReadData( IICNum,  ReadAddr, ReadBuf);
		if(ret==GT_OK)
		{
			break;
		}
		else
		{
			LL_mDelay(1);
		}
		Times++;
	}
	return ret;
}

GT_TYPE SH30x_WriteData(SIMIIC_name_t IICNum, uint8_t WriteAddr,uint8_t DataToWrite)
{
	uint8_t i = 0;
	uint8_t DataBuffer[4] = {SH_ADDRESS,WriteAddr,DataToWrite,0};
	GT_TYPE ret =GT_OK;
	if(IICNum>=CFG_SHUSENUM)
	{
		return GT_OK;
	}
	DataBuffer[3] = CRC8(DataBuffer, 3);

	SH30x_I2C_Start(IICNum);  	
	for(i=0;i<4;i++)
	{
		SH30x_I2C_SendByte(IICNum,DataBuffer[i]); //����������ַ0XA0,д���� 		 
		SH30x_I2C_WaitAck(IICNum);		
	}
	SH30x_I2C_Stop(IICNum);//����һ��ֹͣ���� 	  
#if 0
	//LL_mDelay(5);
	DataBuffer[0]=0;
	DataBuffer[1]=0;
	DataBuffer[2]=0;
	DataBuffer[3]=0;
	ret=SH30x_ReadData(IICNum,WriteAddr,DataBuffer);
	if((DataToWrite == DataBuffer[0])&&(ret == GT_OK ))
	{
		ret= GT_OK;
	}
	else 
	{
#ifdef DBG_Printf_SH30X
		printf("SH30x_WriteData<< %d - %x >W= %x R= %x>  ERR \r\n",IICNum,WriteAddr,DataToWrite,DataBuffer[0]);
#endif
		ret= GT_WriteERR;
	}
#endif
	return ret;
}

GT_TYPE SH_GetCellVolt(uint16_t * VoltBuff)
{

	int i=0;
	uint8_t  RegBuff[2];
	S_HALFWORD halfword;
#ifdef DBG_Printf_Volt
	printf("sampVolt : ");
#endif
	for(i=0;i<DEF_TOTAL_CELLNUM;i++)
	{
		RegBuff[0]= 0;
		RegBuff[1]= 0;
		SH30x_ReadData((SIMIIC_name_t)(i/10),(REG_Cell1H+(i%10)*2),RegBuff);
		halfword.bytes.byte_h = RegBuff[0];
		halfword.bytes.byte_l  = RegBuff[1];//
		VoltBuff[i]=(uint16_t)((halfword.halfword*6000L)/4096);
#ifdef DBG_Printf_Volt
		printf("%d ",VoltBuff[i]);
#endif
	}
#ifdef DBG_Printf_Volt
	printf("\r\n");
#endif
	return GT_OK;
}
GT_TYPE SH_GetCUR(int32_t * CUR)
{
	uint8_t  RegBuff[2];
	int64_t  Value= 0;
	S_HALFWORD halfword;
#ifdef DBG_Printf_CURR

	printf("sampCURR:");
#endif
	SH30x_ReadData(SIMIIC_1,REG_CurrH,RegBuff);
	halfword.bytes.byte_h = RegBuff[0];
	halfword.bytes.byte_l  = RegBuff[1];//
	if(halfword.halfword>=0x1000)//  bit12 wei 1 ʱ�ŵ�   0  ���
	{
		halfword.halfword|= 0xe000;
	}
	// 0-100mV    I= 1000*CUR/32768/R             (��)
	//				 = 1000*CUR*1000*100/32768/R    (0.01m��)
	//				 = 390625*CUR/128/R    (0.01m��)
	Value= (int16_t)halfword.halfword;
	*CUR=(int32_t)(0-(390625*Value/(128*CFG_CURR_RES) ));
#ifdef DBG_Printf_CURR
	printf("%x %d\r\n",halfword.halfword,*CUR);
#endif
	return GT_OK;
}

GT_TYPE SH_GetCellTemp(uint16_t * TempBuff)
{
	int i=0;
	uint8_t  RegBuff[2];
	S_HALFWORD halfword;
#ifdef DBG_Printf_Temp

	printf("sampTemp:");
#endif
	for(i=0;i<DEF_TOTAL_TEMPNUM;i++)
	{
		SH30x_ReadData((SIMIIC_name_t)(i/2),(REG_Temp1H+(i%2)*2),RegBuff);
		halfword.bytes.byte_h = RegBuff[0];
		halfword.bytes.byte_l  = RegBuff[1];//

		TempBuff[i]=halfword.halfword ;

#ifdef DBG_Printf_Temp
		printf("%d  ",TempBuff[i]);
	}
	printf(" \r\n");
#else
	}
#endif

	return GT_OK;
}
/*************����״̬��ȡ****************/

GT_TYPE  SH_CHGR_CHECK(uint8_t mod)
{
	SCONF1_Data.Bits.CHGR_EN= mod;
	SH30x_WriteData(SIMIIC_1,REG_CONF1,SCONF1_Data.Vaule);
#ifdef DBG_Printf_SH30X
	printf("SH_CHGR_CHECK:  %x\r\n", mod);
#endif
	return GT_OK;
}

GT_TYPE  SH_LOAD_CHECK(uint8_t mod)
{
	SCONF1_Data.Bits.LOAD_EN= mod;
	SH30x_WriteData(SIMIIC_1,REG_CONF1,SCONF1_Data.Vaule);
#ifdef DBG_Printf_SH30X
	printf("SH_LOAD_CHECK:  %x\r\n", mod);
#endif
	return GT_OK;
}

GT_TYPE  SH_ClrAlarm(void)
{
	SCONF1_Data.Bits.LTCLR_EN= 1;
	SH30x_WriteData(SIMIIC_1,REG_CONF1,SCONF1_Data.Vaule);
	SH30x_WriteData(SIMIIC_2,REG_CONF1,SCONF1_Data.Vaule);
	SH30x_WriteData(SIMIIC_3,REG_CONF1,SCONF1_Data.Vaule);
	LL_mDelay(10);
	SCONF1_Data.Bits.LTCLR_EN= 0;
	SH30x_WriteData(SIMIIC_1,REG_CONF1,SCONF1_Data.Vaule);
	SH30x_WriteData(SIMIIC_2,REG_CONF1,SCONF1_Data.Vaule);
	SH30x_WriteData(SIMIIC_3,REG_CONF1,SCONF1_Data.Vaule);
	return GT_OK;
}



GT_TYPE SH_GetBSTATUS(void)
{
	GT_TYPE ret =GT_OK;
	uint8_t RegBuff[2]={0};
	ret=SH30x_ReadData(SIMIIC_1,REG_BSTATUS,RegBuff);
	if(ret ==GT_OK)
	{
		BSTATUS_Data.Vaule= RegBuff[0]&0xff;
	}
#ifdef DBG_Printf_SH30X
	printf("GetBSTATUS:%d   S=%x   int= %x\r\n",ret,BSTATUS_Data.Vaule, RegBuff[1]);
#endif
	return GT_OK;
}
GT_TYPE SH_GetSFlag(void)
{
	GT_TYPE ret =GT_OK;
	uint8_t RegBuff[2]={0};
	//��iic1��sh306оƬ
	ret=SH30x_ReadData(SIMIIC_1,REG_BFLAG1,RegBuff); //һ�λ��16bit��ֻ&�õ��Լ���Ҫ������
	if(ret ==GT_OK)
	{
		Flag1_Data.Vaule= RegBuff[0]&0xff;
		Flag2_Data.Vaule= RegBuff[1]&0xff;
	}
	//vadc_flag��adcת����ɵĵı�־������Ҫ�ռ�ÿ��sh306��vadc_flagȷ���ǲ���ת�����
	SampVoltFlag|=Flag2_Data.Flag_Bits.VADC_FLG;//��һ�£���bitλ����ȡ֮��Ӳ�����Զ�����
	//�����������λ ����������SH306-iic1
	if(Flag2_Data.Flag_Bits.RST_FLG==1)
	{
		SH30x_init(SIMIIC_1);
	}
	//��iic2��sh306оƬ
	ret=SH30x_ReadData(SIMIIC_2,REG_BFLAG1,RegBuff);
	if(ret ==GT_OK)
	{
		Flag1_Data.Vaule|= RegBuff[0]&0xff;
		Flag2_Data.Vaule|= RegBuff[1]&0xff;
	}
	//�����������λ ����������SH306
	SampVoltFlag|=(Flag2_Data.Flag_Bits.VADC_FLG<<1);//��һ�£���bitλ����ȡ֮��Ӳ�����Զ�����
	if(Flag2_Data.Flag_Bits.RST_FLG==1)
	{
		SH30x_init(SIMIIC_2);
	}	
	//iic3��sh306оƬ
	ret=SH30x_ReadData(SIMIIC_3,REG_BFLAG1,RegBuff);
	if(ret ==GT_OK)
	{
		Flag1_Data.Vaule|= RegBuff[0]&0xff;
		Flag2_Data.Vaule|= RegBuff[1]&0xff;
	}
	//�����������λ ����������SH306
	SampVoltFlag|=(Flag2_Data.Flag_Bits.VADC_FLG<<2);//��һ�£���bitλ����ȡ֮��Ӳ�����Զ�����
	if(Flag2_Data.Flag_Bits.RST_FLG==1)
	{
		SH30x_init(SIMIIC_3);
	}
	//����Ĵ�λ����SampVoltFlag��ֵ�����ǣ���λ���洢
#ifdef DBG_Printf_SH30X
	printf("GetSFlag:   %x  %x   %x \r\n",Flag1_Data.Vaule,Flag2_Data.Vaule,SampVoltFlag);
#endif
	return GT_OK;
}


GT_TYPE SH_EnterLowPower(void)
{

	SCONF1_Data.Bits.PD_EN=1;
	SH30x_WriteData(SIMIIC_3,REG_CONF10,0x33);
	SH30x_WriteData(SIMIIC_3,REG_CONF1,SCONF1_Data.Vaule);

	SH30x_WriteData(SIMIIC_2,REG_CONF10,0x33);
	SH30x_WriteData(SIMIIC_2,REG_CONF1,SCONF1_Data.Vaule);

	SH30x_WriteData(SIMIIC_1,REG_CONF10,0x33);
	SH30x_WriteData(SIMIIC_1,REG_CONF1,SCONF1_Data.Vaule);
	LL_mDelay(5000);
	
	SCONF1_Data.Bits.PD_EN=0;
	return GT_OK;
}
GT_TYPE SH_Sleep_C(void)
{
	SCONF3_Data.Bits.SCAN_C= DEF_SCAN100ms;
	SCONF3_Data.Bits.VADC_C= 1;         //������ѹ�¶Ȳɼ�
	SCONF3_Data.Bits.VADC_EN= 1;      //����VADCģ��
	SCONF3_Data.Bits.CBIT_C= 1;		//ѡ�� 13λ
	SCONF3_Data.Bits.CADC_M= 1;		//���������ɼ�
	SCONF3_Data.Bits.CADC_EN= 1;		//����CADC
	SH30x_WriteData(SIMIIC_1,REG_CONF3,SCONF3_Data.Vaule);
	SH30x_WriteData(SIMIIC_2,REG_CONF3,SCONF3_Data.Vaule);
	SH30x_WriteData(SIMIIC_3,REG_CONF3,SCONF3_Data.Vaule);
	return GT_OK;
}

GT_TYPE SH_OpenAlarmInt(void)
{

	SH30x_WriteData(SIMIIC_1,REG_INTEN,0x70);
	SH30x_WriteData(SIMIIC_2,REG_INTEN,0x70);
	SH30x_WriteData(SIMIIC_3,REG_INTEN,0x70);
	return GT_OK;
}

GT_TYPE SH_MOSCtrl(uint8_t CHGCtrl,uint8_t DSGCtrl)
{
	SCONF2_Data.Bits.CHG_C= CHGCtrl;
	SCONF2_Data.Bits.DSG_C= DSGCtrl;
	SH30x_WriteData(SIMIIC_1,REG_CONF2,SCONF2_Data.Vaule);
	SH30x_WriteData(SIMIIC_2,REG_CONF2,SCONF2_Data.Vaule);
	SH30x_WriteData(SIMIIC_3,REG_CONF2,SCONF2_Data.Vaule);
	return GT_OK;
}

GT_TYPE SH_BALANCESET(uint32_t BalanceFlag)
{
	SH30x_WriteData(SIMIIC_1,REG_BALANCE_CL,(BalanceFlag>>0)&0x1f);
	SH30x_WriteData(SIMIIC_1,REG_BALANCE_CH,(BalanceFlag>>5)&0x1f);
	SH30x_WriteData(SIMIIC_2,REG_BALANCE_CL,(BalanceFlag>>10)&0x1f);
	SH30x_WriteData(SIMIIC_2,REG_BALANCE_CH, (BalanceFlag>>15)&0x1f);
	SH30x_WriteData(SIMIIC_3,REG_BALANCE_CL , (BalanceFlag>>20)&0x1f);
	SH30x_WriteData(SIMIIC_3,REG_BALANCE_CH, (BalanceFlag>>25)&0x1f);
	return GT_OK;
}

GT_TYPE SH_StrlVCAD(uint8_t comd)
{
	GT_TYPE ret =GT_OK;
	uint8_t Times= 0;

	SCONF3_Data.Bits.SCAN_C= DEF_VolSacnTime;
	SCONF3_Data.Bits.VADC_C= comd;         //������ѹ�¶Ȳɼ�
	SCONF3_Data.Bits.VADC_EN= comd;      //����VADCģ��
	SCONF3_Data.Bits.CBIT_C= 1;		//ѡ�� 13λ
	SCONF3_Data.Bits.CADC_M= 1;		//���������ɼ�
	SCONF3_Data.Bits.CADC_EN= 1;	//����CADC
	SH30x_WriteData(SIMIIC_1,REG_CONF3,SCONF3_Data.Vaule);
	SH30x_WriteData(SIMIIC_2,REG_CONF3,SCONF3_Data.Vaule);
	SH30x_WriteData(SIMIIC_3,REG_CONF3,SCONF3_Data.Vaule);
	if(comd==1)
	{
		LL_mDelay(10);
		while(Times<DEF_READ_TIMES)
		{
			ret =	SH_GetSFlag();
			if((SampVoltFlag==7)&&(ret == GT_OK))
			{
				SampVoltFlag= 0;
				break;
			}
			else
			{
				LL_mDelay(2);
			}
			Times++;
		}
	}
	return GT_OK;
	
}

GT_TYPE SH30x_init(SIMIIC_name_t IICNum )
{
	S_HALFWORD halfword;
	SH30x_IICPort_Init(IICNum);
	//���õ���������Χ����·��ѹ�� ��·�ж�ʱ�� 
	SH30x_WriteData(IICNum,REG_SCD,CONFG6_VALUE);
	//���ù�ѹʱ��      ��ŵ�����ֵ
	SH30x_WriteData(IICNum,REG_OV_T,CONFG7_VALUE);
	//���ù�ѹ����ֵ
	halfword.halfword= (CFG_ST_OV+50)*100/586;
	SH30x_WriteData(IICNum,REG_OV_VH,halfword.bytes.byte_h);
	SH30x_WriteData(IICNum,REG_OV_VL,halfword.bytes.byte_l);


	SCONF3_Data.Bits.SCAN_C= DEF_VolSacnTime;
	SCONF3_Data.Bits.VADC_C= 1;         //������ѹ�¶Ȳɼ�
	SCONF3_Data.Bits.VADC_EN= 1;      //����VADCģ��
	SCONF3_Data.Bits.CBIT_C= 1;		//ѡ�� 13λ
	SCONF3_Data.Bits.CADC_M= 1;		//���������ɼ�
	SCONF3_Data.Bits.CADC_EN= 1;		//����CADC
	SH30x_WriteData(IICNum,REG_CONF3,SCONF3_Data.Vaule);
	
	SCONF1_Data.Bits.CHGR_EN= 0;
	SCONF1_Data.Bits.LOAD_EN= 0;
	SCONF1_Data.Bits.OV_EN= 1;
	SCONF1_Data.Bits.SC_EN= 1;
	SCONF1_Data.Bits.WDT_EN= 0;
	SCONF1_Data.Bits.PD_EN= 0;
	SCONF1_Data.Bits.CTLD_EN= 0;
	SCONF1_Data.Bits.LTCLR_EN= 0;
	SH30x_WriteData(IICNum,REG_CONF1,SCONF1_Data.Vaule);
	return GT_OK;
}


