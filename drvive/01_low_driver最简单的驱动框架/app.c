#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>

int main(int argc,char **argv)
{
    int fd;
    char buf[1024];
    int len;
    if(argc<2)
    {
        printf("usage:%s -w<strning>\n",argv[0]);
        printf("      %s -r\n",argv[0]);
        return -1;
    }
    fd=open("/dev/hello",O_RDWR);
    if((0==strcmp(argv[1],"-w")) &&(argc==3))
    {
        len=strlen(argv[2]+1);
        len=len<1024?len:1024;
        write(fd,argv[2],len);
    }else
    {
        len=read(fd,buf,1024);
        buf[1023]='\0';
        printf("app read:%s",buf);
    }
    close(fd);
}
//调试打印的内核信息
//写入测试
// [39446.827345] /root/drv/low_driver/hello_drv.c hello_init line:74
// [39457.421507] /root/drv/low_driver/hello_drv.c hello_drv_open line:51
// [39457.422680] /root/drv/low_driver/hello_drv.c hello_drv_write line:45
// [39457.423816] /root/drv/low_driver/hello_drv.c hello_drv_close line:56
//读出测试
// [39476.218194] /root/drv/low_driver/hello_drv.c hello_drv_open line:51
// [39476.219610] /root/drv/low_driver/hello_drv.c hello_drv_read line:38
// [39476.221019] /root/drv/low_driver/hello_drv.c hello_drv_close line:56