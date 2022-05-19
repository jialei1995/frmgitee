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

#include "led_opr.h"

#define LED_NUM 2//定义设备的个数

/* 1. 确定主设备号                                                                 */
static int major = 0;
static struct class *led_class;
struct led_operations *p_led_opr;//空指针


#define MIN(a, b) (a < b ? a : b)

/* 3. 实现对应的open/read/write等函数，填入file_operations结构体                   */
static ssize_t led_drv_read (struct file *file, char __user *buf, size_t size, loff_t *offset)
{
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	return 0;
}

/* write(fd, &val, 1); */
static ssize_t led_drv_write (struct file *file, const char __user *buf, size_t size, loff_t *offset)
{
	int err;
	char status;
	struct inode *inode = file_inode(file);
	int minor = iminor(inode);//获得次设备号：write中入参没有inode得从struct file中先获得inode
	//然后再通过inode中获得次设备号
	
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	err = copy_from_user(&status, buf, 1);

	/* 根据次设备号和用户空间传的status控制LED,此设备号的获取如上 */
	p_led_opr->ctl(minor, status);//已经有remap后的虚拟地址则可以直接操作寄存器了
	
	return 1;
}

static int led_drv_open (struct inode *node, struct file *file)
{
	int minor = iminor(node);//直接通过node获得次设备号
	
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	/* 根据次设备号初始化想控制的LED */
	p_led_opr->init(minor);//在init中ioremap物理地址并初始化硬件
	
	return 0;
}

static int led_drv_close (struct inode *node, struct file *file)
{
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	return 0;
}

/* 2. 定义自己的file_operations结构体                                              */
static struct file_operations led_drv = {
	.owner	 = THIS_MODULE,
	.open    = led_drv_open,
	.read    = led_drv_read,
	.write   = led_drv_write,
	.release = led_drv_close,
};

/* 4. 把file_operations结构体告诉内核：注册驱动程序                                */
/* 5. 谁来注册驱动程序啊？得有一个入口函数：安装驱动程序时，就会去调用这个入口函数 */
static int __init led_init(void)
{
	int err;
	int i;
	
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	major = register_chrdev(0, "100ask_led", &led_drv);  /* /dev/led */

	//led类只需要创建1个
	led_class = class_create(THIS_MODULE, "100ask_led_class");
	err = PTR_ERR(led_class);
	if (IS_ERR(led_class)) {
		printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
		unregister_chrdev(major, "100ask_led");
		return -1;
	}
	//根据同一个类可以创建多个设备
	for (i = 0; i < LED_NUM; i++)//这里次设备号分别为0..2..n
		device_create(led_class, NULL, MKDEV(major, i), NULL, "100ask_led%d", i); /* /dev/100ask_led0,1,... */

	p_led_opr = get_board_led_opr();//指向对应的操作方法
	
	return 0;
}

/* 6. 有入口函数就应该有出口函数：卸载驱动程序时，就会去调用这个出口函数           */
static void __exit led_exit(void)
{
	int i;
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);

	for (i = 0; i < LED_NUM; i++)
		device_destroy(led_class, MKDEV(major, i)); /* /dev/100ask_led0,1,... */

	device_destroy(led_class, MKDEV(major, 0));
	class_destroy(led_class);
	unregister_chrdev(major, "100ask_led");
}


/* 7. 其他完善：提供设备信息，自动创建设备节点                                     */

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");


