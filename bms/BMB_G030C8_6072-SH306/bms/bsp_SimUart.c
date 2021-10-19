#include "main.h"

SimUARTSt  SUartInfo;
uint8_t ucAcquCx= 0;
uint8_t ucRecvTrueCx= 0;
static uint8_t ucSendBitCx= 0;
static uint8_t ucSendByteCx= 0;
 uint8_t ReceveAllow=0;




void SUart_Read_Enable(void)
{
 	SIFBUS_H;
	SUartInfo.Mode=ModNoll;
	ReceveAllow= 1;
	GT_UART2_CMD( ENABLE);	
	SUartInfo.RecdataCount= 0;
	SUartInfo.RecBitCount= 0;
	
}
void SUart_Read_Disable(void)
{
	GT_UART2_CMD( DISABLE);	
	ReceveAllow= 0;
	SUartInfo.RecdataCount= 0;
	SUartInfo.RecBitCount= 0;
}


void UART2RxDataPro(uint16_t data)
{
	if(data!=0xffff)
	{
		SUartInfo.RecData[SUartInfo.DataNum++]=data&0xff;	//将串口rcvbuf中数据存起来 
		SUartInfo.DataNum %=SIMUARTDATAMAX;//更新接收buf数据，不超过200byte
		SUartInfo.SUartIdelTime= 0;

	}
	else//串口数据收完了
	{
		if(SUartInfo.DataNum>0)
		{
			SUartInfo.rcvFlag=1;//其实串口接收本不需要flag，这里的flag其实是有rcv data need handle
			SUartInfo.RecdataCount=SUartInfo.DataNum;
			SUartInfo.DataNum=0;
		}
	}
}

uint8_t CHECK_BIT(void)
{
	uint8_t ret= 0xff;
	if(    ucAcquCx == 0)
	{
		ucAcquCx = 1;
		ret= 0xff;
	}
	ucAcquCx++;
	if(ucAcquCx >= DEF_BIT_SampCount)
	{
		ret = 1;
	}
	return ret;
}

void SUartRxDataPro(void )
{
//	uint8_t value =0;
	switch(SUartInfo.UartStat)
	{
		case COM_START_BIT_DEAL :                         //起始位 必须连续三次采集正确才认可
			{
				if(    ucAcquCx == 0)
				{
					ucAcquCx = 1;
					return ;
				}
				ucAcquCx = 0;
				SUartInfo.UartStat = COM_DATA_BIT_DEAL;

			}
			break;

		case COM_DATA_BIT_DEAL :                         //数据位
			{
				if(  ucAcquCx == 1)
				{
					ucAcquCx = 0;
					return ;
				}
				ucAcquCx= 1;
				if(SUART_Rx == UART_MOD_TTL)
				{
					SUartInfo.RecData[SUartInfo.DataNum]|= (1 << SUartInfo.RecBitCount);
				}
				else
				{
					SUartInfo.RecData[SUartInfo.DataNum] &= ~(1 << SUartInfo.RecBitCount);
				}

				if(SUartInfo.RecBitCount >= 7)
				{
					SUartInfo.RecBitCount = 0;
					SUartInfo.UartStat = COM_STOP_BIT_DEAL;
					SUartInfo.DataNum++;
					SUartInfo.DataNum%=SIMUARTDATAMAX;
				}
				else
				{
					SUartInfo.RecBitCount ++;
				}
			}
			break;

		case COM_CHECK_BIT_DEAL :                         //校验位
			{

			}
			break;

		case COM_STOP_BIT_DEAL :                         //停止位
			{
				ucAcquCx = 0;
				SUartInfo.UartStat = COM_NONE_BIT_DEAL;
				SUartInfo.SUartIdelTime= 0;
				GT_TIM16_CMD( DISABLE); 
			}
			break;
		default:
			break;
	}
}
void SUartTxDataPro(void )
{
	switch(SUartInfo.UartStat)  /*串口发送位状态判断
			COM_NONE_BIT_DEAL=0,
			COM_START_BIT_DEAL,
			COM_DATA_BIT_DEAL,
			COM_LEVEH_DEAL,
			COM_LEVEL_DEAL,
			COM_STOP_BIT_DEAL,
			COM_CHECK_BIT_DEAL,
			COM_ERR_DEAL*/
	{
		case COM_START_BIT_DEAL :
		{
			GPIO_WritePin(SIF_OUT_Port, SIF_OUT_Pin, (GPIO_PinState)0);
			ucAcquCx++;
			if(ucAcquCx >= DEF_BIT_SampCount)    /*由于发送时在立即拉低就进入判断，且自加，所以需要加到4，实际延时3个周期*/
			{
				SUartInfo.UartStat = COM_DATA_BIT_DEAL;
				ucAcquCx = 0;
				ucSendBitCx = 0;
			}
		}
		break;

		case COM_DATA_BIT_DEAL :
		{

			GPIO_WritePin(SIF_OUT_Port, SIF_OUT_Pin, (GPIO_PinState)((SUartInfo.TrxData[ucSendByteCx] >> ucSendBitCx) & 0x01));
			ucAcquCx++;
			if(ucAcquCx >= DEF_BIT_SampCount) 
			{
				ucAcquCx = 0;

				ucSendBitCx ++;

				if(ucSendBitCx >= 8)
				{
					SUartInfo.UartStat = COM_STOP_BIT_DEAL;
					ucSendByteCx++;
				}
			}
		}
		break;

		case COM_CHECK_BIT_DEAL :
		{

		}
		break;

		case COM_STOP_BIT_DEAL :
		{
			GPIO_WritePin(SIF_OUT_Port, SIF_OUT_Pin, (GPIO_PinState)1);

			ucAcquCx++;
			if(ucAcquCx >= DEF_BIT_SampCount)    /*由于发送时在立即拉低就进入判断，且自加，所以需要加到4，实际延时3个周期*/
			{
				ucAcquCx = 0;
				ucSendBitCx = 0;

				if(ucSendByteCx <SUartInfo.TrxdataCount)
				{
					SUartInfo.UartStat = COM_START_BIT_DEAL;
				}
				else
				{
					GT_TIM16_CMD( DISABLE); 
					ucSendByteCx = 0;
					SUartInfo.UartStat = COM_NONE_BIT_DEAL;

					SUart_Read_Enable();
					GT_TIM_SET_COUNTER( 0);
				}
			}
		}
		break;

		default:
			GT_TIM16_CMD( DISABLE); 
			GT_TIM_SET_COUNTER( 0);
		break;

	}
}
void SifTxDataPro(void )
{
	switch(SUartInfo.UartStat)  /*串口发送位状态判断*/
	{
		case COM_START_BIT_DEAL :
		{
			ucAcquCx++;
			if(ucAcquCx >= DEF_SIf_St_count)    /*由于发送时在立即拉低就进入判断，且自加，所以需要加到4，实际延时3个周期*/
			{
				GPIO_WritePin(SIF_OUT_Port, SIF_OUT_Pin, (GPIO_PinState)1);
				SUartInfo.UartStat = COM_LEVEL_DEAL;
				ucAcquCx = 0;
				ucSendBitCx = 0;
			}
			else
			{
				GPIO_WritePin(SIF_OUT_Port, SIF_OUT_Pin, (GPIO_PinState)0);
			}
		}
		break;

		case COM_LEVEL_DEAL:
		{
			if((SIFBUS_READ==0)&&(ucAcquCx==0))
			{
				SUartInfo.UartStat = COM_ERR_DEAL;
				SIFBUSData.SIFConnectFlag= SIF_NoCONNECT;
				break;
			}
			
			SIFBUSData.SIFConnectFlag= SIF_CONNECT;
			
			GPIO_WritePin(SIF_OUT_Port, SIF_OUT_Pin, (GPIO_PinState)0);

			ucAcquCx++;
			if(ucAcquCx >= ((2-(((SUartInfo.TrxData[ucSendByteCx] >> (7-ucSendBitCx)) & 0x01)))%3) )
			{
				ucAcquCx = 0;
				SUartInfo.UartStat = COM_LEVEH_DEAL;
			}
		}
		break;
		case COM_LEVEH_DEAL:
		{
			GPIO_WritePin(SIF_OUT_Port, SIF_OUT_Pin, (GPIO_PinState)1);

			ucAcquCx++;
			if(ucAcquCx >= ((1+(((SUartInfo.TrxData[ucSendByteCx] >> (7-ucSendBitCx)) & 0x01)))%3)) 
			{
				ucAcquCx = 0;

				ucSendBitCx ++;

				if(ucSendBitCx >= 8)
				{
					ucSendByteCx++;
					ucSendBitCx= 0;
				}
				ucSendBitCx%=8;
				if(ucSendByteCx >= SUartInfo.TrxdataCount)
				{
					SUartInfo.UartStat = COM_STOP_BIT_DEAL;
				}
				else
				{
					SUartInfo.UartStat = COM_LEVEL_DEAL;
				}
			}
		}
		break;

		case COM_CHECK_BIT_DEAL :
		{

		}
		break;

		case COM_STOP_BIT_DEAL :
		{
			GPIO_WritePin(SIF_OUT_Port, SIF_OUT_Pin, (GPIO_PinState)0);

			ucAcquCx++;
			if(ucAcquCx >= DEF_SIf_St_count)    /*由于发送时在立即拉低就进入判断，且自加，所以需要加到4，实际延时3个周期*/
			{
				ucAcquCx = 0;
				ucSendBitCx = 0;
				ucSendByteCx = 0;
				SUartInfo.UartStat = COM_NONE_BIT_DEAL;
				SUartInfo.Mode=ModNoll;
				GT_TIM16_CMD( DISABLE); 
				GT_TIM_SET_COUNTER( 0);
			}
		}
		break;

		default:
			ucSendBitCx = 0;
			ucSendByteCx = 0;
			SUartInfo.Mode=ModNoll;
			SUart_Read_Enable();
			GT_TIM16_CMD( DISABLE); 
			GT_TIM_SET_COUNTER(0);
		break;

	}
}

/*
定时器2  中断触发
后如果收到的数据位大于10个 ，
则触发接收完成标志
*/
void SIF_TIM_IRQPro(void)
{

	/*
	HAL_GPIO_TogglePin(SIF_OUT_Port, SIF_OUT_Pin);
	*/
	if(SUartInfo.Mode== TxEn)
	{
		SUartInfo.DataNum= 0;
		SUartTxDataPro();
	}
	else if(SUartInfo.Mode== SIFTxEn)
	{
		SUartInfo.DataNum= 0;
		SifTxDataPro();
	}
	else
	{
		LL_TIM_DisableCounter(TIM16);
	}
}

void UARTBUS_SendMsg(uint8_t *buf, uint16_t len)
{
	uint16_t i = 0;
	for (i = 0; i < len; i++)
	{
		SUartInfo.TrxData[i]=buf[i];
	}

	SUart_Read_Disable();
	SUartInfo.UartStat = COM_START_BIT_DEAL;
	SUartInfo.TrxdataCount= i;
	SUartInfo.Mode= TxEn;
	ucSendBitCx = 0;
	ucSendByteCx = 0;
	//printf("\r\nUARTBUS_SendMsg     %d\r\n",SUartInfo.TrxdataCount);
	GT_TIM_SET_AUTORELOAD(DEF_SUART_SampTime);//34us进一次time16中断
	GT_TIM16_CMD(ENABLE);	
}

void SIFBUS_SendMsg(uint8_t *buf, uint16_t len)
{
	int i = 0;
	for (i = 0; i < len; i++)
	{
		SUartInfo.TrxData[i]=buf[i];
	}
	SUart_Read_Disable();//要发送数据之前disable uart,把数据结构体里面的recv数据清一下
	SUartInfo.UartStat = COM_START_BIT_DEAL;
	/*uart状态取值
	typedef enum
	{
		COM_NONE_BIT_DEAL=0,//发送起始位
		COM_START_BIT_DEAL,
		COM_DATA_BIT_DEAL,
		COM_LEVEH_DEAL,
		COM_LEVEL_DEAL,
		COM_STOP_BIT_DEAL,
		COM_CHECK_BIT_DEAL,
		COM_ERR_DEAL
	}ComStatENUM;
	*/
	SUartInfo.TrxdataCount= i;
	SUartInfo.Mode= SIFTxEn;
	ucSendBitCx = 0;
	ucSendByteCx = 0;
	LL_mDelay(20);
	
	//给time设置500的reload值,前面已经配过了是不是不用配了？
	GT_TIM_SET_AUTORELOAD(SIF_BIT_TIME);//500*16*1/16m=500us
	GT_TIM16_CMD( ENABLE);	//
}


void SUART_Protocol(void)
{
	uint8_t i = 0,cmd = 0;//,ret = 0;
	uint16_t len = 0;
	S_WORD sword;
	
	//printf("SUART_Protocol C= %d ",SUartInfo.RecdataCount);
	if(SUartInfo.RecdataCount<5)//收到的数据太少，改一下recdatacount直接return
	{
		SUartInfo.RecdataCount= 0;
		return ;
	}
	i++;
	
	SIFBUSData.SIFMode = SIF_MODE_GTDS;
	SIFBUSData.SIFConnectFlag= SIF_CONNECT;
	if(0x55 == SUartInfo.RecData[i++])  
	{
		if(0xbb == SUartInfo.RecData[i++]) 
		{
			if(0x03 == SUartInfo.RecData[i++])
			{
				cmd = SUartInfo.RecData[i++];
				sword.word= 0;
				sword.bytes.byte_2 =  SUartInfo.RecData[i++];
				sword.bytes.byte_1 = SUartInfo.RecData[i++];
				len = sword.word;//这里的len表示收到的数据长度，2byte。数据本身肯定很长的。
				SUartInfo.RecData[0]=0xAA;
				BootCmd_Process(cmd,len);//这里len本来是32bit
		//的0x00 00 xx xx，作入参时被类型转换成0x xx xx，只取低16bit
			}
		}
	}
}





