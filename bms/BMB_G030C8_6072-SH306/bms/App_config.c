#include "main.h"

uint8_t BitCount( uint32_t Value )
{
	uint8_t Count = 0;
	while( Value != 0 )
	{
		Value &= ( Value - 1 );
		Count++;
	}
	return Count;
}

void SysParaInit(void)
{
	memset(&syspara,0,sizeof(SysPara));
	userAlarm.bytes= 0;
	sysflag.bytes = 0;
	syspara.IntegralPower= 1050;
	syspara.IntegralPower_C=1000;
	syspara.IntegralPowerjisuan=1050;
}






