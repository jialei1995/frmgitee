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

//�ο��ں���driver/char/misc.c�ļ�--������ַ��豸����
/*���̣�
    1.ȷ�����豸��
    2.�����Լ���file_operations�ṹ��
    3.ʵ�ֶ�Ӧ��ϵͳ���ú���������file_operations
    4.����ɵ�file_operationsע����ںˣ�ע����������
    5.˭��ע�᣺��ں���
    6.���ں���
    7.�ṩ�豸��Ϣ���Զ������豸�ڵ�
*/
//1.ȷ�����豸��
static int major;

// 3.ʵ�ֶ�Ӧ��ϵͳ���ú���������file_operations
static char kernel_buf[1024];//�����ں����飬ֻ�����ںˣ���Ϊ�û��ռ����ں˿ռ�����ݽ���
#define MIN(a,b)    (a<b?a:b)
ssize_t hello_drv_read (struct file *file, char __user *buf, size_t size, loff_t *offset)
{
    printk("%s %s line:%s\n",__FILE__,__FUNCTION__,__LINE__);
    copy_to_user(buf,kernel_buf, MIN(1024,size));
    return MIN(1024,size);//���ض��˶�������
}
ssize_t hello_drv_write (struct file *file, const char __user *buf, size_t size, loff_t *offset)
{
    printk("%s %s line:%s\n",__FILE__,__FUNCTION__,__LINE__);
    copy_from_user(kernel_buf, buf,MIN(1024,size));
    return MIN(1024,size);//����д�˶�������
}
int hello_drv_open (struct inode *node, struct file *file)
{
    printk("%s %s line:%s\n",__FILE__,__FUNCTION__,__LINE__);
    return 0;
}
int hello_drv_close (struct inode *node, struct file *file)
{
    printk("%s %s line:%s\n",__FILE__,__FUNCTION__,__LINE__);
    return 0;
}

// 2.�����Լ���file_operations�ṹ��
static struct file_operations hello_drv={
    .owner  = THIS_MODULE,
    .open   =  hello_drv_open,
    .close   =  hello_drv_close,
    .read   =  hello_drv_read,
    .write   =  hello_drv_write,
};
// 3.ʵ�ֶ�Ӧ��ϵͳ���ú���������file_operations
// 4.����ɵ�file_operationsע����ںˣ�ע����������
// 5.˭��ע�᣺��ں���
static int __init hello_init(void)
{
	major = register_chrdev(0, "hello", &hello_drv);
	return 0;
}
// 6.���ں���
static int __exit hello_exit(void)
{
	unregister_chrdev(major, "hello");
	return 0;
}

module_init(hello_init);
module_exit(hello_exit);
// 7.�ṩ�豸��Ϣ���Զ������豸�ڵ�