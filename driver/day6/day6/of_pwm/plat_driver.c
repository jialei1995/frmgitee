#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
extern int  hello_init(struct platform_device *pDevice);
extern void hello_cleanup(struct platform_device *pDevice);
int HelloDriverProbe(struct platform_device *pDevice)
{//平台驱动的探测函数,  同时带过来平台设备的结构体pDevice  
	//字符设备的完整的流程 
	hello_init(pDevice);
        printk("Driver,HelloDriverProbe\r\n");
        return 0;
}

int HelloDriverRemove(struct platform_device *pDevice)
{//平台驱动的remove函数
			hello_cleanup(pDevice);
        printk("Driver, HelloDriverRemove\r\n");
        return 0;
}
struct of_device_id  hello_id[]= {
	{.compatible = "fs4412,pwm"},
	{ }
};

static struct platform_driver g_stPlantFormDriver =
{
        .driver = {
                .name = "hehe1_platform",// 该平台驱动支持的设备
                .owner = THIS_MODULE,
		.of_match_table = hello_id, //hello_id 描述了该平台驱动支持的设备树里的某一个设备， 
        },
        .probe  = HelloDriverProbe,//注册了该平台驱动时， 如果该驱动支持的设备存在，当前的probe函数会被执行
        .remove =  HelloDriverRemove,//注销该平台驱动时， 当前的remove函数会被执行到
};


int HelloDriverInit(void)
{
        int iRet = 0;
        printk("driver, module init enter\r\n");

        iRet = platform_driver_register(&g_stPlantFormDriver);//注册平台驱动
        if (0 != iRet)
        {
                printk("platform_driver_register error");
                goto DEVICE_REGISTER_ERR;
        }
        printk("driver, module init exit\r\n");
        return iRet;

DEVICE_REGISTER_ERR:
        platform_driver_unregister(&g_stPlantFormDriver);
        return iRet;
}

void HelloDriverRelease(void)
{
        printk("driver, clean module enter\r\n");
        platform_driver_unregister(&g_stPlantFormDriver);
        printk("driver, clean module exit\r\n");
}

module_exit(HelloDriverRelease);
module_init(HelloDriverInit);

MODULE_LICENSE("GPL");


