//将platform_device弄进虚拟bus中，纯粹的硬件资源insmod
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
#include <linux/platform_device.h>

#include "led_resource.h"


static void led_dev_release(struct device *dev)
{
}
//resources里面就是所有的资源
static struct resource resources[] = {
        {
                .start = GROUP_PIN(3,1),
                .flags = IORESOURCE_IRQ,
                .name = "100ask_led_pin",
        },
        {
                .start = GROUP_PIN(5,8),
                .flags = IORESOURCE_IRQ,
                .name = "100ask_led_pin",
        },//若要增加硬件资源，在这里增加
};

//此结构体整合了所有dev那边的数据
static struct platform_device board_A_led_dev = {
        .name = "100ask_led",//name
        .num_resources = ARRAY_SIZE(resources),//resource的数量，(sizeof(x)/sizeof((x)[0]))
        .resource = resources,//具体的资源数据
        .dev = {
                .release = led_dev_release,
         },
};

static int __init led_dev_init(void)
{
    int err;
    //入口函数，初始化中将platform_device弄进虚拟bus中，纯粹的硬件资源insmod
    err = platform_device_register(&board_A_led_dev);   
    
    return 0;
}

static void __exit led_dev_exit(void)
{
    platform_device_unregister(&board_A_led_dev);
}

module_init(led_dev_init);//平台dev也需要注册成驱动
module_exit(led_dev_exit);

MODULE_LICENSE("GPL");

