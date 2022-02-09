void EEInit(void)
{
	EE24CXX_IICPort_Init();//初始化iic所在的port组的时钟与iic pin脚配置
	if(0 != EE_ReadPara())
	{
//		//       AA__printf("EEP_Init_Err");
		EEParaSetDefault();
		EE_WritePara();
	}
}
void EEParaSetDefault(void)设置好eeLogPara中部分参数的值就是默认初始化eeprom
{
	eeLogPara.data.LogNum= 0;
	eeLogPara.data.SaveAddr= 0;
	eeLogPara.data.FistUseData = FistUseFlag ;
}


void EE24CXX_IICPort_Init(void)
{
	__IO uint32_t tmpreg; 
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	SET_BIT(RCC->IOPENR, RCC_IOPENR_GPIOBEN); //使能iic所在的port的时钟
	/* Delay after an RCC peripheral clock enabling */ 
	tmpreg = READ_BIT(RCC->IOPENR, RCC_IOPENR_GPIOBEN); 
	(void)tmpreg; 

	//EEPROM两个pin都是OD输出
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	GPIO_InitStruct.Pin = CFG_EE_IICSCL_Pin;
	HAL_GPIO_Init(CFG_EE_IICSCL_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = CFG_EE_IICSDA_Pin;
	HAL_GPIO_Init(CFG_EE_IICSDA_Port, &GPIO_InitStruct);

	EE24CXX_I2C_Stop();
	EE24CXX_I2C_Start();		/* 发送启动信号 */
}

void Soft_Delay(uint16_t time)
{    
   uint16_t t=time;  
   while(t>0)
   {
   	t--;
   }
}

EEDLY=16

void EE24CXX_I2C_Stop(void)
{
	/* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
	EE24CXX_I2C_SDA(0);
	EE24CXX_I2C_SCL(1);
	Soft_Delay(EEDLY);//延时几微等电平稳定，稳定后拉高sda即是stop信号触发
	EE24CXX_I2C_SDA(1);
}

void EE24CXX_I2C_Start(void)
{
	/* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
	EE24CXX_I2C_SDA(1);
	EE24CXX_I2C_SCL(1);
	Soft_Delay(EEDLY);
	EE24CXX_I2C_SDA(0);
	Soft_Delay(EEDLY);//延时几微秒等电平稳定
	EE24CXX_I2C_SCL(0);//start信后发送后拉低scl则sda可以准备数据了，不然scl处于高电平不能准备数据。
	Soft_Delay(EEDLY);
}

int EE_ReadPara()//读一下eeprom中的数据看一下里面存的数据是不是有效数据，校验通过就是有效数据
{
	EE24CXX_Read(EEADDR_Info, eeLogPara.buff,sizeof(EEParaSt));
	if(eeLogPara.data.FistUseData!= FistUseFlag)
	{
		return -1;
	}
	return (eeLogPara.data.numCRC== LOG_crc(eeLogPara.buff,sizeof(EEParaSt)-2))?0:-1;
}
//1byte的read
uint8_t EE24CXX_I2C_ReadByte(void)
{
	uint8_t i;
	uint8_t value=0;

	/* 读到第1个bit为数据的bit7 */
	for (i = 0; i < 8; i++)
	{
		value <<= 1;	//先存的是高bit的数据，存1个左移1个
		EE24CXX_I2C_SCL(1);//拉高scl将刚刚存的sda的数据读进来
		Soft_Delay(EEDLY);
		if (EE24CXX_READ_SDA)//根据sda电平，给val的最低bit赋值
		{
			value++;
		}
		EE24CXX_I2C_SCL(0);//拉低scl准备要发送的数据
		Soft_Delay(EEDLY);
	}
	return value;//返回读到的1byte的数据
}

//下面的random_read函数可以完全根据从机手册里面的时序去实现，但是首先我们需要实现的是1byte的read
void EE24CXX_Read(uint16_t ReadAddr,uint8_t* buf,uint16_t len)//eeprom的读地址，读到哪里，读多少
{				 
	uint16_t i = 0;	
	S_HALFWORD addr;
	addr.halfword = ReadAddr;
		  	    																 
  	EE24CXX_I2C_Start();
	EE24CXX_I2C_SendByte(0xA0) ;   //发送器件地址0XA0,写数据 。读之前要先写	 
	EE24CXX_I2C_WaitAck();
  	EE24CXX_I2C_SendByte(addr.bytes.byte_h); //发送内部reg的高地址
	EE24CXX_I2C_WaitAck();   
	EE24CXX_I2C_SendByte(addr.bytes.byte_l); //发送内部reg的低地址
	EE24CXX_I2C_WaitAck();	
	EE24CXX_I2C_Start();  	//再次start开始读	 	   
	EE24CXX_I2C_SendByte(0xA1);   //进入接收模试，不需要再次发内部reg地址了，只发送器件地址即可		   
	EE24CXX_I2C_WaitAck(); 
	for (i = 0; i < len; i++)
	{
		*buf++ = EE24CXX_I2C_ReadByte();		
		if (i != len - 1)
		{
			EE24CXX_I2C_Ack();	收到的不是最后1byte数据则从机会继续给我们发数据
		}
		else
		{
			EE24CXX_I2C_NAck();	收到最后1byte数据发nack表示不读了，从机就不发了
		}
	}
	EE24CXX_I2C_Stop();//产生一个停止条件
}

uint8_t EE24CXX_I2C_WaitAck(void)//占一个时钟周期
{
	uint8_t re;

	EE24CXX_I2C_SDA(1);	/* CPU释放SDA总线 */
	Soft_Delay(EEDLY);
	EE24CXX_I2C_SCL(1);	/* CPU驱动SCL(1), 此时器件会返回ACK应答 */
	Soft_Delay(EEDLY);
	re = EE24CXX_READ_SDA;	/* CPU读取SDA口线状态 */
	EE24CXX_I2C_SCL(0);
	Soft_Delay(EEDLY);
	return re;
}

void EE24CXX_I2C_Ack(void)
{
	EE24CXX_I2C_SDA(0);	/* CPU驱动SDA = 0 */
	Soft_Delay(EEDLY);
	EE24CXX_I2C_SCL(1);	/* CPU产生1个时钟 */
	Soft_Delay(EEDLY);
	EE24CXX_I2C_SCL(0);
	Soft_Delay(EEDLY);
	EE24CXX_I2C_SDA(1);	/* CPU释放SDA总线 */
}

void EE24CXX_I2C_NAck(void)
{
	EE24CXX_I2C_SDA(1);	/* CPU驱动SDA(1) */
	Soft_Delay(EEDLY);
	EE24CXX_I2C_SCL(1);	/* CPU产生1个时钟 */
	Soft_Delay(EEDLY);
	EE24CXX_I2C_SCL(0);
	Soft_Delay(EEDLY);	
}

void EE24CXX_I2C_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	/* 先发送字节的高位bit7 */
	for (i=0; i < 8; i++)
	{		
		if (_ucByte & 0x80)	//看sda线应该拉高还是拉低
		{
			EE24CXX_I2C_SDA(1);
		}
		else
		{
			EE24CXX_I2C_SDA(0);
		}
		Soft_Delay(EEDLY);//等待sda数据稳定
		EE24CXX_I2C_SCL(1);//发sda的数据
		Soft_Delay(EEDLY);	
		EE24CXX_I2C_SCL(0);//拉低scl又来准备数据
		if (i == 7)//如果8bit都发完了则释放sda，拉高就是释放sda
		{
			 EE24CXX_I2C_SDA(1); // 释放总线
		}
		_ucByte <<= 1;	/* 左移一个bit */
		Soft_Delay(EEDLY);
	}
}
//写num byte的函数看器件手册---512k的eeprom支持最大128byte的写，其他的只支持最大64byte的写
void EEProm_Write(uint16_t Addr,uint8_t* Buf,uint16_t num)
{
	uint16_t i = 0;
	uint8_t div = num/128;
	uint8_t over = num%128;
	
	if(num>=128)//例如258个就分3次写，addr+0写128byte addr+128写128byte addr+256写overbyte
	{
		for(i=0;i<div;i++)
		{
			EE24CXX_WriteBytes(i*128+Addr, Buf+i*128,128);
			Soft_Delay(PAGEDLY);
		}
		if(over > 0)
		{
			EE24CXX_WriteBytes(i*128+Addr, Buf+i*128,over);
			Soft_Delay(PAGEDLY);
		}
	}
	else
	{
		EE24CXX_WriteBytes(Addr, Buf,num);
		Soft_Delay(PAGEDLY);
	}
}
//写len byte的函数
void EE24CXX_WriteBytes(uint16_t WriteAddr,uint8_t* buf,uint16_t len)
{
	S_HALFWORD data;
	if(len>128)return;
	data.halfword = WriteAddr;
	EE24CXX_I2C_Start();  	
	EE24CXX_I2C_SendByte(0xA0);   //发送器件地址
	EE24CXX_I2C_WaitAck();
	EE24CXX_I2C_SendByte(data.bytes.byte_h);  
	EE24CXX_I2C_WaitAck();
	EE24CXX_I2C_SendByte(data.bytes.byte_l); 
	EE24CXX_I2C_WaitAck();
	while(len--)
	{
		EE24CXX_I2C_SendByte(*buf++);  
		EE24CXX_I2C_WaitAck();
	}

	EE24CXX_I2C_Stop();//产生一个停止条件 	  
}

void SaveLogProcess(void)
{
	uint8_t i = 0;
	int16_t currValue=0;
	eeLogData.data.year= date_Now.Year;	//将需要存到eeprom中的数据都存到变量eeLogData中然后写道eeprom中
	eeLogData.data.month= date_Now.Month; 	月
	eeLogData.data.date= date_Now.Month;	日
	eeLogData.data.hour= time_Now.Hours;	
	eeLogData.data.mintus= time_Now.Minutes;	
	eeLogData.data.secound= time_Now.Seconds;
	for(i= 0;i<16;i++)
	{
		eeLogData.data.cellVolt[i]=syspara.cellVolt[i];		所有电压
	}
	currValue= (int16_t)(((int32_t) syspara.current)/10);	
	eeLogData.data.corrent= currValue;					电流
	eeLogData.data.soc= syspara.SOC;					soc 
	eeLogData.data.cellTemp[0]=syspara.cellTemp[0];		温度
	eeLogData.data.cellTemp[1]=syspara.cellTemp[1];
	eeLogData.data.cellTemp[2]=syspara.cellTemp[2];
	eeLogData.data.cellTemp[3]=syspara.cellTemp[3];
	eeLogData.data.Alarm=  syspara.alarmByte;			alarm报警
	eeLogData.data.mostemp= syspara.mosTemp;			mos温度
	eeLogData.data.mosvalue=(((syspara.chgMos<<0)|(syspara.dsgMos<<1))&0x03);		充放电mos状态
	eeLogData.data.CapSesign= (CFG_DESIGN_CAPACITY/100);		标定容量
	eeLogData.data.sys_CHG= flashpara.cyc_CHG;			循环次数

	EE_WriteLOG();
}

void EE_WriteLOG(void)
{
	uint16_t  S_StartAddr = eeLogPara.data.SaveAddr + EEADDR_LOG ;	存数据的起始地址=之前的地址+0x40因为每次存的数据大小都是54 这里一下存64
	EEProm_Write(S_StartAddr,eeLogData.buff , EE_LOGsize) ;//一次写EE_LOGsize个byte的eeLogData数据
	eeLogPara.data.LogNum ++ ;						//lognum+1
	eeLogPara.data.SaveAddr +=  EE_LOGsize ;		//存log的地址+=  EE_LOGsize
	eeLogPara.data.SaveAddr %= (EE_LOGsize*EE_LOGMax) ;		//总数据条数不超过1000=EE_LOGMax
	EE_WritePara();
}
void EE_WritePara(void)
{
	eeLogPara.data.numCRC = LOG_crc(eeLogPara.buff,sizeof(EEParaSt)-2);
	EEProm_Write(EEADDR_Info,eeLogPara.buff,sizeof(EEParaSt));//将eeLogPara中的数据都存到eeprom中
}