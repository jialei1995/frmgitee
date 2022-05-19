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
/*�Լ�����Ĵ���ӳ�䣬�����reg��Ҫ����ں�����ȥӳ��*/
//��ʼ��io�õ��ļĴ���
static volatile unsigned int * REG1;//ʵ�ʵ�ַ��0x50000001
static volatile unsigned int * REG2;//ʵ�ʵ�ַ��0x50000002
static volatile unsigned int * REG3;//ʵ�ʵ�ַ��0x50000003
static volatile unsigned int * REG4;//ʵ�ʵ�ַ��0x50000004

//���ں���driver/char/misc.c�ļ�--������ַ��豸����
/*���̣�
    1.ȷ�����豸��
    2.�����Լ���file_operations�ṹ��
    3.ʵ�ֶ�Ӧ��ϵͳ���ú���������file_operations
    4.����ɵ�file_operationsע����ںˣ�ע����������
    5.˭��ע�᣺��ں���
    6.���ں���
    7.�ṩ�豸��Ϣ���Զ������豸�ڵ�
*/
static struct class *led_class;
//1.ȷ�����豸��
static int major;

// 3.ʵ�ֶ�Ӧ��ϵͳ���ú���������file_operations
static char kernel_buf[1024];//�����ں����飬ֻ�����ںˣ���Ϊ�û��ռ����ں˿ռ�����ݽ���
#define MIN(a,b)    (a<b?a:b)
ssize_t led_read (struct file *file, char __user *buf, size_t size, loff_t *offset)
{
    unsigned long ret;
    printk("%s %s line:%d\n",__FILE__,__FUNCTION__,__LINE__);
    ret=copy_to_user(buf,kernel_buf, MIN(1024,size));
    return MIN(1024,size);//���ض��˶�������
}
ssize_t led_write (struct file *file, const char __user *buf, size_t size, loff_t *offset)
{
    //����ֱ�ӽ�1/0д�������ַ����Ҫ�Ƚ������ַӳ��Ϊ�����ַ�ſ���д��������ӳ���أ���ں�����ӳ�䣬
    //��Ĳ��������Ϳ���ֱ����ӳ���ĵ�ַ��
    unsigned long ret;
    char val;
    printk("%s %s line:%d\n",__FILE__,__FUNCTION__,__LINE__);
    ret=copy_from_user(&val, buf,1);//���صĲ���Ҫô��0/1��1�ֽھ͹���
    if(val)
    {
        //����
        *REG4 |= 1<<26;
    }else
    {
        *REG4 &= ~(1<<26);
    }
    return MIN(1024,size);//����д�˶�������
}
int led_open (struct inode *node, struct file *file)
{
    printk("%s %s line:%d\n",__FILE__,__FUNCTION__,__LINE__);
    //��ʼ��gpio
    //ʹ������ʱ��
    *REG1 |= (1<<0);
    //�ȴ�����ʹ�ܳɹ�
    while((*REG1&0x02)==0);
    //PLL4�����ȶ�
    //ʹ��GPIOA�˿�
    *REG2 |= 1<<0;
    //����Ϊ���ģʽ�����������ƽ1/0
    *REG3 &=~(3<<20);
    *REG3 |=~(1<<20);
    //��ʼ��io��ɡ��������ֱ�Ӳ���io�ܽŸߵͿ���led
    return 0;
}
int led_close (struct inode *node, struct file *file)
{
    printk("%s %s line:%d\n",__FILE__,__FUNCTION__,__LINE__);
    return 0;
}

// 2.�����Լ���file_operations�ṹ��
static struct file_operations led={
    .owner  = THIS_MODULE,
    .open   =  led_open,
    .release   =  led_close,
    .read   =  led_read,
    .write   =  led_write,
};
// 3.ʵ�ֶ�Ӧ��ϵͳ���ú���������file_operations
// 4.����ɵ�file_operationsע����ںˣ�ע����������
// 5.˭��ע�᣺��ں���
static int __init led_init(void)
{
    int err;
    printk("%s %s line:%d\n",__FILE__,__FUNCTION__,__LINE__);

	major = register_chrdev(0, "led", &led);//�������豸�ţ��ô��豸�Ž��û��ռ����ں˿ռ���ϵ����
    led_class=class_create(THIS_MODULE,"led_class");
    err = PTR_ERR(led_class);
    if(IS_ERR(led_class))
    {
        unregister_chrdev(major, "led");
        return -1;
    }
    //������豸�ţ������豸������豸�����
    device_create(led_class,NULL,MKDEV(major,0),NULL,"led");

    //��ʼioremapӳ�������ַ,ӳ��4�ֽڣ���Ϊÿ��reg��Ķ���32bit������----ӳ��ŵ�open��ʱ�������ֻ��open��ʱ���ռ�ں�̬�ڴ�
    REG1=ioremap(0x50000001,4);//ʵ�ʵ�ַ��0x50000001
    REG2=ioremap(0x50000002,4);//ʵ�ʵ�ַ��0x50000002
    REG3=ioremap(0x50000003,4);//ʵ�ʵ�ַ��0x50000003
    REG4=ioremap(0x50000004,4);//ʵ�ʵ�ַ��0x50000004

        
	return 0;
}
// 6.���ں���
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
// 7.�ṩ�豸��Ϣ���Զ������豸�ڵ�---����ں�����(������+�豸)
