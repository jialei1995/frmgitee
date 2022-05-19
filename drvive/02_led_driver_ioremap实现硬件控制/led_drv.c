#include <linux/module.h>

#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/kmod.h>
#include <linux/gfp.h>
/*自己定义寄存器映射，定义的reg需要在入口函数中去映射*/
//初始化io用到的寄存器
static volatile unsigned int * REG1;//实际地址是0x50000001
static volatile unsigned int * REG2;//实际地址是0x50000002
static volatile unsigned int * REG3;//实际地址是0x50000003
static volatile unsigned int * REG4;//实际地址是0x50000004

//考内核中driver/char/misc.c文件--经典的字符设备驱动
/*流程：
    1.确定主设备号
    2.定义自己的file_operations结构体
    3.实现对应的系统调用函数，填入file_operations
    4.把完成的file_operations注册进内核：注册驱动程序
    5.谁来注册：入口函数
    6.出口函数
    7.提供设备信息，自动创建设备节点
*/
static struct class *led_class;
//1.确定主设备号
static int major;

// 3.实现对应的系统调用函数，填入file_operations
static char kernel_buf[1024];//创建内核数组，只属于内核，作为用户空间与内核空间的数据交换
#define MIN(a,b)    (a<b?a:b)
ssize_t led_read (struct file *file, char __user *buf, size_t size, loff_t *offset)
{
    unsigned long ret;
    printk("%s %s line:%d\n",__FILE__,__FUNCTION__,__LINE__);
    ret=copy_to_user(buf,kernel_buf, MIN(1024,size));
    return MIN(1024,size);//返回读了多少数据
}
ssize_t led_write (struct file *file, const char __user *buf, size_t size, loff_t *offset)
{
    //不能直接将1/0写到物理地址，需要先将物理地址映射为虚拟地址才可以写，在哪里映射呢？入口函数中映射，
    //别的操作函数就可以直接用映射后的地址了
    unsigned long ret;
    char val;
    printk("%s %s line:%d\n",__FILE__,__FUNCTION__,__LINE__);
    ret=copy_from_user(&val, buf,1);//开关的操作要么是0/1，1字节就够了
    if(val)
    {
        //开灯
        *REG4 |= 1<<26;
    }else
    {
        *REG4 &= ~(1<<26);
    }
    return MIN(1024,size);//返回写了多少数据
}
int led_open (struct inode *node, struct file *file)
{
    printk("%s %s line:%d\n",__FILE__,__FUNCTION__,__LINE__);
    //初始化gpio
    //使能总线时钟
    *REG1 |= (1<<0);
    //等待总线使能成功
    while((*REG1&0x02)==0);
    //PLL4总线稳定
    //使能GPIOA端口
    *REG2 |= 1<<0;
    //配置为输出模式，配置输出电平1/0
    *REG3 &=~(3<<20);
    *REG3 |=~(1<<20);
    //初始化io完成。后面可以直接操作io管脚高低控制led
    return 0;
}
int led_close (struct inode *node, struct file *file)
{
    printk("%s %s line:%d\n",__FILE__,__FUNCTION__,__LINE__);
    return 0;
}

// 2.定义自己的file_operations结构体
static struct file_operations led={
    .owner  = THIS_MODULE,
    .open   =  led_open,
    .release   =  led_close,
    .read   =  led_read,
    .write   =  led_write,
};
// 3.实现对应的系统调用函数，填入file_operations
// 4.把完成的file_operations注册进内核：注册驱动程序
// 5.谁来注册：入口函数
static int __init led_init(void)
{
    int err;
    printk("%s %s line:%d\n",__FILE__,__FUNCTION__,__LINE__);

	major = register_chrdev(0, "led", &led);//创建主设备号，用此设备号将用户空间与内核空间联系起来
    led_class=class_create(THIS_MODULE,"led_class");
    err = PTR_ERR(led_class);
    if(IS_ERR(led_class))
    {
        unregister_chrdev(major, "led");
        return -1;
    }
    //入参是设备号，由主设备号与次设备号组成
    device_create(led_class,NULL,MKDEV(major,0),NULL,"led");

    //开始ioremap映射物理地址,映射4字节，因为每个reg存的都是32bit的数据----映射放到open的时候更合理，只有open的时候才占内核态内存
    REG1=ioremap(0x50000001,4);//实际地址是0x50000001
    REG2=ioremap(0x50000002,4);//实际地址是0x50000002
    REG3=ioremap(0x50000003,4);//实际地址是0x50000003
    REG4=ioremap(0x50000004,4);//实际地址是0x50000004

        
	return 0;
}
// 6.出口函数
static void __exit led_exit(void)
{
    printk("%s %s line:%d\n",__FILE__,__FUNCTION__,__LINE__);
    iounmap(REG1);
    iounmap(REG2);
    iounmap(REG3);
    iounmap(REG4);
    device_destroy(led_class,MKDEV(major,0));
    class_destroy(led_class);
	unregister_chrdev(major, "led");
	return ;
}

module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");
// 7.提供设备信息，自动创建设备节点---在入口函数中(创建类+设备)
