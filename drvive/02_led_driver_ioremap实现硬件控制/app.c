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
