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
static struct class *hello_class;
//1.确定主设备号
static int major;

// 3.实现对应的系统调用函数，填入file_operations
static char kernel_buf[1024];//创建内核数组，只属于内核，作为用户空间与内核空间的数据交换
#define MIN(a,b)    (a<b?a:b)
ssize_t hello_drv_read (struct file *file, char __user *buf, size_t size, loff_t *offset)
{
    unsigned long ret;
    printk("%s %s line:%d\n",__FILE__,__FUNCTION__,__LINE__);
    //返回值要接一下，否则编译报错，内核对函数调用要求严格
    ret=copy_to_user(buf,kernel_buf, MIN(1024,size));
    return MIN(1024,size);//返回读了多少数据
}
ssize_t hello_drv_write (struct file *file, const char __user *buf, size_t size, loff_t *offset)
{
    unsigned long ret;
    printk("%s %s line:%d\n",__FILE__,__FUNCTION__,__LINE__);
    ret=copy_from_user(kernel_buf, buf,MIN(1024,size));
    return MIN(1024,size);//返回写了多少数据
}
int hello_drv_open (struct inode *node, struct file *file)
{
    printk("%s %s line:%d\n",__FILE__,__FUNCTION__,__LINE__);
    return 0;
}
int hello_drv_close (struct inode *node, struct file *file)
{
    printk("%s %s line:%d\n",__FILE__,__FUNCTION__,__LINE__);
    return 0;
}

// 2.定义自己的file_operations结构体
static struct file_operations hello_drv={
    .owner  = THIS_MODULE,
    .open   =  hello_drv_open,
    .release   =  hello_drv_close,
    .read   =  hello_drv_read,
    .write   =  hello_drv_write,
};
// 3.实现对应的系统调用函数，填入file_operations
// 4.把完成的file_operations注册进内核：注册驱动程序
// 5.谁来注册：入口函数来
static int __init hello_init(void)
{
    int err;
    printk("%s %s line:%d\n",__FILE__,__FUNCTION__,__LINE__);
    //注册的时候内核会分配此设备的主设备号
	major = register_chrdev(0, "hello", &hello_drv);
    hello_class=class_create(THIS_MODULE,"hello_class");
    err = PTR_ERR(hello_class);
    if(IS_ERR(hello_class))
    {
        unregister_chrdev(major, "hello");
        return -1;
    }
    //入参是设备号，由主设备号与次设备号组成
    device_create(hello_class,NULL,MKDEV(major,0),NULL,"hello");
        
	return 0;
}
// 6.出口函数
static void __exit hello_exit(void)
{
    printk("%s %s line:%d\n",__FILE__,__FUNCTION__,__LINE__);
    //退出的时候destroy与创建时的顺序相反
    device_destroy(hello_class,MKDEV(major,0));
    class_destroy(hello_class);
	unregister_chrdev(major, "hello");
	return ;
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
// 7.提供设备信息，自动创建设备节点---在入口函数中(创建类+设备)
