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
		SUartInfo.RecData[SUartInfo.DataNum++]=data&0xff;	//������rcvbuf�����ݴ����� 
		SUartInfo.DataNum %=SIMUARTDATAMAX;//���½���buf���ݣ�������200byte
		SUartInfo.SUartIdelTime= 0;

	}
	else//��������������
	{
		if(SUartInfo.DataNum>0)
		{
			SUartInfo.rcvFlag=1;//��ʵ���ڽ��ձ�����Ҫflag�������flag��ʵ����rcv data need handle
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
		case COM_START_BIT_DEAL :                         //��ʼλ �����������βɼ���ȷ���Ͽ�
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

		case COM_DATA_BIT_DEAL :                         //����λ
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

		case COM_CHECK_BIT_DEAL :                         //У��λ
			{

			}
			break;

		case COM_STOP_BIT_DEAL :                         //ֹͣλ
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
	switch(SUartInfo.UartStat)  /*���ڷ���λ״̬�ж�
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
			if(ucAcquCx >= DEF_BIT_SampCount)    /*���ڷ���ʱ���������;ͽ����жϣ����Լӣ�������Ҫ�ӵ�4��ʵ����ʱ3������*/
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
			if(ucAcquCx >= DEF_BIT_SampCount)    /*���ڷ���ʱ���������;ͽ����жϣ����Լӣ�������Ҫ�ӵ�4��ʵ����ʱ3������*/
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
	switch(SUartInfo.UartStat)  /*���ڷ���λ״̬�ж�*/
	{
		case COM_START_BIT_DEAL :
		{
			ucAcquCx++;
			if(ucAcquCx >= DEF_SIf_St_count)    /*���ڷ���ʱ���������;ͽ����жϣ����Լӣ�������Ҫ�ӵ�4��ʵ����ʱ3������*/
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
			if(ucAcquCx >= DEF_SIf_St_count)    /*���ڷ���ʱ���������;ͽ����жϣ����Լӣ�������Ҫ�ӵ�4��ʵ����ʱ3������*/
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
��ʱ��2  �жϴ���
������յ�������λ����10�� ��
�򴥷�������ɱ�־
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
	GT_TIM_SET_AUTORELOAD(DEF_SUART_SampTime);//34us��һ��time16�ж�
	GT_TIM16_CMD(ENABLE);	
}

void SIFBUS_SendMsg(uint8_t *buf, uint16_t len)
{
	int i = 0;
	for (i = 0; i < len; i++)
	{
		SUartInfo.TrxData[i]=buf[i];
	}
	SUart_Read_Disable();//Ҫ��������֮ǰdisable uart,�����ݽṹ�������recv������һ��
	SUartInfo.UartStat = COM_START_BIT_DEAL;
	/*uart״̬ȡֵ
	typedef enum
	{
		COM_NONE_BIT_DEAL=0,//������ʼλ
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
	
	//��time����500��reloadֵ,ǰ���Ѿ�������ǲ��ǲ������ˣ�
	GT_TIM_SET_AUTORELOAD(SIF_BIT_TIME);//500*16*1/16m=500us
	GT_TIM16_CMD( ENABLE);	//
}


void SUART_Protocol(void)
{
	uint8_t i = 0,cmd = 0;//,ret = 0;
	uint16_t len = 0;
	S_WORD sword;
	
	//printf("SUART_Protocol C= %d ",SUartInfo.RecdataCount);
	if(SUartInfo.RecdataCount<5)//�յ�������̫�٣���һ��recdatacountֱ��return
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
				len = sword.word;//�����len��ʾ�յ������ݳ��ȣ�2byte�����ݱ���϶��ܳ��ġ�
				SUartInfo.RecData[0]=0xAA;
				BootCmd_Process(cmd,len);//����len������32bit
		//��0x00 00 xx xx�������ʱ������ת����0x xx xx��ֻȡ��16bit
			}
		}
	}
}





