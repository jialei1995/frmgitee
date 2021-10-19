#include <main.h>


#define COMMANDLEN  6
extern S16_BITS GalobleErrBit;
ringBuffer_t RS485Buf = {0,0,{0},0,0,100};
#if(PROJECTCODE==BLX_7220)
uint8_t VolNum_Ture[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,22,23,24,20,21,25};
#else
uint8_t VolNum_Ture[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,0,0,0,0};
#endif

void RingBuf_Write(uint8_t data)
{
	RS485Buf.ringBuf[RS485Buf.tailPosition++] = data;
	RS485Buf.tailPosition %= BUFFER_MAX;

	if(RS485Buf.tailPosition == RS485Buf.headPosition)
		RS485Buf.headPosition %= BUFFER_MAX;
}

uint8_t RingBuf_Read(uint8_t* val)	
{	
	if(RS485Buf.headPosition == RS485Buf.tailPosition)
	{
		RS485Buf.headPosition= 0;
		RS485Buf.tailPosition= 0;
		return 0;
	}
	else
	{
		*val = RS485Buf.ringBuf[RS485Buf.headPosition++];
		RS485Buf.headPosition %= BUFFER_MAX;
		return 1;//读取成功
	}
}


uint8_t DataSBuff[100]= {0};
uint8_t RS485Check(uint8_t* DataBuff,uint8_t len)
{
	uint8_t  i = 0;
	uint8_t CheckValue= 0;
	for(i= 0;i<len;i++)
	{
		CheckValue^=DataBuff[i];
	}
	return CheckValue;
}

void SendArgDataToPC(uint16_t DataNum)
{
	uint8_t i = 0;
	uint8_t Datacount= 0;
	uint8_t CheckLen= 0;
	memset(DataSBuff,0,100);
	DataSBuff[Datacount++]= PC_CMDHEAD;
	DataSBuff[Datacount++]= 0xF3;
	DataSBuff[Datacount++]= CFG_ST_OV>>8;
	DataSBuff[Datacount++]= (CFG_ST_OV&0xff);
	DataSBuff[Datacount++]= CFG_Sof_UV>>8;
	DataSBuff[Datacount++]= (CFG_Sof_UV&0xff);
	DataSBuff[Datacount++]= (CFG_ST_ODC/100)>>8;
	DataSBuff[Datacount++]= (CFG_ST_ODC/100)&0xff;
	DataSBuff[Datacount++]= (DEF_SCD_VOLTAGE*1000+1000)>>8;
	DataSBuff[Datacount++]= ((DEF_SCD_VOLTAGE*1000+1000)&0xff);
	DataSBuff[Datacount++]= CFG_ST_UV>>8;
	DataSBuff[Datacount++]= (CFG_ST_UV&0xff);
	DataSBuff[Datacount++]= (CFG_ST_OCC/100)>>8;
	DataSBuff[Datacount++]= ((CFG_ST_OCC/100)&0xff);
	DataSBuff[Datacount++]= (Def_SH_OTD+TEMP_OFFSET);
	DataSBuff[Datacount++]= (Def_SH_OTC+TEMP_OFFSET);
	DataSBuff[Datacount++]= (Def_Sof_UTD+TEMP_OFFSET);
	DataSBuff[Datacount++]= (Def_Sof_UTC+TEMP_OFFSET);
	DataSBuff[Datacount++]= CFG_ST_UV3>>8;
	DataSBuff[Datacount++]= (CFG_ST_UV3&0xff);
	DataSBuff[Datacount++]= (CFG_ST_OTM+TEMP_OFFSET);
	DataSBuff[Datacount++]= 0;
	DataSBuff[Datacount++]= 0;
	for(i=0;i<24;i++)
	{
		DataSBuff[Datacount++]= flashpara.VoltRate[i];
	}

	CheckLen= Datacount;
	DataSBuff[Datacount++]= RS485Check(DataSBuff,CheckLen);
	
	RS485_SendByte(DataSBuff,Datacount);
}

void SendRArgDataToPC(uint16_t DataNum)
{
	uint8_t Datacount= 0;
	uint8_t CheckLen= 0;

	memset(DataSBuff,0,100);
	DataSBuff[Datacount++]= PC_CMDHEAD;
	DataSBuff[Datacount++]= 0xF4;
	DataSBuff[Datacount++]= CFG_ST_OVR>>8;
	DataSBuff[Datacount++]= (CFG_ST_OVR&0xff);
	DataSBuff[Datacount++]= CFG_Sof_UVR>>8;
	DataSBuff[Datacount++]= (CFG_Sof_UVR&0xff);
	DataSBuff[Datacount++]= CFG_ST_UVR>>8;
	DataSBuff[Datacount++]= (CFG_ST_UVR&0xff);
	DataSBuff[Datacount++]= (Def_SH_OTDR+TEMP_OFFSET);
	DataSBuff[Datacount++]= (Def_SH_OTCR+TEMP_OFFSET);
	DataSBuff[Datacount++]= (Def_Sof_UTDR+TEMP_OFFSET);
	DataSBuff[Datacount++]= (Def_Sof_UTCR+TEMP_OFFSET);
	DataSBuff[Datacount++]= CFG_S0f_UVR>>8;
	DataSBuff[Datacount++]= (CFG_S0f_UVR&0xff);
	DataSBuff[Datacount++]= (CFG_ST_OTMR+TEMP_OFFSET);
	CheckLen= Datacount;
	DataSBuff[Datacount++]= RS485Check(DataSBuff,CheckLen);
	
	RS485_SendByte(DataSBuff,Datacount);
}


void Send485DataToPC(uint16_t DataNum)
{
	uint8_t Datacount= 0;
//	int ERRVAL = 0;
	uint8_t i= 0;
	static uint8_t TempNum= 0;
	uint8_t CheckLen= 0;
	uint16_t KelVTempV=0;
	uint16_t KelVTemp[6]={ 0};
	
			KelVTemp[0]= syspara.cellTemp[0];
			KelVTemp[1]= syspara.cellTemp[1];
			KelVTemp[2]= syspara.mosTemp;
			KelVTemp[3]= syspara.balTemp;

	memset(DataSBuff,0,100);
	switch(DataNum)
	{
		case 0x0100:
		
			KelVTempV= syspara.cellTemp[TempNum]+2731-400;
			DataSBuff[Datacount++]= PC_CMDHEAD;
			DataSBuff[Datacount++]= 0xA1;
			DataSBuff[Datacount++]= ((TempNum<<4)|((KelVTempV>>8)&0x0f));
			DataSBuff[Datacount++]= KelVTempV&0xff;
			TempNum++;
			TempNum=(TempNum%2);
			
			DataSBuff[Datacount++]= syspara.totalSumVolt>>8;
			DataSBuff[Datacount++]= syspara.totalSumVolt;

			DataSBuff[Datacount++]= (syspara.current/10)>>8;
			DataSBuff[Datacount++]= (syspara.current/10);
			DataSBuff[Datacount++]= syspara.SOC;
			DataSBuff[Datacount++]= syspara.SOC;
			DataSBuff[Datacount++]= (syspara.RemainCap/3600/50)>>8;
			DataSBuff[Datacount++]= (syspara.RemainCap/3600/50);
			DataSBuff[Datacount++]= 0;//GalobleErrBit.bytes>>8;
			DataSBuff[Datacount++]= 0;//GalobleErrBit.bytes&0xff;
			DataSBuff[Datacount++]=  syspara.SOH;
			CheckLen= Datacount;
			DataSBuff[Datacount++]= RS485Check(DataSBuff,CheckLen);
			break;
		case 0x0200:
			DataSBuff[Datacount++]= PC_CMDHEAD;
			DataSBuff[Datacount++]= 0xA2;
			DataSBuff[Datacount++]= flashpara.CycleCount>>8;
			DataSBuff[Datacount++]= flashpara.CycleCount;
			DataSBuff[Datacount++]= 0;
			DataSBuff[Datacount++]= 0;
			DataSBuff[Datacount++]= 0;
			DataSBuff[Datacount++]= 0;
			for(i = 0;i<16;i++)
			{
				DataSBuff[Datacount++]= flashpara.id[i];
			}
			DataSBuff[Datacount++]= '0';
			DataSBuff[Datacount++]= RWSERVIN%256;
			DataSBuff[Datacount++]= RWSERVIN;
			CheckLen= Datacount;
			DataSBuff[Datacount++]= RS485Check(DataSBuff,CheckLen);
			break;
		case 0x0301:
		case 0x0302:
		case 0x0303:
		case 0x0304:
			DataSBuff[Datacount++]= PC_CMDHEAD;
			DataSBuff[Datacount++]= 0xA3;
			DataSBuff[Datacount++]= DataNum&0x0f;
			for(i= (((DataNum&0xff)-1)*7);i<((DataNum&0xff)*7);i++)
			{	
				if(i<CFG_Cell_NUM)
				{
					DataSBuff[Datacount++]= syspara.cellVolt[i]>>8;
					DataSBuff[Datacount++]= syspara.cellVolt[i];
				}
				else
				{					
					DataSBuff[Datacount++]= 0;
					DataSBuff[Datacount++]= 0;
				}
			}
			CheckLen= Datacount;
			DataSBuff[Datacount++]= RS485Check(DataSBuff,CheckLen);
			
			break;
		case 0x0311:
		case 0x0312:
		case 0x0313:
		case 0x0314:
			DataSBuff[Datacount++]= PC_CMDHEAD;
			DataSBuff[Datacount++]= 0xA3;
			DataSBuff[Datacount++]= DataNum&0x0f;
			for(i= (((DataNum&0x0f)-1)*7);i<((DataNum&0x0f)*7);i++)
			{	
				if(i<CFG_Cell_NUM)
				{		
					DataSBuff[Datacount++]= syspara.batVolt[VolNum_Ture[i]]>>8;
					DataSBuff[Datacount++]= syspara.batVolt[VolNum_Ture[i]];
				}
				else
				{					
					DataSBuff[Datacount++]= 0;
					DataSBuff[Datacount++]= 0;
				}
			}
			CheckLen= Datacount;
			DataSBuff[Datacount++]= RS485Check(DataSBuff,CheckLen);
			
			break;
		case 0xF000:
			DataSBuff[Datacount++]= PC_CMDHEAD;
			DataSBuff[Datacount++]= 0xF0;
			DataSBuff[Datacount++]= syspara.mosTemp;
			DataSBuff[Datacount++]= syspara.balTemp;
			DataSBuff[Datacount++]= 0;
			CheckLen= Datacount;
			DataSBuff[Datacount++]= RS485Check(DataSBuff,CheckLen);
			break;
		case 0xF100:
			DataSBuff[Datacount++]= PC_CMDHEAD;
			DataSBuff[Datacount++]= 0xF1;
			DataSBuff[Datacount++]= 0>>8;
			DataSBuff[Datacount++]= 0;
			DataSBuff[Datacount++]= 0;
			DataSBuff[Datacount++]= 0>>8;
			DataSBuff[Datacount++]= 0;
			DataSBuff[Datacount++]= 0;
			CheckLen= Datacount;
			DataSBuff[Datacount++]= RS485Check(DataSBuff,CheckLen);
			break;
		case 0xF200:
			DataSBuff[Datacount++]= PC_CMDHEAD;
			DataSBuff[Datacount++]= 0xF2;
			DataSBuff[Datacount++]= syspara.dsgMos;
			DataSBuff[Datacount++]= syspara.chgMos;
			DataSBuff[Datacount++]= 0;
			CheckLen= Datacount;
			DataSBuff[Datacount++]= RS485Check(DataSBuff,CheckLen);
			break;
		case 0xF500:
			DataSBuff[Datacount++]= PC_CMDHEAD;
			DataSBuff[Datacount++]= 0xF5;
			DataSBuff[Datacount++]= (SIFBUSData.SIFConnectFlag==SIF_CONNECT)?0:1;
			DataSBuff[Datacount++]= KelVTemp[0];
			DataSBuff[Datacount++]= KelVTemp[1];
			DataSBuff[Datacount++]= KelVTemp[2];
			DataSBuff[Datacount++]= KelVTemp[3];
			DataSBuff[Datacount++]= KelVTemp[4];
			CheckLen= Datacount;
			DataSBuff[Datacount++]= RS485Check(DataSBuff,CheckLen);
			break;
		default:
			break;
	}
	RS485_SendByte(DataSBuff,Datacount);
}

/**
  * @brief  PC监控协议解析
  * @param  
  * @retval 
  */
void PCMonitor_Process(uint8_t head)
{
	uint8_t sendbuf[100] = {0};
	uint8_t buf[100] = {0};
	uint8_t i,readVal = 0;
	uint8_t num= 0;
	buf[0] = head;
	while((RingBuf_Read(&readVal))&&(num<50))
	{
		buf[i++] = readVal;//有的程序中这里只会读6byte
		num++;
	}
	RS485Buf.headPosition= 0;    //读到50就已经超了，一般只有8，9个字节
	RS485Buf.tailPosition= 0;
	if(num<6)
	{
		return ;
	}
	if(NULL != strstr((const char*)buf,"GTDSDRC"))//看协议，读数据的
	{
				
		#if(Basic==1)
							Delay_ms(600);
		#endif
							Send485DataToPC(0x0301);
							Send485DataToPC(0x0302);
							Send485DataToPC(0x0303);
							Send485DataToPC(0x0304);
							Send485DataToPC(0xF500);
							Send485DataToPC(0x0100);
		#if(Basic==0)
							
							Send485DataToPC(0xF000);
							Send485DataToPC(0xF100);
							Send485DataToPC(0xF200);
							
		#endif
	}
	else if(NULL != strstr((const char*)buf,"GTDSDRT"))//看协议
	{
		Send485DataToPC(0x0311);
		Send485DataToPC(0x0312);
		Send485DataToPC(0x0313);
		Send485DataToPC(0x0314);
	}
	else if(NULL != strstr((const char*)buf,"GTDSCSR"))//soc设置，这隔程序中soc设置不了，别的程序中可以
	{
		if(RingBuf_Read(&readVal))
		{
			syspara.SocChangFlag = readVal;
		}
	}
	
	else if(NULL != strstr((const char*)buf,"GTDSDSP"))//深度休眠指令
	{
		if(num<7)
		{
			return ;
		}
		if(buf[7]==0x5a)
		{
			if(syspara.DeepSleepCMD==0x5a)
			{
				syspara.DeepSleepCMD= 0xa5;
			}
			else
			{
				syspara.DeepSleepCMD= 0x5a;
			}
		}
	}
	else if(NULL != strstr((const char*)buf,"GTDSLSP"))//休眠命令，连续收到2次5a才会休眠，收到1次不处理
	{
		if(num<7)
		{
			return ;
		}
		if(buf[7]==0x5a)
		{
			if(syspara.SleepCMD==0x5a)
			{
				syspara.SleepCMD= 0xa5;
			}
			else
			{
				syspara.SleepCMD= 0x5a;
			}
		}
	}
	else if(NULL != strstr((const char*)buf,"GTDSARC"))//参数读取
	{
		Send485DataToPC(0x0200);
		SendRArgDataToPC(0);
		SendArgDataToPC(0);
	}
	else if(NULL != strstr((const char*)buf,"GTDSCCC"))//电流校准
	{
		syspara.CurrCailFalg=0x55;
	}
	else if(NULL != strstr((const char*)buf,"GTDSASC"))  //标定的电压差数据写到flash
	{
		for(i=0;i<24;i++)
		{
			if(i<(num-6))
			{
				if(flashpara.VoltRate[i] != (int16_t)((int8_t)buf[7+i]))
				{
					sysflag.bits.flashSave_Flag=1;
					flashpara.VoltRate[i] = (int16_t)((int8_t)buf[7+i]);
				}
			}			
		}
	}
	else if(NULL != strstr((const char*)buf,"GTDSIDS"))//通过485写ID
	{
		for(i=0;i<16;i++)
		{
			if(i<(num-6))
			{
				if(flashpara.id[i] != buf[7+i])
				{
					flashpara.id[i] = buf[7+i];
					sysflag.bits.flashSave_Flag=1;
				}
			}			
		}
	}
	else if(NULL != strstr((const char*)buf,"GTDSIDR"))//通过485读ID
	{
		sendbuf[0] = 0x03;
		sendbuf[1] = 0xFB;
		for(i=0;i<16;i++)
		{
			sendbuf[2+i] = 	flashpara.id[i];
		}
		RS485_SendByte(sendbuf,18);
		
	}

}


void RS485_Process(void)
{
	uint8_t readVal = 0;
	
	while(RingBuf_Read(&readVal))
	{
		RS485Buf.dummy = 0;
		while('G' != readVal)
		{
			if(0 == RingBuf_Read(&readVal))
			{					
				RS485Buf.dummy = 1;
				break;
			}
		}
		if(!RS485Buf.dummy)
		{		
			RS485Buf.NodataTime= 0;
			PCMonitor_Process(readVal);		
		}
	}
}

