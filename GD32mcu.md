# `GD32` 使用

怎么扩展存储空间:

​	利用`eeprom`或者sd卡存储数据，通过`i2c`或`spi`或`uart`的通信方式与外设芯片通信，存储读取数据。



直接使用`jflash`烧写合成的hex文件：

+ 官网下载`JLink_Windows_V690`安装包----32位的就行
+ 要选择芯片类型等等。。。
+ 配置：烧写方式为`SWD`方式，不是`JTAG`方式
+ 点击Target->Manual Programming进行烧写



### `printf`底层实现

对于stdio中的`printf`函数：

```c
Keil 和 IAR：printf will call fputc to printf
int fputc();

gcc: printf will call this func to printf
int _put_char_();

这两个底层函数内部实现一摸一样，只是函数名字不同，不同的编译器编译printf的时候回选择不同的底层函数。
```



### 串口`DMA`功能配置

```c
void usart_dma_coonfig()
{
	dma_parameter_struct dma_init_struct;
	rcu_periph_clock_enable(RCU_DMA0);//使用dma0去输送数据
	//deinit  dma的channel 3-usart的tx
	dma_deinit(DMA0,DMA_CH3);
	dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;//内存到总线---变量值往总线发
	dma_init_struct.memory_addr = (uint32_t)tx_buff;//发数据的buf地址，自己定义好的数组
	dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;//内存是递增的
	dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;//1字节1字节的发
	dma_init_struct.number = sizeof(tx_buff)/sizeof(*tx_buff);//一次发多少字节,就是数组大小
	dma_init_struct.periph_addr = USART_DATA_ADDRESS;//内存数据要往哪里写，串口的发送缓冲区，写到这里数据就自动发走了--需要自己宏定义位置
    //USART_DATA_ADDRESS这个地址可以自己计算的====&USART_DATA(USART0)计算数据reg具体地址
	dma_init_struct.periph_width = DMA_PERIPH_WIDTH_8BIT;
	dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_ENABLE;
	dma_init_struct.priority = DMA_PRIORITY_HIGH;//设置优先级
	dma_init(DMA0,DMA_CH3,&dma_init_struct);
	//cfg dma mode
	dma_circulation_disable(DMA0,DMA_CH3);//循环禁用
	dma_memory_to_memory_disable(DMA0,DMA_CH3);//串口的传输方向是内存到总线，则关掉内存到内存

	//配置接收数组-怎么从外设上读数据
	dma_deinit(DMA0,DMA_CH4);//deinit ch4通道
	dma_init_struct.direction = DMA_PERIPHERAL_TO_MEMORY;//总线到内存---总线发数据到内存变量
	dma_init_struct.memory_addr = (uint32_t)rx_buff;//收数据的buf地址，自己定义好的数组
	dma_init(DMA0,DMA_CH4,&dma_init_struct);
	//cfg dma mode
	dma_circulation_disable(DMA0,DMA_CH4);//循环禁用
	dma_memory_to_memory_disable(DMA0,DMA_CH4);
}
char txbuf[10] = "0123456789";
char rxbuf[10] = "";
int main()
{
    usart_dma_coonfig();
    //2个通道都使能
	dma_channel_enable(DMA0,DMA_CH3);
	dma_channel_enable(DMA0,DMA_CH4);
	//DMA request enable for transmission
	usart_dma_transmit_config(USART0,USART_DENT_ENABLE);
	//DMA request enable for reception 
	usart_dma_receive_config(USART0,USART_DENR_ENABLE);
	//full transfer finish flag of channel   阻塞等待发送完成
	while(RESET == dma_flag_get(DMA0,DMA_CH3,DMA_INTF_FTFIF));
	//阻塞等待接收完成
	while(RESET == dma_flag_get(DMA0,DMA_CH4,DMA_INTF_FTFIF));
    
    compare(对比txbuf与rxbuf数据是否一致);
}
```



### 程序中用到`jtag`所在的引脚的时候需要关闭`jtag`功能

```gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP,ENABLE);```

一条语句即可，不能禁用swd功能，否则无法烧录程序了---实在要禁用swd功能，前面加延时，保证上电能烧写hex。



### 逻辑移位与算数移位

```c
逻辑移位：
    移出的空位都用 0 来补
算数移位：
    对于无符号型算数移位，算数移位 == 逻辑移位
    对于有符号数，算数左移等同于逻辑左移
    			算数右移补的是符号位，正数补0  负数补1

对于c编译中用的哪种移位方式呢？
对于无符号类型----编译器默认生成的汇编指令是逻辑左移和逻辑右移。
对于有符号的数据进行移位时，左移还是逻辑左移，但右移时生成的汇编指令是算术右移。
    
ps：
int main()
{
    char d = 127; //0111 1111  
    d >>= 3; //0000 1111 15   知道d是个正数 右移的时候左边补0
    char e = -8; //内存以补码为1111 1000存储  源码为1000 1000 反码为1111 0111
    e >>= 3; //-1  补码1111 1111 源码 1000 0001 反码1111 1110
    #知道e是个负数 右移的时候左边补1

    return 0;
}

```

##### 以后对于无符号或者有符号的正数都按照逻辑移位；对于负数就按照算数移位



### `DAC`是什么?

将内存的变量0~2^12数字量转化成`0~3.3V`对应电压并输出电压。

用于转速快慢控制，输出电压强弱控制。

```c
#define DAC_OUT_VAL  0x7ff0   //2^12--最大4096
void dac_config()
{
	rcu_periph_clock_enable(RCU_DAC);//使能dac时钟
	dac_deinit();
	//配置dac
	dac_trigger_disable(DAC0);
	dac_wave_mode_config(DAC0,DAC_WAVE_DISABLE);
	dac_output_buffer_enable(DAC0);
	//使能DAC0功能  设置初始化输出电压大小  左对齐
	dac_data_set(DAC0,DAC_ALIGN_12B_L,DAC_OUT_VAL);
}

左对齐：0x7ff0===0x7ff因为12bit需要左对齐 
右对齐：就是正常的数据
dac_data_set(DAC0,DAC_ALIGN_12B_L,value);可以在程序中动态改变输出电压。
dac_data_set(DAC0,DAC_ALIGN_12B_L,DAC_OUT_VAL);
dacvalue = dac_output_value_get(DAC0);  获取dac的数字量
```

