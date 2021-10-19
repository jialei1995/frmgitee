#include "main.h"
SIFBUSst SIFBUSData;

uint8_t SHOWSOC= 0;
//u16 bufftestnum= 0;




uint8_t BootFlag= 0;

static const uint16_t CrcCcittTable[] = {
0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};
uint16_t Crc16_Calculate(uint8_t* RCC_PSource, uint16_t RCC_DataLen)
{
	uint32_t i, j, crc = 0;
	for (i = 0; i < RCC_DataLen; i++)
	{
		j = (uint16_t )((crc >> 8) ^ RCC_PSource[i]);
		crc = (uint16_t )((crc << 8) ^ CrcCcittTable[j]);
	}
	return crc;
}

uint8_t Data_xor(uint8_t* buf,uint8_t n)
{	 		  	  
	uint8_t i = 0;
	uint8_t ret = 0;
	for(i=0;i<n;i++)
	{
		ret ^= *buf++;
	}
	return ret;
} 




uint8_t GTDS_SIF_process(uint8_t *TempDataBuff)
{
	uint8_t len = 0;
	uint8_t i = 0;
	S_WORD word;
	len= 0;
	for(i=0;i<CFG_Cell_NUM;i++)
	{
		word.word = syspara.cellVolt[i];
		TempDataBuff[len++] = word.bytes.byte_2;
		TempDataBuff[len++] = word.bytes.byte_1;
	}
	for(;i<24;i++)
	{
		TempDataBuff[len++] = 0;
		TempDataBuff[len++] = 0;
	}
	word.word = syspara.voltMax;
	TempDataBuff[len++] = word.bytes.byte_2;
	TempDataBuff[len++] = word.bytes.byte_1;
	word.word = syspara.voltMin;
	TempDataBuff[len++] = word.bytes.byte_2;
	TempDataBuff[len++] = word.bytes.byte_1;
	word.word = (syspara.voltAvg/10*CFG_Cell_NUM);
	TempDataBuff[len++] = word.bytes.byte_2;
	TempDataBuff[len++] = word.bytes.byte_1;
	word.word = syspara.current/10+30000;
	TempDataBuff[len++] = word.bytes.byte_2;
	TempDataBuff[len++] = word.bytes.byte_1;
	TempDataBuff[len++] = syspara.SOC;// SHOWSOC;
	TempDataBuff[len++] = (((syspara.chgMos<<0)|(syspara.dsgMos<<1))&0x03);
	word.word = userAlarm.bytes;
	TempDataBuff[len++] = word.bytes.byte_2;
	TempDataBuff[len++] = word.bytes.byte_1;
	TempDataBuff[len++] = syspara.mosTemp;
	TempDataBuff[len++] = syspara.balTemp;
	TempDataBuff[len++] = syspara.cellTemp[0];
	TempDataBuff[len++] = syspara.cellTemp[1];
	TempDataBuff[len++] = 0;//(CFG_DESIGN_CAPACITY/100);
	TempDataBuff[len++] = 0;//RWSERVIN;
	TempDataBuff[len++] = 0;//PROJECTCODE;
	TempDataBuff[len++] = 0;//PROJECTCODE;
	return len;
}


static uint16_t SIF_QueryDataPack(uint8_t* SifSendBuff,uint8_t func,uint16_t rcvLen)
{
	uint16_t index = 7;//,div = 0,over = 0;

	uint8_t i= 0;
//	uint32_t firmWareSize = 0;
//	S_WORD sword;
//	S_DWORD sdword;
//  printf("CMD= %x  ",func);

	switch(func)
	{
		case 0xA0:
		{
			SIFBUSData.SIFMode=SIF_MODE_GTDS;
			BootFlag = 1;		
		}
		break;
		case 0xA4:
		{
			SifSendBuff[index++] = 1;
		}
		break;

		case 0xE0:
		{
			SIFBUSData.SIFMode=SIF_MODE_GTDS;
			index+= GTDS_SIF_process(SifSendBuff+index);
		}
		break;
		case 0xFE:
		{
			SIFBUSData.SIFMode=SIF_MODE_GTDS;
			SifSendBuff[index++] = HARDCODE_M;
			SifSendBuff[index++] = HARDCODE_S;
			SifSendBuff[index++] = RWSERVIN;
			SifSendBuff[index++] = BOOTSERVIN;
			SifSendBuff[index++] = FACTURECODE;
			SifSendBuff[index++] = CFG_Cell_NUM;
			SifSendBuff[index++] = (CFG_DESIGN_CAPACITY/100);
			SifSendBuff[index++] = BATTERY_TYPE;
			SifSendBuff[index++] = ((PROJECTCODE>>8)&0xff);
			SifSendBuff[index++] = (PROJECTCODE&0xff);
		}
		break;
		case 0xE1:
		{
			SIFBUSData.SIFMode=SIF_MODE_GTDS;
			SifSendBuff[index++] = ((flashpara.CycleCount)>>8);
			SifSendBuff[index++] = (flashpara.CycleCount);
			SifSendBuff[index++] = ((flashpara.shortCnt)>>8);
			SifSendBuff[index++] = (flashpara.shortCnt);
			SifSendBuff[index++] = ((flashpara.P_UV_Count)>>8);
			SifSendBuff[index++] = (flashpara.P_UV_Count);
			SifSendBuff[index++] = ((flashpara.cyc_CHG)>>8);
			SifSendBuff[index++] = (flashpara.cyc_CHG);
			SifSendBuff[index++] = syspara.SOH;
		}
		break;
		
		case 0xEb:
		{
			SIFBUSData.SIFMode=SIF_MODE_GTDS;
			if((SUartInfo.RecData[7]==99)
			&&(SUartInfo.RecData[8]==99)
			&&(SUartInfo.RecData[9]==99)
			&&(1))
			{
				syspara.CurrCailFalg=0x55;
			}
			else
			{
				for(i=7;i<rcvLen;i++)
				{
					if(i>=20)
					{
						break;
					}
					if(flashpara.id[i-7]!= SUartInfo.RecData[i])
					{
						flashpara.id[i-7]= SUartInfo.RecData[i];
						sysflag.bits.flashSave_Flag= 1;
					}
				}
			}
		}
		break;
#if (CFG_SET_IDFUN==1)
		case 0xEA:
		{
			SIFBUSData.SIFMode=SIF_MODE_GTDS;
			for(i=0;i<16;i++)
			{
				SifSendBuff[index++]= 	flashpara.id[i]&0xff;
			}
		}
		break;
#endif
		default:
			break;
	}	
	return index-7;
}

/**
  * @brief  BOOT命令处理 
  * @param  
  * @retval 
  */
uint8_t BootCmd_Process(uint8_t cmd,uint16_t length)
{
	uint16_t i = 0;	
	uint8_t SifSendBuff[100] = {0};
	S_WORD sword ;	
	S_WORD sCks;	
	S_WORD sRcv;	


	
	for(i=7;i<9+length;i++)//7个字节头+2个字节校验
	{
		if(i>=SIMUARTDATAMAX)//收到的数据太多了
		{
			return 0 ;
		}
	}		
	sCks.word = Crc16_Calculate(SUartInfo.RecData,i-2);//获取最后的2byte校验位
	sRcv.word=0;
	sRcv.bytes.byte_2 = SUartInfo.RecData[i-2];
	sRcv.bytes.byte_1 = SUartInfo.RecData[i-1];
	//printf("CRC : R= %x  S= %x",sRcv.word,sCks.word);
	if(sCks.word == sRcv.word)
	{				
		SifSendBuff[0] = 0XAA;
		SifSendBuff[1] = 0X55;
		SifSendBuff[2] = 0XBB;
		SifSendBuff[3] = 0X03;
		SifSendBuff[4] = cmd;
		sword.word = SIF_QueryDataPack(SifSendBuff,cmd,length);	//根据收到的命令处理
		SifSendBuff[5] = sword.bytes.byte_2;
		SifSendBuff[6] = sword.bytes.byte_1;
		sCks.word = Crc16_Calculate(SifSendBuff,7+sword.word);
		SifSendBuff[7+sword.word] = sCks.bytes.byte_2;
		SifSendBuff[8+sword.word] = sCks.bytes.byte_1;	

		if(((cmd == 0xA1)||(cmd == 0xA0))&&(BootFlag == 1))
		{

			flashpara.appmask=0xABCD;//进下载模式，下载完程序才会出来
			Flash_WritePara();
			NVIC_SystemReset();
		}
		else
		{
			UARTBUS_SendMsg(SifSendBuff,9+sword.word);
		}
		//printf("\r\n");
		return 1;
	}
	//printf("\r\n");
	return 0;
}


#if(CFG_SET_IDFUN == 1)

void ComVesionProcess(void)
{
	
	uint8_t buf[20] = {0x03,0x16,0x00,0x00};
	buf[2]= 0x0A	;//
	buf[3]= 1	;// 1:4814    2:4820    3:4824    4:4827
	buf[4]= 0x0A;//厂商
	buf[5]= (flashpara.id[0]&0xff);//年
	buf[6]= (flashpara.id[1]&0xff);//月
	buf[7]= (flashpara.id[2]&0xff);//日
	buf[8]= RWSERVIN;//软件版本
	buf[9]= (flashpara.id[3]&0xff);//流水码高字节
	buf[10]= (flashpara.id[4]&0xff);//流水码低字节
	buf[11]= 0x03;//电池类型
	buf[12]= 0;//预留
	buf[13]= 0;//预留
	buf[14]= Data_xor(buf,14);//
	SIFBUS_SendMsg(buf,15);

}

#endif
void ShowSOCprocess(void)
{
#if(FACTURECODE==GTDS_FAC_CF)
	SHOWSOC= syspara.SOC;//SOC
#else
	static uint8_t CutCOUNT= 0;
	static uint8_t AddCOUNT= 0;
	static uint8_t Show0Flag= 0;
	if((syspara.SOC<=2))
	{
		AddCOUNT= 0;
		if(CutCOUNT<4)
		{
			CutCOUNT++;
			SHOWSOC= 2;//SOC
		}
		else if(CutCOUNT<8)
		{
			CutCOUNT++;		
			SHOWSOC=  1;
		}
		else 
		{	
			SHOWSOC =  0;
			Show0Flag=1;
		}
	}
	else if(syspara.SOC==3)
	{
		CutCOUNT= 0;
		if(Show0Flag==1)
		{
			if(AddCOUNT<10)
			{
				AddCOUNT++;
				SHOWSOC= 0;//SOC
			}
			else if(AddCOUNT<20)
			{
				AddCOUNT++;
				SHOWSOC=  1;
			}
			else 
			{
				SHOWSOC = syspara.SOC;//SOC
				Show0Flag= 0;
			}
		}
		else
		{
			SHOWSOC = syspara.SOC;//SOC
		}
	}
	else
	{
		CutCOUNT= 0;
		AddCOUNT= 0;
		Show0Flag= 0;
		SHOWSOC= syspara.SOC;//SOC
	}
#endif
}

void CFSIFProtocol(void)
{
	uint8_t buf[12] = {0x03,0x01,0x00,0x00};
	buf[2]= ((1<<7)|
			(userAlarm.bits.O_T_D<<6)|
			(userAlarm.bits.U_T_D<<5)|
			(((syspara.SOH<60)?1:0)<<4)|
			(userAlarm.bits.D_M_E<<3)|
			(userAlarm.bits.C_M_E<<3)|
			(userAlarm.bits.S_C_D<<2))	;//报警
#if(CFG_XIEYI_NUM== CFG_XIEYI_12)
	buf[3]= syspara.voltMax/10-185;//SOC
#endif


	buf[4]=syspara.SOC;//SHOWSOC;//SOC
	
	buf[5]= syspara.SOH;//SOH
	if(syspara.cellTemp[0]>=40)
	{
		buf[6]=syspara.cellTemp[0]-40;
	}
	else
	{
		buf[6]=40-syspara.cellTemp[0];
		SetBit(buf[6],7);
	}
	if(syspara.cellTemp[1]>=40)
	{
		buf[7]=syspara.cellTemp[1]-40;
	}
	else
	{
		buf[7]=40-syspara.cellTemp[1];
		SetBit(buf[7],7);
	}
	buf[8]= (uint8_t)(flashpara.CycleCount>>8);//循环次数高字节
	buf[9]= (uint8_t)(flashpara.CycleCount);//循环次数低字节
#if(CFG_XIEYI_NUM== CFG_XIEYI_12)
	buf[10]= syspara.voltMin/10-185;//SOC
#endif
	buf[11]= Data_xor(buf,11);//
	
	SIFBUS_SendMsg(buf,12);
	//UARTBUS_SendMsg("UARTBUS_SendMsg \r\n",18);
}

void SIFProcess(uint32_t SysTime)
{
	static uint32_t Timed=0;
#if((CFG_SET_IDFUN==1))
	static uint8_t S_Flag=0;
	if((abs(SysTime-Timed))>500)
#else
	if((abs(SysTime-Timed))>1000)
#endif
	{
		Timed= SysTime;
		//ShowSOCprocess();
		if(SIFBUSData.SIFMode!=SIF_MODE_GTDS) //若不是测试模式是车载模式
		{
			if(SIFBUSData.SIFConnectFlag==1)  //刚开始是非链接状态进入后面分支
			{
#if((CFG_SET_IDFUN==1))
				if(S_Flag%4==0)
				{
					ComVesionProcess();
					S_Flag= 0;
					S_Flag++;
				}
				else
				{
					CFSIFProtocol();
					S_Flag++;
				}
#else
				CFSIFProtocol();//纯发送
#endif
			}
			else
			{
				SIFBUS_H;  //一线通不通信，但是应该将tx电平置1，不然别人拉不起来,无法发数据
				LL_mDelay(20);//此时置为接收状态，若读到1，则SIFConnectFlag变成1.
				SIFBUSData.SIFConnectFlag= SIFBUS_READ; //SIFConnectFlag
			}
		}
		else //是gtds测试模式，测试模式
		{
			SIFBUSData.SIFModetime++;//1s没有通信就++,5s没有通信进车载模式
			SIFBUS_H;//tx设置为1
		}
	}

	if((SIFBUSData.SIFMode==SIF_MODE_GTDS)||( SUartInfo.rcvFlag==1))//如果为测试模式或uart的接收flag=1，
	//这里其实已经收到数据了，是需要处理收到的数据，在中断中就已经把数据存到了全局变量中
	{
		if(SIFBUSData.SIFModetime >= SIF_TESTMODE_TIMEOUT)//5s没接收到cmd数据,进入车载(sif)模式
		{
			SIFBUSData.SIFMode = SIF_MODE_NULL;
		}
		if( SUartInfo.rcvFlag==1)//若有收到cmd数据,设置sifmodetime=0
		{
			//printf("SUART_Protocol T= %d f= %d C= %d\r\n",SIFBUSData.SIFModetime,SUartInfo.rcvFlag,SUartInfo.RecdataCount );
			SIFBUSData.SIFModetime=0;
			SUART_Protocol();//进行串口的通信协议直接收数据
			SUartInfo.rcvFlag= 0;//数据处理完
		}
	}

}


