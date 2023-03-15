# BMS知识

## DMA

> DMA支持的外设：timer/ADC/SPI/IIC/UART 管理存储器（包括flash和sram）到存储器，设备到存储器，设备到设备的数据传输

---

## 电池相关知识

> chgdet：充电信号线，充电器一接为1，开始充后又变成0
> det 放电信号线，放电时为1
> idel：中颖的一种休眠模式  此模式下可以充放电唤醒，还有别的模式sleep  xxx只能接充电机唤醒。
> bq76930+bq78350：测电压的可以一节一节校准电压的。
> sh367309：中颖。
>
> 充电口万用表检测电压≤3V。
> 因为充电mos是关闭的，测的电压是0V，并不会直接链接到b+ b-
>
> 低功耗模式：注意在停机或者待机模式下，RTC、IWDG的时钟不会停止。

---

> bootload是一段写在真实程序之前的程序，最终会与真实的程序并到一起（用一个特定的exe程序）
> 在bootload中会关闭中断，所以我们在后面的程序中一上来就需要__enable_irq();打开中断，否则中断都不会触发。

---

> 一线通：
> tx:
> rx:收数据是在中断中收完的
> cpu用了2根线，但是外部通过硬件电路处理成了一根线

---

> 电流的标定与电压的标定不是一回事，电流的标定其实就是直接获取程序中sh367获取的电流值。

### 深度睡眠与普通睡眠的关系

> 深度睡眠只有电芯欠压会触发，只有充电机信号会恢复;
>
> 普通睡眠就是害怕板子耗电，各项指标没问题检测判断一遍就会进休眠，随便啥信号就可以唤醒。
>
> 保护板运行的过程中并不是一直在运行态，运行态会耗电，有的板子运行3m会采集一次参数然后判断如果参数都正常进入休眠状态，然后隔几秒就被唤醒去采集数据如果正常就去继续休眠。如果参数异常就去做相应的操作。

### timer

> 1. 为啥要有定时器：
>    CPU本身也是可以定时的，但是会消耗大量的时间，交给定时器就不用管了，他会帮忙精准定时，每个定时器都有1个16bit的预分频器，4个独立的通道，每个通道可以用于输入捕获、输出比较、pwm与单脉冲模式输出。

> 2. timer的clock—source选择internal_clock
>    timer中断的开启需要在界面配置，timex_update_interrupt通过类似TIM_START_TI(&timex)的函数。
>    假设时钟为32M，如何配置一个500ms的计时器让它每500ms产生一次中断：
>    配置时钟源后还需要配置预分频系数：32 000
>    需要配置counter：500
>    则32000✖500✖1/32000000=500ms 这样配置的timer就是500ms。
>
>    timer启动还需要在主函数初始化time完成后调用相应的启动函数才可以启动：
>    tim_base_start(); 不带中断的tim启动
>    tim_base_start_it(); 带中断的time启动函数。
>
> 3. pwm的配置，选择相应的timer在界面配置pwm-generation-chx，需要在主函数中调用
>    Time_PWM_start();去启动pwm通道 pwm就是配置占空比来调节io口的输出高低电平比例。
>
> 4. RTC的alarmup作用，每次休眠之前调一下定时唤醒的函数，在隔特定时长后就会自己alarm把cpu唤醒。
>    cubemx生成的工程是默认配置systick的，生成的systick隔1s会加1.不需要配置就自动隔1s加1.
>
> 5. systick也是可以配置时钟与reload的，ms/us的延时都可以做到，一般做成ms级别的
>    Systick的中断是默认已经开启了的，直接使用即可，查看NVIC里面的time base:systick timer可以看到默认开中断的
>
> 6. 如何获取板子开机至当前运行时间长度：定义ss=0;500ms计数ss05=0;mm=0;hh=0
>    在timer的500ms的中断处理函数中给上述的几个变量赋值：
>
> ```c
>    handle()
>    {
>    	ss05++;
>    	if(ss05==2)
>    	{
>    		ss05=0;
>    		ss++;
>    		if(ss==60)
>    		{
>    			ss=0;
>    			mm++;
>    			if(mm=60)
>    			{
>    				mm=0;
>    				hh++;
>    			}
>    		}
>    	}
>    }
> ```

### adc

> 技术指标：
> 分辨率：10bit的位数1024，每改变1引起的实际电压的变化就是分辨率，0.00xV
> 量程：模拟信号的变化范围，实际是可以准确采集到的，只不过位数有限，所以才会有误差
> 位数：
> 转换时间：完成一次ad转换需要的时间-包括：采样保持量化编码。
> cubemx里面只需要将io口配置成中断，然后写回调函数即可。中断的触发时刻是adc转换完成会触发。
>
> ---
>
> adc驱动：
> 	开启DMA功能后，读取的最新adc的数据会自动存到自己定义的变量当中去（自己定义的变量就在内存中）
> 	开启DMA功能后DMA需要知道它要从哪里读取数据，所以需要把adc_data_reg的reg地址define出来
> 	stm32的对应的地址区域内都有特定功能对应的寄存器，例如adc1相关reg的区域是0x1111-0x2222等等
> 	一个通道就是一个pin脚，可以采集一个adc数据，就是把此pin作为adc后从datareg可以读到对应的采集的adc数据
> 	先在peripheral register boundary addresses章节找到对应的外设占用的边界地址，
> 	然后去对应外设的register map章节查找对应reg的偏移量确认reg的精确地址。
> 	或者用第一次找到的addr->datareg  这样去找数据reg
> 	
>
> 	dma_init() dma的初始化
> 	{
> 		定义dma初始化struct变量;
> 		dma_deinit()；复位dma通道x,固定用法
> 		将dma的struct变量初始化
> 		调用dma_init初始化刚才初始化的变量
> 		dma_cmd(dma1_channel,enable);
> 	}
> 	gpio_init()  adc的gpio初始化
> 	{
> 		开对应port时钟
> 		使能对应的dmax时钟---dmax也算是外设
> 		使能对应的adcx时钟
> 		设置gpio为模拟输入
> 		设置变量的pin为对应的pin	
> 	}
> 	adc_init()
> 	{
> 		初始化dma与gpio后，定义adc的struct
> 		初始化adc的变量结构体
> 		调用adc_init初始化刚才的var
> 		设置adc控制器x，adc的通道，采样顺序，采样周期。调用库函数
> 		adc_regularChannelConfig();
> 		adc_dmacmd(); 开启adc1的dma支持
> 		adc_resetCalbration(); 重置adc1校准reg
> 		等待校准重置成功
> 		adc_startcalibration(); 开始adc1校准
> 		等待校准完成
> 		adc_softwareStartConvcmd(); 使能adc1开始转换。
> 	}

---

> cubemx直接生成的LL库的adc初始化函数会卡死，如图部分：
>
> ![image-20211027185938393](C:\Users\X1 YOGA\AppData\Roaming\Typora\typora-user-images\image-20211027185938393.png)

> 经过同事测试直接删除没不良影响。

### 结构体

```c
typedef union
{
    uint32_t word;
    struct
    {   
        uint8_t byte_1:8; 0x01  在低字节
        uint8_t byte_2:8; 0x02
        uint8_t byte_3:8; 0x03
        uint8_t byte_4:8; 0x04	在高字节
    }bytes;
}S_WORD;  这么定义的变量,用%d打印出来实际上是：0x04 03 02 01
```

### FSMC是扩展外部sram的接口。

### uart

> uart的接收中断有个hal库函数可以设置接到几个byte才发生中断：（相当于idel中断那个标志位）
> hal_uart_receive_it(&uart1,rxbuf,10);表示收到往rxbuf里面放10个byte才会进入中断处理函数。
> 这句话在uart初始完开启中断后就可以调一下。
> 这样我们可以直接在中断处理函数中处理rxbuf中的数据，解析。

> idel中断：https://blog.csdn.net/qq_29413829/article/details/63262321
> 串口的RXNE中断一次只能接收一个字节的数据，没有缓冲区，无法接收一帧多个数据
> rxne:接收buf非空，收满一个byte触发中断。
>
> ---
>
> RS232原理驱动：
> 	与uart驱动完全一样。
> RS485原理驱动：非屏蔽双绞线
> 	连线需要A--A  B--B
> 	uart+1个gpio =经过电平转换芯片= 485
> 	此gpio为h，表示发送状态 为l表示接收状态
> 	rs485_init()
> 	{
> 		初始化1个普通的io端口并设置为低电平；表示接收
> 		
>
> 	}
> 	rs485_printf(char * fmt,...)  发送函数
> 	{
> 		就是uart的发送函数；+gpio高低电平
> 	}

> ---
>
> 要使用printf之前需要在uart.c里面增加如下代码：
>
> ```c
> ```
>
> 

### IIC

> > https://blog.csdn.net/zhanghuaichao/article/details/48266309 对iic的解读，每个函数定义
>
> IIC驱动程序：基于stm32f10x_ll_iic.c 固件库 
> 	分为总线的驱动iic.c与器件的驱动程序lm75a.c 
> 	iic.c中集成：iic配置初始化函数	
> 初始化gpio+初始化iic控制器--注意需要开启对应iic控制器的时钟与对应的gpio的时钟
> 发送一个字节：按照协议->start->sendaddr+rw->senddata->stop
> 发送buffer：start->slaveaddr->while(datelen>0){发送一个byte;datebuf++}->stop
>
> ```c
> readbyte：
> 					{
> 						while(总线是否繁忙)；
> 						start；等待
> 						sendslaveaddr；等待有没有数据
> 						stop（）；
> 						buf=databuf(iic);将收到的datareg中的数据放到buf中返回。					
> 					}
> ```
>
> ​	lm75a.c中集成了：读取温度值函数->iicread_buffer(lm75addr,regaddr,buf,bytenum);
> ​	掉电函数->iicsend_byte(lm75addr,regaddr,1);--->一般不用，对应低功耗的板子才会去关断它。
>
> 手动模拟io口做iic怎么实现：
>
> ```c
> void SH30x_IICPort_Init(SIMIIC_name_t IICNum)
> {
> 	//配置io口的模式，输出类型，上拉下拉，速度
> 	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
> 	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
> 	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
> 	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_MEDIUM;
> 	//sda与scl线都是一样的配置，直接初始化对应的pin即可
> 	GPIO_InitStruct.Pin = IIC_GPIO_INFO[SIMIIC_1].SCL_PIN;
> 	LL_GPIO_Init(IIC_GPIO_INFO[SIMIIC_1].SCL_PORT, &GPIO_InitStruct);
> 	GPIO_InitStruct.Pin = IIC_GPIO_INFO[SIMIIC_1].SDA_PIN;
> 	LL_GPIO_Init(IIC_GPIO_INFO[SIMIIC_1].SDA_PORT, &GPIO_InitStruct);
> 	//初始化完成先发下stop信号，保证初始化没有iic通信
> 	SH30x_I2C_Stop(SIMIIC_1);
> }
> ```
>
> //需要看连接的外设的datasheet,不是看cpu里面的iic描述
> //sh367306的电气参数章节，有个关于通信的通信时序图，里面有个告诉得超过多久才能将电平拉低才会起作用
> //一般只有最小值不设置最大值。参考sh367306的56也的电器图
>
> ```c
> void SH30x_I2C_Stop(SIMIIC_name_t IICNum)
> {
> 	/* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
> 	GPIO_WritePin(IIC_GPIO_INFO[x].SDA_PORT, IIC_GPIO_INFO[x].SDA_PIN, GPIO_PIN_RESET);先让数据线为0
> 	Soft_Delay(2);延时，这里的实现就是让一个数递减，很占用cpu，可以用timer优化吗？
> 	I2C_SCL_1(IICNum);让scl为1
> 	Soft_Delay(1);延时 
> 	I2C_SDA_1(IICNum);数据线为1
> 	Soft_Delay(2);延时
> }
> ```
>
> ```c
> iic连续发多个字节怎么发：
> start->ack->slaveaddr->ack->data->ack->data->ack->data->ack->stop
> ```
>
> 

### gpio

> 复用的时候Alternate选择AFx怎么确认，在datesheet里面搜alternate function mapping
>
> ---
>
> 配置gpio-init：
> 	1.初始化struct变量
> 	2.将以上配置init(pinx,portA)
> 	pin的输入模式改成in后，设置的推挽输出就不起作用了，不影响
>
> EXTI中断：
> 	配置可以触发中断的中断号，PX
> 	把对应的gpio初始化为input模式--初始化gpio结构体
> 	初始化exti变量结构体--对应的pin脚
> 	设置优先级+使能中断--就是将EXTI与NVIC对应的通道结合起来
> 	exti0-15对应7个中断处理函数，这就是7个中断向量，NVIC里面定下来的。
> 		stm32的外部引脚中断只有0-15号中断，16以上的中断是内部其他控制器可以触发的中断
> 		exti是外部中断，nvic是arm内核里面的中断向量--exti配置的中断最终会跟nvic的中断对应起来
> 	EXTI_X_IRQ()
> 	{
> 		先判断是哪个中断
> 		{
> 			清除触发此号中断的标志位；
> 			调用回调函数处理中断---可以重写。
> 		}
> 	}

### 片内flash操作

> stm32对flash读写：falsh操作的途中要关闭中断__disable_irq();
>
> ```c
> 	flash写：传入flash中的add，传入要写入的data。
> 	固件库中提供了很多对flash的op函数
> 	{
> 		1.需要在这里或者外部提前开启flash的时钟            
> 		2.flash_unlock();  固定操作，每次操作前都要先解锁
> 		3.清除标志位x,y,z
> 		4.flash_erasepage(add); 清除要写的地址的数据，要以页的方式擦除
> 		5.flash_programword(add,data); 调用固件库函数从指定页的add开始写入data
> 		6.清除标志位x,y,z
> 		7.flash_lock(); 
> 	}
> 	flash读：传入add即可
> 	{
> 		u16 a;
> 		return a=*(u16*)(add); 从指定页的add开始读，只返回16bit的数据
> 	}
> ```
>
> 操作一定是先擦除再写入，每页1024个地址，起始地址0x08000000 
> 擦除以页为单位，写操作必须以16bit为单位，允许跨页写
> 擦除时注意避开用户程序的存储区域，避免擦除掉用户程序
>
> g030的flash大小64k，这64k的前面的地址用来存储code，最后的2k用来存数据.
> 手册里面每个page大小说是2k。
> 总的地址范围是：0x0800 0000---0x0801 0000这是16进制，则大小就是64k了。
> user的可用地址范围是：0x0801 0000-0x800到addr的end（刚好一页大小）
> 我们存到flash里面的数据就这一个dword共8byte，直接映射就得到了当前flash中的值dword = *(__IO uint64_t *)Address;
> flash里面的初始值就都是1，我们平时擦除的时候，就是将flash里面所有的bit置1.

### bootloader分析

> 启动文件：bootloader
> 1.初始化堆栈指针  sp指针由高地址向低地址生长
> 2.初始化pc指针，pc=reset_handle
> 3.初始化中断向量表
> 4.配置系统时钟
> 5.跳转到c里面的main函数
>
> 0x400=1k
> 1G=1ns  1M=1us  1k=1ms

### 看门狗

> 独立看门狗：
> 12bit递减计数器，8bit预分频器，用的内部40kHz的RC振荡器提供时钟，
> 这个振荡器独立于主时钟，所以它可以工作在停机待机模式。
> 通过配置可以配置成硬件或软件启动看门狗。
> 调试模式时，计数器可以被冻结。
>
> 窗口看门狗：
> 7bit递减计数器，可以设置自由运行，由主时钟驱动

### 中断

> nvic中断分为系统级的与外设级的：
> 系统级的有10个：例如reset，nmi,未定义指令，取指令失败，systick
> 外设级的有60个：各种外设
> nvic是嵌套中断向量控制器，属于内核的外设，管理所有的片上的中断相关的功能。
>
> 外设中断进程调用HAL_Delay(底层就是systick的中断)的时候，要特别注意中断的优先级问题，如果systick的中断优先级低于外设中断优先级，会导致一直在外设中断中阻塞，
> 如果外设中调用了HAL_Delay()，一定要保证Systick的中断优先级高于外设中断优先级，但是，小飞哥是极其不建议在中断中调用HAL_Delay（）函数的
>
> ---
>
> gpio中断设置时可以直接选择gpio_extix，这里这么配置后关于gpio的选项里面才会出现NVIC的配置
> 同时在NVIC里面也要打开EXTI对应的那些line，触发条件边沿触发，
> 使能相应的NVIC通道并选择优先级。
>
> ```c
> EXTI15_10_IRQHandle()
> {
> 	HAL_GPIO_EXTI_IRQHandle(pin)   如果没有hal库就自己在这个handle里面写操作就行，注意清flag。
> 	{
> 		清标志位
> 		执行callback回调函数
> 		{
> 			真正对pin脚的操作。
> 		}
> 	}
> }
> ```
>
> 对应的中断优先级的配置在界面的NVIC控制器里面可以通过对勾开启对应的中断并选择优先级。

### 消抖操作

```c
if(按键被按下)
{
	delay 20ms;   延时一会后按键仍被按下。
	if(案件被按下)
	{
		做真正的操作；
	}
}
判断充电状态的时候需要防抖动，得判断充电time>3才可以真正的去开充电mos。
```

### 寄存器映射

```c
寄存器存在于每个外设中，编程就是通过cpu配置各种外设的reg的值
给有特定功能的内存单元取别名，这个别名就是reg，给这个特定内存单元取别名的过程就是reg映射。

reg的映射过程演变：
1.直接操作地址：
	*(unsigned int)(0x12345678) = 0xffff
2.可以通过操作给每个地址取个名字，例如51单片机中的操作
	sfr p0=0x80；
	sfr tlo=0x40；
	写数据的时候可以直接 *p0=0xffff
3.在stm32中:不是通过sfr，而是通过define给每个地址取个名字。
	#define GPIOB_ODR (unsigned int)(0x12345678)
	操作： *GPIOB_ODR=0xffff；  这种方法往地址写数据
4.为了更方便：
	#define GPIOB_ODR *(unsigned int)(0x12345678)
	操作： GPIOB_ODR=0xffff；   
	c语言对总线外设基地址的映射通过头文件，这里的都是地址的映射，都没有带*
		define ahb  0x1
		define apb ahb+x
		define gpioB apb+x
		define gpidB_date gpioB+x
		define gpioB_ctl gpioB+y
	增加struct后呢？
	typedef struct{
		reg1;reg2;reg3;
	}gpio_typedef;
	只需定义一个gpio_typedef*变量就可以访问通过->访问相关reg。

	这种封装已经在固件库中封装好了，以上是封装的过程，原理。
```



## sourceInsight快捷键

ctrl+/ ：搜索当前函数或变量在整个工程中出现的位置，看哪里调用了函数或者修改了变量。

## 无法分类的知识

> 常量存在flash里面，变量存在sram里面。
>
> ---
>
> 固件库分析：
> 1.startup—stm32-hd.s hd表示flash大小，设置堆栈，pc指针，初始化中断向量表，配置sys时钟，调用_main进入c的世界
> 2.system-stm32f10x.c 时钟配置文件,倍频外部时钟8-72m
> 3.外设相关 stm32f10x.h  实现内核之外的外设的reg映射
> 	stm32f10x_xx.c 外设的初始化struct，外设的驱动函数库文件（这个文件很多
> 	种类例如：stm32f10x_gpio.h stm32f10x_uart.h stm32f10x_iic.h等等）
> 4.内核相关 core_cm3.h 实现内核里面的外设的reg映射 
> 5.头文件的配置文件 stm32f10x_conf.h 
> 	头文件的头文件，这个头文件中已经include了所有外设用的头文件
> 	例如：stm32f10x_gpio.h stm32f10x_uart.h stm32f10x_iic.h但是都是屏蔽的
> 	所以我们只需要包含这个一个头文件，将这个文件中的相关的include去掉屏蔽即可。
> 6.专门存放中断服务程序的c文件 stm32f10x_it.h  方便管理。
> nvic嵌套向量中断控制器，系统滴答定时器--misc.h
>
> ---
>
> ENUM
>
> typedef enum{var1,var2,var3} VVV;
> 这里的var1,var2,var3是用VVV定义的变量x只能从这3个数据中取值
> 并且：var1,var2,var3本身也可以直接在程序中去用，把它当成一个define好的宏去用就可以。
>
> typedef enum{
> 	uint32_t byte;
> 	struct {
> 		bit1:1;先写的bit是lsb，最下面的bit是msb
> 		...
> 	};
> }; 这里的这种方法共用地址，定义的变量初始化的时候可以用var.byte=0去初始化
> 往变量里面赋值的时候可以用var.bit的方法去1bit1bit的赋值
> 最后还是用var.byte的方法去读值。这种技巧很适合对于reg的操作。
>
> ---
>
> 移位操作
>
> 不同的数据左右移位之后的数据变化
> int i=1 有符号： 000001 右移之后成0000
>                     左移之后成00010
> int i=-1 有符号： 11111  左移之后右边补0后成11110  这个是-2
>                      右移之后左边补1,还是11111 还是-1，右移几bit都一样是-1
> unsigned int i=1 无符号：000001，右移之后左边补0变成00000
> 						左移之后还是变成2
> unsigned int i=-1，这个也是可以的，意思是i=2^64-1如果用%u输出
> 如果用%d输出还是-1，左移还是变成-2。%d会将原数据默认转换成signed int

---

> 参考程序:
>
> ```c
> int main()
> {
> 	int i=0;
> 	uint8_t buf[12] = {0};
> 	uint16_t var=0x1023; 
> 	buf[0] = var>>8; buf[0]只取var>>8的低8bit
> 	buf[1] = var;buf[1]只取var的低8bit，高bit不管
> 	printf("%x--%x\n",buf[0],buf[1]); 0x10 0x23
>     return 0;
> }
> ```
>
> 

## bootloader

> 为啥要有bootloader：
> flash中存的bootloader+app。0-1000存bootloader  1000+2000 存app.
> 第一次上电从bootloader运行判断是要下载程序还是直接运行，
>
> 1.若下载程序，直接根据新的app大小擦除覆盖1000-2000的数据,然后直接写入flash中新的app程序。
> 2.若要运行程序，直接跳到1000去执行。
>
> ### g030的app程序需要更改3处位置，作bootloader新程序。
>
> flash本身是从8000 000位置开始存放数据
> bootloader越小越好，一般从8000 000-8000 1900之前存bootloader，之后存app
>
> 1. keil设置target中的IROM1:0X80018F8表示新程序从这里开始。
>
> 2. 将main里面的static const uint8_t VERINFO[8] __attribute__((at(0x80018f8)))这句话写上，表示我们在程序最开始位置写gtds头信息(其实写入的并不是GTDSLONG，还|了一下别的参数)
>
> 3. 修改g0xx.c  #define VECT_TAB_OFFSET  0x1900U为啥是1900呢？
>
>      1）.程序开始是中断向量表必须是100的倍数。
>      2）. 本来我们想设置app从1800开始的，但是程序会变成gtdsxxxxxxxxxapp，中间的位置就空了，程序出现断裂， 所以我们直接把程序的开始放到1900往前偏移8byte程序就完全连接起来了。
>

---

> 16进制的数据里面会存放数据的位置信息，但是2进制的数据里面不会存放位置信息，所以2进制程序比16进制程序小很多--一半以上
> 第一次烧写程序的时候放的bootloader+app的16进制文件合并内容，以后再烧写的时候放的是bin文件。

---

> ## bin文件怎么用keil生成呢？
> 在keil配置的User里面选中after build/rebuild.
>
> ![image-20211026113910628](C:\Users\X1 YOGA\AppData\Roaming\Typora\typora-user-images\image-20211026113910628.png)
>
> ```shell
> fromelf --bin --output .\black_shield\black_shield.bin .\black_shield\black_shield.axf
> ```
>
> > fromelf --bin --output .\black_shield\black_shield.bin .\black_shield\black_shield.axf
> >
> > .\：为当前keil工程所在的同级路径.
>
> ### bin文件命名规范
>
> ```
> 项目代号_FACTURECODE_PROJECTCODE_HARDCODE_M_软件版本.bin
> jltest_01_620_V102_V100.bin
> 其中中间三项必须与app代码中的一模一样，项目代号与软件版本可以随便写。
> bin文件名字与hex的名字一致才可以生成bin文件。
> 烧写程序的时候监控软件有个硬件子版本号需要跟main刚开始的HARDCODE_S一致，切记
> ```
>
> ![image-20211026120823569](C:\Users\X1 YOGA\AppData\Roaming\Typora\typora-user-images\image-20211026120823569.png)

![image-20211026175611231](C:\Users\X1 YOGA\AppData\Roaming\Typora\typora-user-images\image-20211026175611231.png)

#### bootloader修改配置

> 初始化刷程序的uart
>
> 初始化power_en的gpio并设为1
>
> 修改flash大小：不同的片内flash不一样
>
> 修改FIRMWARE_MAXSIZE大小---boot_flash.c
>
> 其他地方不用改就好

#### app修改配置

> #define FLASH_USER_END_ADDR   (FLASH_BASE + 0X8000)    app_flash.h中8000是flash的size,这里用宏替代***
>
> 3.将cinfigure中的IROM1改成800 18f8
>
> system_stm32g0xx.c文件设置中断偏移量VECT_TAB_OFFSET改成1900,这个是程序的开始位置
>
> #define VECT_TAB_OFFSET  0x1900U

#### 调试

> 1.将system_stm32g0xx.c中断偏移量VECT_TAB_OFFSET改成0000
>
> 2.将main.c中刚开始的绝对地址赋值屏蔽。
>
> 3.将cinfigure中的IROM1改成800 0000



# keil报错

uint8_t is not a type name ; 原因：在某个.h文件中声明的函数名字没写对，只有

void (uint8_t *txdBuf,uint16_t len);

2. ST-link驱动怎么升级？点击device connect然后点击确定（需要将板子+st-link+笔记本连接起来才能升级）

![image-20211030115323689](C:\Users\X1 YOGA\AppData\Roaming\Typora\typora-user-images\image-20211030115323689.png)

还是连接不上

![image-20211030115904853](C:\Users\X1 YOGA\AppData\Roaming\Typora\typora-user-images\image-20211030115904853.png)

# 其他常识

#### 2v的电芯300a放电有危险吗？

没有，人体电阻1000k。站到300a导线中间，则电流就流不过去了，因为2/1000k=0.000002a一点危险都没有。

#### 测绝缘的机器2500v，为啥把人打不死？
它的输出只有2ma，两端断开的时候它就是2500v，但是人手解除的时候，人是2ma,2ma*2k=4v 接触的同时人身体的电压只有4v，并不是2500v

#### 保护板上的充电mos与放电mos怎么开启的？

正常放电的时候充电mos与放电mos都是闭合的，如放电有保护，则关闭放电mos，则现在不能放电只有充电mos是闭合的。此时如果充电，在放电mos的二极管会有0.7v压降，但是并不影响充电，保护板检测到有充电电流的时候会闭合放电mos，减小功耗的损失。

正常充电的时候充电mos与放电mos都是闭合的，如充电有保护，则关闭充电mos，则现在无法充电了，但是可以放电。放电时充电mos有0.7v压降，但是并不影响放电，保护板检测到放电电流的时候会闭合充电mos，减小功耗的损失。

#### uart

笔记本缺少232驱动 485连接无法通讯

缺少ch340驱动，串口无法通讯。





检测充电器是否撤去：对于脉冲充电器 需要在1ms中断中检测是否撤掉---
对于直冲充电器 直接检测充电器信号就知道充电器在不在
对于脉冲充电器  就得这么检测
sysflag.bits.CHG_DET_load = CHG_DET_Get();//1ms中断中 赋值即可
uint8_t CHG_DET_Get(void )
{
	static	uint8_t time =0 ; 
	static	uint16_t Retime =0 ; 
	static	uint8_t ret= 0;
	if(gpio_input_bit_get(chgdet_GPIO_Port, chgdet_Pin)==1)//说明chgdet在线
	{
		if(time<100) 
		{
			time++;
		}
		Retime =0;
	}
	else
	{
		if(Retime<2200)
		{
			Retime++;
		}
		time= 0;
	}
	if(time>10)			//有10ms 在线 说明充电器未撤
	{
		ret = 1;
	}
	else
	{
		if(Retime>2000) //持续2s 充电器不在 说明充电器撤掉了
		{
			ret = 0;
		}
	}
	return ret ;
}



小端：低字节在前 高字节在后
大端 高字节在前，低字节在后


同步帧处理：
采集程序：
while(rcvFlag==0 && (delay<2000) )  收不到同步帧，采集以下  能收到直接采集
{
	delay++;
	hal_delay(1ms);
}
//rcvFlag=0 delay=0
开始采集数据 ... 

回复数据 根据id
收到同步帧，将当前数据存到buffer，打开定时器 tim14_syscnt=0;
void TIM14_IRQHandler(void)
{
	LL_TIM_ClearFlag_UPDATE(TIM14);
	tim14_syscnt++;
	if(tim14_syscnt>(300+50*(syspara.BmsID-1)))//到了自己该发数据的时候 发数据
	{
		USART2_SendByte(10+SendLen.halfword);
		LL_TIM_DisableIT_UPDATE(TIM14);//停止定时器  等下次收到同步帧再开启
		LL_TIM_DisableCounter(TIM14);
		tim14_syscnt=0;
	}
}

深度休眠唤醒后：
void DeepSleepCheck(void)
{
	static uint32_t  ltime=0;
	static uint8_t  first_in=0;
	if(first_in==0)//第一次上电一定给ltime赋值，否则 直接欠压的时候 就直接进深度了
	{
		first_in=1;
		ltime=NowTime;
	}
	deepSleepFlag.bytes = 0;
	// 欠压才会触发深度休眠
	deepSleepFlag.bits.bit0 = ((syspara.voltMin < flashpara.bytes.CFG_ST_DPSLPV)) ? 1 : 0;
	
	if (deepSleepFlag.bytes != 0) //需要深度休眠
	{
		if ((NowTime-ltime)> (300)) 
		{
			DeepSleep();
		}
	}
	else//只能电压大的时候恢复   第一次如果欠压  就会直接进深度
	{
		deepsleep_sendflag=0;
		sysflag.bits.WAKE_RTC = 0;
		ltime=NowTime;
	}
}