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
#include <asm/pgtable.h>
#include <linux/mm.h>
#include <linux/slab.h>

/* 1. 确定主设备号                                                                 */
static int major = 0;
static char *kernel_buf;
static struct class *hello_class;
static int bufsiz = 1024*8;//内核中的数组大小

#define MIN(a, b) (a < b ? a : b)

/* 3. 实现对应的open/read/write等函数，填入file_operations结构体                   */
static ssize_t hello_drv_read (struct file *file, char __user *buf, size_t size, loff_t *offset)
{
	int err;
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	err = copy_to_user(buf, kernel_buf, MIN(bufsiz, size));
	return MIN(bufsiz, size);
}

static ssize_t hello_drv_write (struct file *file, const char __user *buf, size_t size, loff_t *offset)
{
	int err;
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	err = copy_from_user(kernel_buf, buf, MIN(1024, size));
	return MIN(1024, size);
}

static int hello_drv_mmap(struct file *file, struct vm_area_struct *vma)
{
	/* 获得内核buf的真实物理地址 */
	unsigned long phy = virt_to_phys(kernel_buf);
	/* 设置属性:是否使用 cache, buffer */
	/* 如果使用了cache，有可能我们写入reg（显卡、dma）的数据可能不会直接写入，先写到cahe--再写到reg-导致程序运行异常
	，没有正常操作硬件
	对于dma、reg、显卡framebuffer不能使用cache，对于一般的内存可以使用cache*/
	//这个宏指定了是否用cache，是否用写buf---能否直接写到硬件
	vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);
	/* map vma-用户空间的虚拟地址 */
	if (remap_pfn_range(vma, vma->vm_start, phy >> PAGE_SHIFT,  //映射用户空间的虚拟地址到哪一页？一般一页4096
			    vma->vm_end - vma->vm_start, vma->vm_page_prot)) {
		printk("mmap remap_pfn_range failed\n");
		return -ENOBUFS;
	}
	return 0;
}

static int hello_drv_open (struct inode *node, struct file *file)
{
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	return 0;
}

static int hello_drv_close (struct inode *node, struct file *file)
{
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	return 0;
}

/* 2. 定义自己的file_operations结构体                                              */
static struct file_operations hello_drv = {
	.owner	 = THIS_MODULE,
	.open    = hello_drv_open,
	.read    = hello_drv_read,
	.write   = hello_drv_write,
	.release = hello_drv_close,
	.mmap    = hello_drv_mmap,
};

/* 4. 把file_operations结构体告诉内核：注册驱动程序                                */
/* 5. 谁来注册驱动程序啊？得有一个入口函数：安装驱动程序时，就会去调用这个入口函数 */
static int __init hello_init(void)
{
	int err;

	kernel_buf = kmalloc(bufsiz, GFP_KERNEL);//在内核中开辟空间，开辟bufsiz大小
	strcpy(kernel_buf, "old");//填充原始数据，old字母
	
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	major = register_chrdev(0, "hello", &hello_drv);  /*将file_opera结构体注册进内核，用主设备号major关联起来*/


	hello_class = class_create(THIS_MODULE, "hello_class");//创建hello类，创建具体设备需要类
	err = PTR_ERR(hello_class);
	if (IS_ERR(hello_class)) {
		printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
		unregister_chrdev(major, "hello");
		return -1;
	}
	
	device_create(hello_class, NULL, MKDEV(major, 0), NULL, "hello0"); /* 占用次设备号0，创建/dev/hello0设备 */
	device_create(hello_class, NULL, MKDEV(major, 1), NULL, "hello1"); /* 占用次设备号1，创建/dev/hello1设备  */

	return 0;
}

/* 6. 有入口函数就应该有出口函数：卸载驱动程序时，就会去调用这个出口函数           */
static void __exit hello_exit(void)
{
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	device_destroy(hello_class, MKDEV(major, 0));
	class_destroy(hello_class);
	unregister_chrdev(major, "hello0");//删除设备0
	unregister_chrdev(major, "hello1");//删除设备1
	kfree(kernel_buf);
}


/* 7. 其他完善：提供设备信息，自动创建设备节点                                     */

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");


