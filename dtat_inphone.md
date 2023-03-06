### 寄存器映射

```c
//gpio 端口全部输出 高电平
#define  GPIO_ODR  (unsigned int*)(0x40010c0c)
*GPIO_ODR = 0xff;//给odr 寄存器赋值 使所有端口输出高电平
	|  进化的写法
	|
#define  GPIO_ODR  *(unsigned int*)(0x40010c0c)
GPIO_ODR = 0xff;   定义宏的时候直接把*写进去  就可以直接赋值了
```

```c
寄存器本身不具有地址信息，他的地址是芯片厂商或用户分配的
给reg分配地址的过程就是reg映射，如上block
```

```c
typedef struct  //特点就是内部的reg是4byte递增的，因为每个reg都是uint32_t  占4byte
{
    uint32_t CRL;		地址偏移0x00---如果不用struct，就得一个一个define，太麻烦了
    uint32_t CRH;		地址偏移0x04
    uint32_t IDR;		地址偏移0x08
    uint32_t ODR;		地址偏移0x0c
    uint32_t BSRR;		地址偏移0x10
    uint32_t LCKR;		地址偏移0x14
}GPIO_TypeDef;
结构体封装后，定义的GPIO_TypeDef类型的指针可以直接->访问内部的相关gpio的reg地址，而且地址是4 byte递增的，不需要一个一个去define了
```



### uart的通信波形

9600 波特率--1/9600=104us/bit

115200 波特率---1/115200=8us/bit

uart每个字节的传输包含---1bit(start)+8bit(数据)+1bit(stop)-------start：波形拉下来  stop：波形主动拉上去



### can收发器与can控制器的关系

一般can控制器已经被继承在mcu内部了，只需要在mcu外部can控制器外部链接can收发器，就可以直接与can总线相连，收发can数据。



### JTAG与sw关系
jtag烧写程序用4个线，sw烧写只需要2个线（jtag中的2根即可）

jtag与sw都可以调试



### 单片机的启动文件

.s文件，ST官方提供了几个.s

start_stm32fxx_ld.s---low density  			小容量  flash容量在32--64k之间

start_stm32fxx_md.s---middle density  	中容量  flash容量在64--128k之间

start_stm32fxx_ld.s---high  density  		  大容量  flash容量在256--512k之间

start_stm32fxx_xd.s---超大容量  		  大容量  flash容量在521-1024k之间



### TTL电平与CMOS电平区别

TTL：

输入L:<0.8   H:>2.4

输出L:<1.2    H:>2.0

cmos:

输入L：<0.1Vcc   H:>0.9Vcc

输出L：<0.3Vcc   H:>0.7Vcc

判断电平高低的标准不一样



### DCDC转换器

高压DC--AC--变压器--整流二极管---低压DC



