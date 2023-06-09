# 休眠唤醒

## 进休眠之前

在main中先初始化相应的外设，在while（1）中定时去休眠，调用SleepCheck休眠检测函数。

满足休眠条件时先清之前的唤醒标志位-（这些标志位在上次在唤醒中断中置1后其他代码中就没动过，现在需要清0）

![image-20211026180636349](C:\Users\X1 YOGA\AppData\Roaming\Typora\typora-user-images\image-20211026180636349.png)

在Enter_StopMode函数中要先关闭systick中断，关闭需要休眠的外设，使能可以唤醒的中断gpio引脚，使能rtc中断。--这样只有gpio的中断与rtc才可以将板子唤醒。

```c
LL_SYSTICK_DisableIT();
Sleep_PinInit()
{
	LL_TIM_DeInit(TIM16);//将内部的timer，adc，iic，uart都关闭了
	LL_TIM_DisableCounter(TIM16);
	LL_ADC_DeInit(ADC1);
	LL_USART_Disable(USART2);

	LL_GPIO_ResetOutputPin(TVCC_GPIO_Port, TVCC_Pin);//将需要关闭的io给关了
	LL_GPIO_ResetOutputPin(POWER_EN_GPIO_Port, POWER_EN_Pin);

	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);//可以删除的代码
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC);
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
    
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;//测试一下，感觉配成输入就可以，主要就是降低功耗
	GPIO_InitStruct.Pin = LL_GPIO_PIN_ALL;
	
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	LL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);//将所有的io初始化为

	MX_EXTI_Init()//使能可以中断的io口
    {
        LL_EXTI_SetEXTISource();
        LL_GPIO_SetPinMode(GPIOC,LL_GPIO_PIN_15,LL_GPIO_MODE_INPUT);
        LL_EXTI_Init(&EXTI_InitStruct);//enable中断line
        NVIC_SetPriority(EXTI0_1_IRQn, 0);//使能中断
		NVIC_EnableIRQ(EXTI0_1_IRQn);
    }
}
SetWakeUpTimer_IT( 60, LL_RTC_WAKEUPCLOCK_CKSPRE);//开启rtc中断
```

## 被唤醒之后

```c
	//先要关闭之前的那些io中断与rtc中断，应该禁止掉这些中断，这些中断只有进入休眠之前才会被用到
	GPIO_CloseWEAK_EXTI();
	//使能外部时钟
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);//休眠里面把板子上的外设时钟都给关了。
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
	/* 按照main刚开始的流程再初始化一遍 */
	SystemClock_Config();
	NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); 
	LL_SYSTICK_EnableIT();
	MX_GPIO_Init();
   //MX_ADC1_Init();//自己生成的adc第二次初始化有问题，会卡死
    MX_I2C2_Init();
    MX_RTC_Init();
    MX_TIM16_Init();
    MX_USART2_UART_Init();
```

# bootloader

## boot程序的修改

> 1. 修改flash大小：不同的片内flash不一样
> 2. 修改FIRMWARE_MAXSIZE大小---boot_flash.c,这个需要计算一下是不是bin文件大小符合要求
> 3. 初始化power_en与相关的外设，即uart它只是用来刷程序用的，波特率要跟上位机一致。

## app程序的修改

> 1. 修改flash大小
> 2. 给main开头的绝对地址写入数据，代码头
> 3. system_stm32g0xx.c文件设置中断偏移量VECT_TAB_OFFSET改成1900--程序开头地址必须是100的倍数存放NVIC...
> 4. #define VECT_TAB_OFFSET  0x1900U  这个一定要改，中断偏移

# sh367309

## iic

```c
同事的iic只需要在GPIO初始化的时候直接初始化对应的IIC的pin脚即可。
后面的函数都是自己根据电平模拟出来的。
```

## mos温度

```c
cubemx里面adc初始化的函数会卡死，参考之前可以正常运行的代码发现，那部分会卡死的代码被删除了。可见直接删除并不会影响程序。
```

![image-20211027185938393](C:\Users\X1 YOGA\AppData\Roaming\Typora\typora-user-images\image-20211027185938393.png)

## iic采集电压部分

```c
刚开始测出的电压值不对，因为电池组负极的开关断开了。
 负极断开则cpu的负极就飘了，所以测不准
如果只将正极的开关断开只会有一节电压不准。
```

