## 对enum的学习 struct中的：占位

1. enum ASN{MON=7, TUE, WED=10, THU, FRI};  

2. ASN是个标识符，是一种数据类型（可选项-可有可无）

*enum DAY
{
   MON=1, TUE, WED, THU, FRI, SAT, SUN
};*

enum DAY good_day, bad_day; //变量good_day和bad_day的类型均为枚举型enum DAY

---

enum    //跟第一个定义不同的是，此处的标号DAY省略，这是允许的。
{
    saturday,
    sunday = 0,
    monday,
    tuesday,
    wednesday,
    thursday,
    friday
} workday; //变量workday的类型为枚举型enum DAY，这种无法再定义此类型的变量

---

 typedef enum用法

  typedef enum workday（此处workday是数据类型，通过它可以定义变量）
{
    saturday,
    sunday = 0,
    monday,
    tuesday,
    wednesday,
    thursday,
    friday
};

workday today, tomorrow; //变量today和tomorrow的类型为枚举型workday，即enum workday

-----

*** 对于创建的变量赋值的时候只能赋值括号中的那些数值，默认从0开始。 也可以每个数据都赋别的数值，不然enum就会默认加1.





-----struct中的”：“什么意思

https://blog.csdn.net/q2519008/article/details/81478701

位域：位域出现的原因是由于某些信息的存储表示只需要几个bit位就可以表示而不需要一个完整的字节，同时也是为了节省存储空间和方便处理。

```c
typedef struct  bit_struct

{
    int  bit1:3;

    int  bit2:5;

    int  bit3:7;

}data;
```

bit1、bit2、bit3表示对应的位域

整个位域结构体占用2个字节

bit1占3位，bit2占5位，bit1和bit2共用一个字节

bit3占7位，独占一个字节。

位域必须存储在同一个类型中，不能跨类型，同时也说明位域的长度不会超过所定义类型的长度

----

ringBuffer_t var= {0,0,{0},0,0,100};  对结构体中有数组的变量可以直接在定义的时候初始化。

### 多层while与if嵌套

```c
在多重while嵌套结构中使用break，会退出距离break最近的那一层while循环，且多从if嵌套对break无约束作用，会跳出最外层if，寻找与之最近的最内层while跳出
   exp:https://blog.csdn.net/oykotxuyang/article/details/105876206
```

---

串口初始化流程：

```c
{
	定义LL_USART_InitTypeDef结构体变量；
	定义LL_GPIO_InitTypeDef结构体变量；
        
    使能uart1/2时钟，这个是外设时钟；
    使能具体gpio时钟，uart1/2用的那个管脚
        
    LL_GPIO_InitTypeDef.成员={pin,模式（输入、输出、复用）,速度,
                            输出类型（推挽、开漏）--串口就设置推挽,
                           Pull(no,pull up,pull down)--一般硬件管理？我们设置no即可，
                           Alternate(LL_GPIO_AF_1)--复用哪个功能，有iic，uart，mco，time查datesheet可知}
    配置完之后初始化一下此配置的pin,则当前变量没用了，对下一个pin初始化。
        
    配置中断优先级并且使能中断。
    
    对LL_USART_InitTypeDef结构体变量的初始化。
    PrescalerValue，波特率，datewidth，stopbit，Parity，TransferDirection，HardwareFlowControl，OverSampling
    配置完之后init这个变量
        
    LL_USART_ConfigAsyncMode(USART1); --异步
	LL_USART_EnableIT_RXNE(USART1); --？
	LL_USART_EnableIT_IDLE(USART1); --？

	LL_USART_Enable(USART1);--最后使能串口1
}


```



---

LL库与hal库怎么用的，怎么选择

LL库与HAL库类似，hal库体积比较大，ll库体积小。

在cubemx生成的时候，选择用哪个库，生成代码的时候就会选择对应的库。--趋势

---

#### ADC初始化

```c
定义LL_ADC_InitTypeDef与LL_ADC_REG_InitTypeDef结构体变量
定义LL_GPIO_InitTypeDef 变量
初始化外设时钟adc外设时钟，初始化gpio时钟
变量.Pin，.Mode = LL_GPIO_MODE_ANALOG；.Pull = LL_GPIO_PULL_NO
初始化此变量
//初始化LL_ADC_InitTypeDef
	ADC_InitStruct.Clock = LL_ADC_CLOCK_SYNC_PCLK_DIV2;
	ADC_InitStruct.Resolution = LL_ADC_RESOLUTION_12B;
	ADC_InitStruct.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
	ADC_InitStruct.LowPowerMode = LL_ADC_LP_MODE_NONE;
	LL_ADC_Init(ADC1, &ADC_InitStruct);
//初始化LL_ADC_REG_InitTypeDef
	ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
	ADC_REG_InitStruct.SequencerLength = LL_ADC_REG_SEQ_SCAN_DISABLE;
	ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
	ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_SINGLE;
	ADC_REG_InitStruct.DMATransfer = LL_ADC_REG_DMA_TRANSFER_NONE;
	ADC_REG_InitStruct.Overrun = LL_ADC_REG_OVR_DATA_PRESERVED;
	LL_ADC_REG_Init(ADC1, &ADC_REG_InitStruct);

	LL_ADC_SetOverSamplingScope(ADC1, LL_ADC_OVS_DISABLE);
	LL_ADC_SetTriggerFrequencyMode(ADC1, LL_ADC_CLOCK_FREQ_MODE_HIGH);
	LL_ADC_SetSamplingTimeCommonChannels(ADC1, LL_ADC_SAMPLINGTIME_COMMON_1, LL_ADC_SAMPLINGTIME_160CYCLES_5);
	LL_ADC_SetSamplingTimeCommonChannels(ADC1, LL_ADC_SAMPLINGTIME_COMMON_2, LL_ADC_SAMPLINGTIME_160CYCLES_5);
	LL_ADC_EnableInternalRegulator(ADC1);
	
	/* Delay for ADC internal voltage regulator stabilization. */
	ADC_VREFINT_CAL= *(uint16_t*)0x1FFF75AA;
	//ADC_VREFINT_DATA= LL_ADC_GetCalibrationFactor(ADC1);
	//printf("ADC_VREFINT_CAL=%d\r\n",ADC_VREFINT_CAL);
	
	ADC_Calibration();
	uint32_t wait_loop_index;
	wait_loop_index = ((LL_ADC_DELAY_INTERNAL_REGUL_STAB_US * (SystemCoreClock / (100000 * 2))) / 10);
	while(wait_loop_index != 0)
	{
		wait_loop_index--;
	}
	//使能内部基准电压
	LL_ADC_SetCommonClock(ADC1_COMMON,LL_ADC_CLOCK_ASYNC_DIV12);
	LL_ADC_SetCommonPathInternalCh(ADC1_COMMON,LL_ADC_PATH_INTERNAL_VREFINT);
/* Configure Regular Channel
	*/
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, DEF_CH_T1);
  LL_ADC_SetChannelSamplingTime(ADC1, DEF_CH_T1, LL_ADC_SAMPLINGTIME_COMMON_1);

？？？还有问题
```



```c
	LL_TIM_InitTypeDef TIM_InitStruct = {0};

	/* Peripheral clock enable */ 给时钟，因为他没链接pin脚，所以不用给pin脚时钟，定时器是内部的一个部件。
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM16);

	/* TIM16 interrupt Init */
	NVIC_SetPriority(TIM16_IRQn, 0);
	NVIC_EnableIRQ(TIM16_IRQn);   使能中断，使能后中断处理函数在哪里呢？stm32xxx_it.h中由对应的函数定义。
    由于中断产生的时候会将定时器的某个regflag置1，所以进入中断处理函数需要先清除此flag，否则一出中断处理函数就又会进入。

	TIM_InitStruct.Prescaler = 15;  预分频
	TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;  向上计数
	TIM_InitStruct.Autoreload = 500;  计数到500停
	TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;  再分频
	TIM_InitStruct.RepetitionCounter = 0;  
	LL_TIM_Init(TIM16, &TIM_InitStruct);
这里初始化里面没有开定时器的中断，可能在外面是开了的。
    定时器的作用：
    1.计算时间--不开中断，不连接pin脚
    	定时器开--干活--定时器关（差值就是时间长度）  注意将reload设置大一些，不然中途自动reload了。
    2.产生中断，开启中断。又对应的中断处理函数--（中断处理函数可以访问全局变量与当前函数的入参）
    3.链接pin脚，产生固定频率的脉冲
```



---

#### rtc初始化

```c
	LL_RTC_TimeTypeDef sTime;	//定义stime
	LL_RTC_DateTypeDef sDate;	//定义stime
	LL_RTC_InitTypeDef RTC_InitStruct = {0};
	SET_BIT(PWR->CR1, PWR_CR1_DBP);
	/* Peripheral clock enable */
	LL_RCC_EnableRTC();
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_RTC);
	/* RTC interrupt Init */
	NVIC_SetPriority(RTC_TAMP_IRQn, 2);
	NVIC_EnableIRQ(RTC_TAMP_IRQn);

	RTC_InitStruct.HourFormat = LL_RTC_HOURFORMAT_24HOUR;
	RTC_InitStruct.AsynchPrescaler = 127;
	RTC_InitStruct.SynchPrescaler = 255;
	LL_RTC_Init(RTC, &RTC_InitStruct);
	/** Enable the WakeUp
	*/
	// LL_RTC_WAKEUP_SetClock(RTC, LL_RTC_WAKEUPCLOCK_DIV_16);

	/* USER CODE BEGIN Check_RTC_BKUP */
	if(initFalg==1)
	{
		LL_RTC_DisableWriteProtection(RTC);
	    if (LL_RTC_IsShadowRegBypassEnabled(RTC) == 0U)
	    {
	        LL_RTC_WaitForSynchro(RTC);
	    }
	    
		LL_RTC_EnableWriteProtection(RTC);
	return ;
	}
	//printf("MX_RTC_ RESET!!!\r\n");
	initFalg=1;

	LL_RTC_TIME_StructInit(&sTime);
	LL_RTC_DATE_StructInit(&sDate);

	/* USER CODE END Check_RTC_BKUP */

	/** Initialize RTC and set the Time and Date
	*/
	sTime.Hours = 14;
	sTime.Minutes = 50;
	sTime.Seconds = 0;
	sTime.TimeFormat= 0;
	if (LL_RTC_TIME_Init(RTC,LL_RTC_FORMAT_BIN, &sTime) != SUCCESS)
	{
	
	}
	sDate.WeekDay = 2;
	sDate.Month = LL_RTC_MONTH_JUNE;
	sDate.Day = 1;
	sDate.Year = 21;

	if (LL_RTC_DATE_Init(RTC,LL_RTC_FORMAT_BIN, &sDate) != SUCCESS)
	{
	}
```





----

```c
sh367xx初始化
SH30x_IICPort_Init(IICNum)； 初始化
    	使能时钟
    	结构体变量赋值
    		

```



---

##### iic本身是主机占据主导地位，由主机发起开始或者结束信号的，主机也可以不结束，马上发起起始信号进行对另一个从机的寻址。

在总线的一次传输过程中，数据串数方向在整个过程中不变方向

![image-20210929160051719](C:\Users\X1 YOGA\AppData\Roaming\Typora\typora-user-images\image-20210929160051719.png)

在传输过程中，当需要改变传递方向时，起始信号与从机地址都被重新发一下，并且改变要读还是要写的那1bit。

![image-20210929161958795](C:\Users\X1 YOGA\AppData\Roaming\Typora\typora-user-images\image-20210929161958795.png)

## stm32的每个管脚都可以开启中断，但是不可以同时开启，因为A1,B1,C1,D1这种序号一样的管脚只有一个管脚可以用来开启，不能同时将这几个管脚开启中断。

```c
IIC作为从机怎么配置使用
#define 每个管脚的时候也需要define出相应管脚的port组。
需要定义的var：
/*---------IIC1---------------*/
uint8_t  Buffer_Rx_IIC1[40];//接收缓存，接收buf
uint8_t  Rx_Idx_IIC1=0;    //接收计数，记下当前读到哪里了
uint8_t  Flag_RcvOK_IIC1 = 0;// 接收完成标志 
uint8_t  Tx_Idx_IIC1=0;    //发送计数，记下当前发到哪里了
u8 Response_Message[40];  //发送缓存，发送buf

RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 使能port组时钟与iic外设时钟
RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

# GPIO配置初始化
GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//必须设置为开漏输出，实现iic的线与逻辑
GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_NOPULL;
//管脚配置中GPIO_InitStructure.GPIO_PuPd必须配置为GPIO_PuPd_NOPULL，若配置为 GPIO_PuPd_UP或GPIO_PuPd_DOWN，IIC总线会一直繁忙，导致总线出错，检测不到IIC从机。

void I2C1_Configuration(void)
{
    I2C_InitTypeDef 定义iicinit结构体;
    NVIC_InitTypeDef 定义nvicinit结构体;

    I2C_DeInit(I2C1);
    .I2C_Mode = I2C_Mode_I2C;
    .I2C_DutyCycle = I2C_DutyCycle_2;
    .I2C_OwnAddress1 = I2C1_Slave_Address; //从机地址，一定要设置正确                          
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress= I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 100000;
    I2C_Init(I2C1, &I2C_InitStructure);  初始化iic结构体
        
    NVIC_InitStructure.NVIC_IRQChannel    = I2C1_EV_IRQn;//事件中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);  初始化iic事件中断

    NVIC_InitStructure.NVIC_IRQChannel = I2C1_ER_IRQn;//错误中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                 
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);   初始化iic的err中断

    I2C_ITConfig(I2C1, I2C_IT_BUF | I2C_IT_EVT |I2C_IT_ERR, ENABLE);   
    I2C_Cmd(I2C1, ENABLE);                                             
}

从机的中断处理程序：
void I2C1_EV_IRQHandler(void)
{
  SR1Register = I2C1->SR1;
  SR2Register = I2C1->SR2;

    /* I2C1是从机(MSL = 0) */
  if((SR2Register &0x0001) != 0x0001)  //判断当前是从机模式
  {
    /* 主机已发送地址，地址为被置位·(ADDR = 1: EV1(包括发送和接收)) 
       从模式下：地址匹配-0：地址不匹配或没有收到地址；1：收到的地址匹配
       表示有主机要与自己通信*/
    if((SR1Register & 0x0002) == 0x0002)
    {
        /* 清除标志位 */
        SR1Register = 0;
        SR2Register = 0;

        Rx_Idx_IIC1=0;
        Tx_Idx_IIC1=0;
    }

    /* 接收数据(RXNE=1: EV2) rxne接收数据reg，0（数据寄存器为空），1（数据寄存器非空）*/
    if((SR1Register & 0x0040) == 0x0040)
    {
        Buffer_Rx_IIC1[Rx_Idx_IIC1++] = I2C1->DR; 把数据reg中的数据放到接收buf
        SR1Register = 0;
        SR2Register = 0;
    }
    /* 检测到停止条件(STOPF =1: EV4)
    0（没有检测到停止条件），1（检测到停止条件*/
    if(( SR1Register & 0x0010) == 0x0010)
    {
        I2C1->CR1 |= 0x0001;（PE-I2C模块使能，定义：0（禁用I2C模块），1（启用I2C模块：根据SMBus位的设置，相应的I/O口需配置为复用功能）
    注：如果清除该位时通讯正在进行，在当前通讯结束后，I2C模块被禁用并返回空闲状态。由于在通讯结束后发生PE＝0，所有的位被清除。
        在主模式下，通讯结束之前，绝不能清除该位。）
        SR1Register = 0;
        SR2Register = 0;
        Flag_RcvOK_IIC1 = 1;            
    }

    /* 发送数据(TxE = 1: EV3、EV3-1) 
    TxE数据寄存器是否为空(发送时)，定义：0（数据寄存器非空）,1（数据寄存器空）-注意：在发送数据时，数据寄存器为空时该位被置’1’，在发送地址阶段不设置该位*/
    if((SR1Register & 0x0080) == 0x0080)
    {
        I2C1->DR = Response_Message[Tx_Idx_IIC1++]; 把发送buf中的数据拷贝到tx发送reg
        SR1Register = 0;
        SR2Register = 0;
    }
    /* 检测到非应答(AF =1: EV3-2) --0（无起始或停止条件出错），1（起始或停止条件出错）*/
    if(( SR1Register & 0x0400) == 0x0400)
    {
        I2C1->SR1 &= 0xFDFF;
        SR1Register = 0;
        SR2Register = 0;        
    }       
  }

}
上述中断程序中，当主机读取和写入数据时，都会引起地址位被置位，即发生EV1事件（即本中断处理程序中将发送和接收的EV1合并了）；
　　当主机写入数据时，中断的执行顺序是EV1—>EV2—>EV4，其中有多个数据EV2会多次执行；
　　当主机读取数据时，中断的执行顺序是EV1—>EV3—>EV3-2，本中断程序中将EV3和EV3-1合并了，若有多个数据，EV3将多次执行。

    
void I2C1_ER_IRQHandler(void) {

  __IO uint32_t SR1Register =0;
  __IO uint32_t SR2Register =0;
  SR1Register = I2C1->SR1;
  SR2Register = I2C1->SR2;

    if(I2C_GetITStatus(I2C1,I2C_IT_SMBALERT)) {
    }
    else if(I2C_GetITStatus(I2C1, I2C_IT_TIMEOUT)) {
    } 
    else if(I2C_GetITStatus(I2C1, I2C_IT_PECERR)) {
    } 
    else if(I2C_GetITStatus(I2C1, I2C_IT_OVR)) {
    }
    else if(I2C_GetITStatus(I2C1, I2C_IT_AF)) {
        I2C_ClearITPendingBit(I2C1, I2C_IT_AF);
    }
    else if(I2C_GetITStatus(I2C1, I2C_IT_ARLO)) {
    }
    else if(I2C_GetITStatus(I2C1, I2C_IT_BERR)) {
    }
        I2C1->CR1 |= 0x0001;
        SR1Register = 0;
        SR2Register = 0;    
}发送各种错误进行错误中断不做对应的处理，最后只进行清除寄存器（SR1和SR2）操作。
```

---

### 几种时钟的区别

```c
rtc定时器是用来计时从1997年开始的时间到现在，这个是个低频的时钟，一般不在系统里面用，只用来显示年月日的。
time定时器：
systick：类似timer，都是有个reload值，向上计数达到此值触发中断然后处理中断。可以通过定义一个全局变量每次出发中断的时候加1，这个数值可以记下来开机到现在的时间长短。timer与systick还有个功能计算任务耗时时长，任务开始时记录全局变量的值，任务结束时记录全局变量的值，相减即可知任务耗时。
    
```

---

### 联合体与结构体大小

```c
union长度怎么计算
    struct的大小计算根据成员的对齐，大小肯定是对齐字节的整数倍。
    union的长度取决于其中的长度最大的那个成员变量的长度。即union中成员变量是重叠摆放的，其开始地址相同。union(共用体)的各个成员是以同一个地址开始存放的,每一个时刻只可以存储一个成员
   union   mm{   
  char   a;//元长度1   
  int   b[5];//元长度4   
  double   c;//元长度8   
  int   d[3]; //元长度4
  };   
本来mm的空间应该是sizeof(int)*5=20;但是如果只是20个单元的话,那可以存几个double型(8位)呢?两个半?当然不可以,所以mm的空间延伸为既要大于20,又要满足其他成员所需空间的整数倍,,因为含有double元长度8，故大小为24。
```

----

#### 很多电池组成的pack再去组，就是电池簇。

----

### 不同型号电池特性

```py
18650
	2.7--3.7--4.2   容量1000mAh以上，目前松下可达到3000mAh
	笔记本用的18650电芯通常2200mAh
磷酸铁锂
	（2-2.5）--3.2--3.65
	
```

