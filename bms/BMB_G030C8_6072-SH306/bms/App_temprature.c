#include "main.h"

const uint16_t Pos5vNTC3380AdTempTABLE[] =
{
	208   ,
	219   ,
	230   ,
	242   ,
	254   ,
	267   ,
	280   ,
	294   ,
	308   ,
	323   ,
	339   ,
	355   ,
	371   ,
	388   ,
	406   ,
	425   ,
	444   ,
	464   ,
	484   ,
	505   ,
	527   ,
	549   ,
	572   ,
	596   ,
	620   ,
	646   ,
	671   ,
	698   ,
	725   ,
	753   ,
	781   ,
	810   ,
	840   ,
	870   ,
	901   ,
	932   ,
	964   ,
	997   ,
	1030  ,
	1064  ,
	1098  ,
	1133  ,
	1168  ,
	1204  ,
	1240  ,
	1277  ,
	1314  ,
	1351  ,
	1389  ,
	1426  ,
	1465  ,
	1503  ,
	1542  ,
	1580  ,
	1619  ,
	1658  ,
	1697  ,
	1736  ,
	1775  ,
	1815  ,
	1854  ,
	1893  ,
	1932  ,
	1970  ,
	2009  ,
	2048  ,
	2086  ,
	2124  ,
	2162  ,
	2199  ,
	2236  ,
	2273  ,
	2310  ,
	2346  ,
	2381  ,
	2417  ,
	2452  ,
	2486  ,
	2520  ,
	2554  ,
	2587  ,
	2620  ,
	2652  ,
	2684  ,
	2715  ,
	2746  ,
	2776  ,
	2806  ,
	2835  ,
	2864  ,
	2892  ,
	2920  ,
	2947  ,
	2973  ,
	2999  ,
	3025  ,
	3050  ,
	3074  ,
	3098  ,
	3122  ,
	3145  ,
	3167  ,
	3190  ,
	3211  ,
	3232  ,
	3253  ,
	3273  ,
	3292  ,
	3312  ,
	3330  ,
	3349  ,
	3367  ,
	3384  ,
	3401  ,
	3418  ,
	3434  ,
	3450  ,
	3465  ,
	3480  ,
	3495  ,
	3510  ,
	3524  ,
	3537  ,
	3550  ,
	3563  ,
	3576  ,
	3588  ,
	3600  ,
	3612  ,
	3624  ,
	3635  ,
	3646  ,
	3656  ,
	3667  ,
	3677  ,
	3686  ,
	3696  ,
	3705  ,
	3714  ,
	3723  ,
	3732  ,
	3740  ,
	3748  ,
	3756  ,
	3764  ,
	3772  ,
	3779  ,
	3786  ,
	3793  ,
	3800  ,
	3807  ,
	3813  ,
	3819  ,
	3826  ,
	3832  ,
	3837  ,
	3843  ,
	3849  ,
	3854  ,
	3859  ,
	3865
};

int tbTempCnt = (sizeof(Pos5vNTC3380AdTempTABLE)/sizeof(Pos5vNTC3380AdTempTABLE[0]));

uint16_t Find_Adc_Temp( uint16_t val )
{
	uint8_t j = 0;
	uint16_t tShort = 0;
	uint16_t Tempdata = 0;
	tShort= 4096-val;
	/*通过AD值，查找温度*/
	
	for( j = 0;   j < tbTempCnt; j++ )
	{
		if( tShort < Pos5vNTC3380AdTempTABLE[j] )
		{
			break;
		}
	}
	if( j == 0 || j > tbTempCnt ) /*判断探头未接入//AD采集的电压为高表示空探头*/
	{
		Tempdata = 0xfe ; /*表示探头故障*/
	}
	else
	{
		Tempdata = j ; /*正常的温度采样值*/
	}
	
	return Tempdata;//
}

void Update_CellTemps(void)
{
	uint16_t Value_Tad= 0;
	uint16_t Temp[6] = {0};
	TVCC_1;	//检测温度之前给电压,才可以开启检测
	LL_mDelay(10);
	ADC_Enable();
	ADC_SelecteChannel(DEF_CH_Tmos);
	Value_Tad=ADC_GetValue( );
	syspara.mosTemp = (Find_Adc_Temp(Value_Tad));
	ADC_Disable();
	TVCC_0;//检测完之后关TVCC,不然会耗电
	SH_GetCellTemp(Temp);

	syspara.cellTemp[0]  = (Find_Adc_Temp(Temp[1]));

	syspara.cellTemp[1]  = (Find_Adc_Temp(Temp[3]));
#if (PROJECTCODE==BLX_7220)
	syspara.balTemp  =       (Find_Adc_Temp(Temp[5]));
#endif

#if(DBG_Printf_Temp==1)
	printf("Update_CellTemps: T1= %d T2= %d Tmos= %d TBal= %d ",syspara.cellTemp[0] ,syspara.cellTemp[1] ,syspara.mosTemp,syspara.balTemp);
#endif	
	syspara.tempMax = (syspara.cellTemp[0]>syspara.cellTemp[1]?syspara.cellTemp[0]:syspara.cellTemp[1]);
	syspara.tempMin =(syspara.cellTemp[0]<syspara.cellTemp[1]?syspara.cellTemp[0]:syspara.cellTemp[1]);
	syspara.tempAvg = (syspara.cellTemp[0]+syspara.cellTemp[1])/2;
	

#if(DBG_Printf_Temp==1)
	printf(" Tmax= %d Tmin= %d Tav= %d \r\n",syspara.tempMax ,syspara.tempMin ,syspara.tempAvg);
#endif		
}

void Up_CellTemps(void)
{
//	uint16_t Value_Tad= 0;

	syspara.mosTemp = 60;
	syspara.cellTemp[0]  = 61;
	syspara.cellTemp[1]  = 62;
	
	//printf("Update_CellTemps: T1= %d T2= %d Tmos= %d ",syspara.cellTemp[0] ,syspara.cellTemp[1] ,syspara.mosTemp);
	
	syspara.tempMax = (syspara.cellTemp[0]>syspara.cellTemp[1]?syspara.cellTemp[0]:syspara.cellTemp[1]);
	syspara.tempMin = (syspara.cellTemp[0]<syspara.cellTemp[1]?syspara.cellTemp[0]:syspara.cellTemp[1]);
	syspara.tempAvg = (syspara.cellTemp[0]+syspara.cellTemp[1])/2;
	
	//printf(" Tmax= %d Tmin= %d Tav= %d \r\n",syspara.tempMax ,syspara.tempMin ,syspara.tempAvg);
}



