# ucos程序分析

## 关中断

```c
/*关闭中断   BSP_IntEn使能中断 后面初始化外设的收会开启对应的 中断*/
CPU_IntDis()；
```



## 初始化os

```c
/*初始化 ucos 系统 */
OSInit
```

## os 中断指向

```c
/*初始化中断向量 65个中断向量 全部指向空函数  BSP_IntVectSet( int_id, BSP_IntHandlerDummy );*/
BSP_IntInit();
```

## 板子资源初始化

```c
//板子资源初始化
Board_Init();
	/*
	 初始化rtc
	 io初始化
	 
	 can初始化
	 定时器初始化--并没有直接开

	*/
```

### adc初始化

```c
typedef struct		adc结构体
{
	ADC_TypeDef*	ADCx;		  //哪个控制器，一个adc有多个采集通道
	uint32_t 		RCC_APB_ADCx; //哪个总线
	uint8_t 		ADCDMAFUN;	  //dma
	uint8_t			ADC_Chnum;     //要初始化的adc通道 数量
	Bsp_ADCCH		ADC_Port[16];  //port pin 管脚在哪个总线  有多个pin
	uint8_t			ADC_Ch[16];	   //通道			每个pin对应1个通道
}Bsp_ADCSt;

//adc初始化--给 Bsp_ADCSt 变量赋值后  一次性初始化整个adc结构体--就将所有的adc通道都初始化了（每个通道的pin初始化1次，adc外设初始化1次）
ADC1ST.ADCx = ADC1;		//一个adc有多个通道可以同时用
ADC1ST.RCC_APB_ADCx= RCC_APB2Periph_ADC1;//外设的总线
ADC1ST.ADC_Chnum= 4; //要初始化的通道个数

ADC1ST.ADC_Port[0].GPIO_Port = GPIOC;
ADC1ST.ADC_Port[0].GPIO_Pin = GPIO_Pin_2;
ADC1ST.ADC_Port[0].RCC_APB_PORTx= RCC_APB2Periph_GPIOC;//通道所在总线
ADC1ST.ADC_Ch[0]  = ADC_Channel_12;//哪个通道

ADC1ST.ADC_Port[1].GPIO_Port = GPIOC;
ADC1ST.ADC_Port[1].GPIO_Pin = GPIO_Pin_3;
ADC1ST.ADC_Port[1].RCC_APB_PORTx= RCC_APB2Periph_GPIOC;
ADC1ST.ADC_Ch[1]  = ADC_Channel_13;

ADC1ST.ADC_Port[2].GPIO_Port = GPIOC;
ADC1ST.ADC_Port[2].GPIO_Pin = GPIO_Pin_4;
ADC1ST.ADC_Port[2].RCC_APB_PORTx= RCC_APB2Periph_GPIOC;
ADC1ST.ADC_Ch[2]  = ADC_Channel_14;

ADC1ST.ADC_Port[3].GPIO_Port = GPIOC;
ADC1ST.ADC_Port[3].GPIO_Pin = GPIO_Pin_5;
ADC1ST.ADC_Port[3].RCC_APB_PORTx= RCC_APB2Periph_GPIOC;
ADC1ST.ADC_Ch[3]  = ADC_Channel_15;

//adc外设初始化   各个pin-通道的初始化
void BSP_ADC_Int(Bsp_ADCSt ADCST)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	ADC_InitTypeDef	ADC_InitStructure;
	uint8_t i= 0;
	ADC_DeInit(ADCST.ADCx);
	RCC_APB2PeriphClockCmd(ADCST.RCC_APB_ADCx, ENABLE);
	for(i= 0;i<ADCST.ADC_Chnum;i++)//一共要初始化几个通道
	{
		RCC_APB2PeriphClockCmd(ADCST.ADC_Port[i].RCC_APB_PORTx, ENABLE);
		GPIO_InitStructure.GPIO_Pin = ADCST.ADC_Port[i].GPIO_Pin;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
		GPIO_Init(ADCST.ADC_Port[i].GPIO_Port, &GPIO_InitStructure);
	}
		// ADC1 configuration -adc外设的初始化----/
		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//ADC工作模式独立模式
		ADC_InitStructure.ADC_ScanConvMode = DISABLE;//多通道或者单通道模式
		ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//连续还是单次模式
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//转换由软件触发
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//左对齐还是右对齐
		ADC_InitStructure.ADC_NbrOfChannel = 1;//规则转换通道数目
		ADC_Init(ADCST.ADCx, &ADC_InitStructure);
		//配置ADC时钟
		RCC_ADCCLKConfig(RCC_PCLK2_Div8);
		// Enable ADC1
		ADC_Cmd(ADCST.ADCx, ENABLE);//
		// Enable ADC1 reset calibaration register
		ADC_ResetCalibration(ADCST.ADCx);//重置校准寄存器
		// Check the end of ADC1 reset calibration register
		while(ADC_GetResetCalibrationStatus(ADCST.ADCx));//获取重置校准寄存器状态
		
		// Start ADC1 calibaration
		ADC_StartCalibration(ADCST.ADCx);//开始校准状态
		// Check the end of ADC1 calibration
		while(ADC_GetCalibrationStatus(ADCST.ADCx));//获取ADC校准程序
}
```

### can初始化

```c
//多个can初始化
CAN1_GPIO_Config();//io
CAN_SetBps(CAN1,CAN1_BPS);//波特率
CAN_SetFilterExt(0,0,0,CAN_FIFO0);/*设置过滤，允许接收所有帧*/
CAN_ITConfig( CAN1, CAN_IT_FMP0, ENABLE );//开不开中断--开的哪一类（没说触发中断走哪里）

//os系统自带多个中断向量 每个分一个id  整数BSP_INT_ID_USB_LP_CAN_RX0就是其中一个
/*
  开始初始化时指向了 空函数  ，现在让他指向 写好的中断处理函数 （正常的中断函数是通过下面设置的）
  NVIC_SetPriority(USART2_IRQn, 0);
  NVIC_EnableIRQ(USART2_IRQn);
*/
BSP_IntVectSet( BSP_INT_ID_USB_LP_CAN_RX0, STM_Can1_RX_ISR );/*注册os的中断函数*/
BSP_IntEn( BSP_INT_ID_USB_LP_CAN_RX0 );//使能os的此中断 指向


CAN2_GPIO_Config();
CAN_SetBps(CAN2,CAN2_BPS);
CAN_SetFilterExt(14,0,0,CAN_FIFO0); /*设置过滤，允许接收所有帧*/
CAN_ITConfig( CAN2, CAN_IT_FMP0, ENABLE );
BSP_IntVectSet( BSP_INT_ID_CAN2_RX0 , STM_Can2_RX_ISR );/*注册中断函数*/
BSP_IntEn( BSP_INT_ID_CAN2_RX0 );

//
```

can的中断处理函数写法

```c
void STM_Can1_RX_ISR( void )
{
     CanRxMsg   RxMessage;
     CAN_Receive( CAN1, CAN_FIFO0, &RxMessage );
     CAN1_ISR_Code(&RxMessage );/*中断代码  CAN1_ISR_Code 是什么鬼
     			其实这里直接写 App_Can1_RX_ISR（）也可以，只是个函数地址罢了*/
}
//CAN1_ISR_Code是个函数指针，指向 App_Can1_RX_ISR  实际的数据处理在 指向的函数里面

```



### `os `给IT函数中函数指针赋值

```c
//这一步不要也行--只是为了让 代码分层 基础-应用层------
/*注册应用层中断处理函数*/
Set_Can1_RX_ISR(App_Can1_RX_ISR);//CAN1_ISR_Code=App_Can1_RX_ISR
Set_Can2_RX_ISR(App_Can2_RX_ISR);//CAN2_ISR_Code=App_Can2_RX_ISR
Set_TIM2_ISR(APP_TIM2_ISR);  //TIM2_ISR_Code=APP_TIM2_ISR
Set_UART2_RxISR( App_UART2_RX_ISR );//UART2_Rx_ISR_Code=App_UART2_RX_ISR
```

### 总结

```c
每次用系统的时候 需要指定单片机类型
    系统会根据类型，知道单片机的中断向量分别在什么位置
    触发响应 can-uart中断的时候会走到各自的中断处理函数去（用cubemx就可以生成单片机与os对应的程序）
    
```





## 几个任务编写

### `App_TaskProc`

```c

```



### `App_TaskIntCan`

### `App_TaskModelBus`



