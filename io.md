
### linux 5大块
+ 文件管理
+ 进程管理
+ 设备管理
+ 内存管理
+ 网络管理

### 分析printf的底层
1. 在用户空间 printf 执行
通过write 显示器 进入内核空间
'write(1,"hello\n",128)'  print的底层
内核空间 操作显示器的相关寄存器 写数据至硬件
行缓存，若没有换行符，则

2. touch 命令的底层就是 
'open("name",O_CREAT|O_RDWR,0777)'
0777表示希望创建的这个文件的权限 
由于umask的掩码是0022
所以最终创建的文件权限为：0755
想要创建文件的时候不受掩码的影响，可以设置umask 为0000  'umask 0000'

3. inode号：
    + 每个创建的文件就会对应1个inode号，文件不一样inode 就不一样
4. open 的flag
    + O_APPEND：追加方式打开，原内容保留
    + O_TRUNC:打开的时候会把已经存在的内容删除
    + O_EXCL: 使用的时候与O_CREAT一起用
    O_CREAT|O_EXCL 若文件已存在 则打开报错；若文件不存在，则可以正确的创建并打开

5. 文件io read write是无缓存的，调用之后就会直接发生io操作，写入文件
标准io库函数，带缓存，所以copy文件的时候 会减少io操作  速度更快

printf 如果没有\n，不能直接打印出来，因为是标准输出流是 行缓存的，不遇到\n 不更新

fflush(stdout);  刷新 FILE 指针

### 读写缓存
对于文件io 没缓存
对于标准io 分三种
全缓存 行缓存 无缓存

重定位函数：
文件io：lseek
标准io：fseek  rewind

标准io：fgets  fputs
fflush  gets puts fprintf sprintf
fgetc  fputc
feof  ferror  clearerr

fgets 读到文件结尾或读错 返回null
fgetc 读到文件结尾或读错 返回EOF
怎么区分是读错了还是读到文件结尾呢？
feof  专门用来判断是否读到文件结尾

### 目录操作
struct dirent* mystr=malloc(sizeof(struct dirent));
DIR*mydir=opendir("testdir");
//一个循环遍历目录中的一个子节点信息
while((mystr = readdir(mydir))!=NULL)
{
    //inode号
    printf("dino=%d\n",mystr->d_ino);
    printf("dino=%d\n",mystr->d_off);
    printf("dino=%d\n",mystr->d_reclen);
    //文件类型
    printf("dino=%d\n",mystr->d_type);
    //文件名
    printf("dino=%s\n",mystr->d_name);
}
return 0;

opendir 只能打开
mkdir   创建目录 ("./ss",0777) 会被掩码掩掉  类似文件创建
readdir 读目录
rewinddir  调整位置指针
telldir   类似ftell 返回位置指针
seekdir   类似fseek 设置位置指针
closedir   关闭目录


### 指针是否需要申请空间呢？
如果函数返回值是个结构体指针，则接收的指针不需要申请空间直接接收就好
struct deient *mystr = readdir(mydir);
因为在readdir内部实现中已经为这个指针指向的区域分配空间了。

什么情况下必须分配空间呢？
直接定义一个指针，哪里都没有去指向，则指针指向的位置是NULL的，不能访问，也不能赋值。
如果可以把指针指向某个地方，则就可以通过prt->去访问该指向的区域的数据。
因为该指向的区域肯定也已经分配空间了，不然该区域就不可能有数据


### goto实现 判断错误循环
start:
    ret=dosth();
    if(ret==null)
    {
        goto start;
    }
    printf("do something ok");

## 进程线程
A fork出来的进程B  两个进程的唯一区别  pid不一样

用户空间不可能实现进程间通信
内核空间才可以进程进程间通信
两个进程都可以访问内核
若A B 两个进程都对内核中同一对象进行操作，即可实现进程间通信

对于同一个进程创建的多个子线程，可以直接在用户空间进程线程间通信--通过全局句柄

进程间通信方法： 都是基于文件io的
无名管道、有名管道
信号通道：信号的发送接收处理
ipc对象：共享内存、消息队列、信号灯
socket  本地通信
### 管道
其实是个队列缓存 buf
写端 进行入队；读端 进行出队
1. 管道特点：
    + 管道中的数据，读完就删除了
    + 若管道是空的，读数据时会阻塞-----S+ 状态
    + 管道创建于内存中，进程结束 则管道就会被释放
    + 假设管道1000，write 1500字节，写满同样会发生写阻塞-----S+ 状态
    + 管道大小为60000 多一点
#### 无名管道
1. pipe 只能用于有亲缘关系的进程间通信
因为pipe只调用1次 产生的fd[0] fd[1]必须在一个程序中使用
只要有亲缘关系 都能访问到fd,别的进程重新pipe 产生的是别的fd
创建成功返回两个文件描述符
2. fd[0]=3  fd[1]=4;//因为每个进程打开就会默认创建0 1 2共三个文件描述符

#### 有名管道
mkfifo("./myfifo",0777);
不占用磁盘空间，只有文件节点的文件类型：
1. 有名管道 --通信用的
2. 字符设备 --设备，是在外面挂载的
3. 块设备   --设备，挂载在系统外
4. 套接字   --通信用的

### 信号通道
kill -l 查看内核 可以发送多少种信号
kill 是系统调用，虽然是用户空间调用的cmd,其实底层是内核空间发的信号
怎么用code实现kill 9 pid
kill(pid,sig)  函数名也是kill
raise(sig) 发信号给自己，也可以写成 kill(getpid(),sig)  实现发信号给自己
alarm(5);  5s后给当前进程发送alarm信号
pause();   类似sleep() 是当前进程处于睡眠态，但是pause的休眠只要外界打断就会继续运行，sleep休眠几秒就自动继续运行了

ctrl+z：STOP信号---暂停，不是终止
sigchild信号：子进程exit时会给父进程发此信号，父进程 可以wait处理，否则不知道多会回收，造成阻塞

fork返回值pid>0的表示父进程，pid为子进程进程号
         pid==0的表示子进程，getpid() 才能得到自己的进程号

if(waitpid(pid,NULL,WNOHANG) == 0)//子进程未退出，无法回收
{
    kill(pid，9);  杀死子进程
}
wait(NULL);  父进程回收子进程资源的函数

WNOHANG 不阻塞
0 阻塞

waitpid(pid,NULL,0); 以阻塞方式回收，等子进程退出 自己再回收，否则一直等

即使子进程exit退出，若父进程不回收子进程资源，子进程仍会变成僵尸进程，直到父进程退出 ，子进程交给1号进程去回收

#### signal
signal(signum,sig_handle) 是信号注册函数，signum:信号类型
sig_handle:信号处理函数

signal(14,SIG_IGN);  忽略对14号信号的处理，不执行默认的终止进程的操作
signal(14,SIG_DFL);  设置为默认的处理方式
```c
优雅的解决子进程的回收，否则子进程退出不回收会产生僵尸
void huishou(int signum)
{
    wait(NULL);
}
int main()
{
        int i;
        int pid =fork();
        if(pid>0)
        {
            signal(17,huishou);//注册对17的处理
            for(int i=0;i<100;i++)
            {
                printf("in parent i=%d\n",i);
                sleep(1);
            }
        }else if(pid==0)
        {
            sleep(5);
            exit(0);//exit的时候 默认向附近车工发出SIGCHLD 17信号
        }
}
```

### 共享内存
ipcs -m/-q/-s  查看 共享内存、消息队列、信号灯
ipcrm -m/-q/-s id 删除 对应ipc类型对象id
数据一旦写入，永远存在（除非自己删除）
管道，写入后，读一下就没了
1. 创建共享内存
    int shm_get(key,size,flag) 0777
    key:IPC_PRIVATE(即0) 或 ftok的返回值
    IPC_PRIVATE只能实现有亲缘关系的进程间通信，ftok创建的key才能实现无亲缘关系的进程间通信
    返回:共享内存标识ID-文件描述符

    通过key值创建的时候需要IPC_CREATE 
    shm_get(key,size,IPC_CREATE|0777);
    通过IPC_PRIVATE创建的时候不需要CREATE
2. 删除内核空间申请的共享内存
    shmctl(shmid,IPC_RMID,NULL);
3. shmat(shmid,void *shmaddr, shmflg) 把内核空间申请的地址映射到用户空间，shmaddr传NULL
4. shmdt(const void *shmaddr);删除用户空间映射的指针

#### 父子进程读写共享内存demo--单向通讯
```c
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <string.h>

void myfun(int signum)
{
    //调用pause的时候程序被暂停，之后收到信号程序进来，再 return 出去后，就到pause之后了
    return;//will return after pause()
}

int main()
{
    int i;
    int shmid = shmget(IPC_PRIVATE,128,0777);
    char *prt=NULL;
    //好像在fork前映射好就可以 TODO：实验是ok的
    //prt=shmat(shmid,NULL,0);
    int pid =fork();
    if(pid>0)
    {
        //父进程根据shmid映射内核空间到用户空间
        prt=shmat(shmid,NULL,0);
        signal(SIGUSR2,myfun);
        while(1)
        {
            printf("parent start write\n");
            fgets(prt,128,stdin);//form stdin input data
            kill(pid,SIGUSR1);//tell child to read
            pause();//wait child to read
        }
    }else if(pid==0)
    {
        //子进程根据shmid映射内核空间到用户空间
        prt=shmat(shmid,NULL,0);
        signal(SIGUSR1,myfun);
        while(1)
        {
            pause();//wait parent write
            printf("child read data = %s\n",prt);
            kill(getppid(),SIGUSR2);//tell parent read over
        }
    }
    shmdt(prt);
    shmctl(shmid,IPC_RMID,NULL);
}

```


#### 非亲缘关系的进程间通信
怎么让两个进程知道对方进程的pid，通过发信号 同步消息
```c
writer.c
void myfun(int signum)
{
    return;//will return after pause()
}

int main()
{
    int i;
    int key=ftok("./",1);
    int shmid = shmget(key,128,IPC_CREAT|0777);
    char *prt=NULL;
    char string[128]={0};
    prt=shmat(shmid,NULL,0);//在当前进程根据shmid 重新映射内核地址到当前进程的用户空间
    signal(SIGUSR2,myfun);
    //itoa(getpid(),string,10);  can use sprintf replace some linux only has atoi,no itoa
    sprintf(string,"%d",getpid());
    strcpy(prt,string);//tell reader: cur pid,直接往共享内存写入write进程id 暂停等read进程读了后 给自己发信号
    printf("in write writer pid=%d\n",getpid());
    pause();//wait to get reader pid
    int readpid = atoi(prt);//获取read进程id
    printf("in write reader pid=%d\n",readpid);

    while(1)
    {
        printf("writer start write\n");
        fgets(prt,128,stdin);//form stdin input data
        kill(readpid,SIGUSR1);//tell reader to read
        pause();//wait read to read
    }
}
```


```c
read.c
void myfun(int signum)
{
    return;//will return after pause()
}

int main()
{
    char string[128]={0};
    int i;
    int key=ftok("./",1);
    int shmid = shmget(key,128,IPC_CREAT|0777);
    char *prt=NULL;
    prt=shmat(shmid,NULL,0);
    printf("in read curid=%d\n",getpid());
    signal(SIGUSR1,myfun);
    int writerpid = atoi(prt);//get writer pid；获取write进程id，写入自己id，发信号
    printf("in read writer pid=%d\n",writerpid);
    sprintf(string,"%d",getpid());
    strcpy(prt,string);//tell writer:cur pid
    kill(writerpid,SIGUSR2);
    while(1)
    {
        pause();//wait writer to write and kill 
        printf("read data=%s",prt);
        kill(writerpid,SIGUSR2);//tell writer read over
    }
}
```

#### 优化 非亲缘关系的进程通信
struct  MemBuf{
    int pid;
    char buf[124];//因为映射的共128字节  id占4字节
}; 优化后不需要多次的int string转换
```c
writer.c
int main()
{
    int i;
    int key=ftok("./",1);
    int shmid = shmget(key,128,IPC_CREAT|0777);

    char string[128]={0};
    struct  MemBuf *prt=(struct  MemBuf *)shmat(shmid,NULL,0);//在当前进程根据shmid 重新映射内核地址到当前进程的用户空间
    signal(SIGUSR2,myfun);
    prt->pid=getpid();  //tell reader: cur pid,直接往共享内存写入write进程id 暂停等read进程读了后 给自己发信号
    pause();//wait to get reader pid
    int readpid = prt->pid;//获取read进程id
    printf("in write reader pid=%d\n",readpid);
    while(1)
    {
        printf("writer start write\n");
        fgets(prt->buf,124,stdin);//form stdin input data 124byte
        kill(readpid,SIGUSR1);//tell reader to read
        pause();//wait read to read
    }
}
```


```c
read.c
int main()
{
    char string[128]={0};
    int i;
    int key=ftok("./",1);
    int shmid = shmget(key,128,IPC_CREAT|0777);
    struct  MemBuf *prt=(struct  MemBuf *)shmat(shmid,NULL,0);
    signal(SIGUSR1,myfun);
    int writerpid = prt->pid;//get writer pid；获取write进程id，写入自己id，发信号
    printf("in read writer pid=%d\n",writerpid);
    prt->pid=getpid();//tell writer:cur pid
    kill(writerpid,SIGUSR2);
    while(1)
    {
        pause();//wait writer to write and kill
        printf("read data=%s",prt->buf);
        kill(writerpid,SIGUSR2);//tell writer read over
    }
}
```



### 消息队列
+ msgget 共享内存需要指定映射大小，消息队列可以随时插入，不需要指定大小
+ 读完队头的数据就没了，再读就读的下一个数据
+ msgctl 
    + (msgid,IPC_RMID,NULL)  删除消息队列
    + 获取 修改 消息队列属性
+ msgsnd 队列的入队
    + (msgid,struct msgbuf*,size,flags)必须传结构体，通过mtype区分消息
        + size:消息正文的长度 
        + flags:IPC_NOWAIT 非阻塞，消息没发完立即返回
                     0：阻塞  直到数据发完才返回
        + struct msgbuf{
            long mtype; //消息类型
            char mtext[N]; //消息正文
        }
        + return  成功0 出错-1
+ msgrcv 队列的出队
    + (msgid,void*buf,size,type,flag)
        + buf: 读到哪里去
        + 想读多少个
        + type: 读哪种类型数据
        + flag：阻塞 不阻塞
        + 返回值：实际读到的正文长度

#### code
```c
struct  MsgBuf{
    long type;
    char voltage[124];
    char curr[124];
};

int main()
{
    int i;
    int key=ftok("./",1);
    int msgid=msgget(key,IPC_CREAT|0777);
    struct  MsgBuf sendbuf={
        .type=100,
        .voltage="i am voltage",
        .curr="i am curr"
    };
    //发哪里的数据 从sendbuf的第一个voltage开始发，发多少字节
    msgsnd(msgid,(void*)&sendbuf,sizeof(struct  MsgBuf)-4,0);
    //发两次 则可以收两次
    msgsnd(msgid,(void*)&sendbuf,sizeof(struct  MsgBuf)-4,0);

    printf("sned over\n");
    struct  MsgBuf readbuf;
    int rcvcnt;
    rcvcnt = msgrcv(msgid,(void*)&readbuf,sizeof(struct  MsgBuf)-4,100,0);
    printf("first read recv 100,rcvcnt=%d ==vol=%s,curr=%s\n",rcvcnt,readbuf.voltage,readbuf.curr);
    memset(&readbuf,0,sizeof(struct  MsgBuf));
    rcvcnt = msgrcv(msgid,(void*)&readbuf,sizeof(struct  MsgBuf)-4,100,0);
    printf("secnod recv 100,rcvcnt=%d ==vol=%s,curr=%s\n",rcvcnt,readbuf.voltage,readbuf.curr);

//  上面已经rcv了2次  由于只有2个节点数据  所以这里会阻塞
    msgrcv(msgid,&readbuf,124,200,0);
    printf("recv 200,rcvcnt=%d ==vol=%s,curr=%s",rcvcnt,readbuf.voltage,readbuf.curr);
    //删除消息 队列
    msgctl(msgid,IPC_RMID,NULL);
}
```

```c
//两个进程间通信 由于snd rcv函数 可以阻塞，所以不需要发信号通知对方

ser.c
int main()
{
    int key=ftok("./",1);
    int msgid=msgget(key,IPC_CREAT|0777);
    struct  MsgBuf sendbuf={
            .type=100,
            .voltage="i am voltage",
            .curr="i am curr" };
    while(1)
    {
        printf("input vol and cur\n");//键盘输入要snd data
        fgets(sendbuf.voltage,124,stdin);
        fgets(sendbuf.curr,124,stdin);
        msgsnd(msgid,&sendbuf,sizeof(struct  MsgBuf)-4,0);
    }
    msgctl(msgid,IPC_RMID,NULL);
    system("ipcs -q");
}

cli.c
int main()
{
    int key=ftok("./",1);
    int msgid=msgget(key,IPC_CREAT|0777);
    struct  MsgBuf readbuf;
    int rcvcnt;
    while(1)
    {
        memset(&readbuf,0,sizeof(struct  MsgBuf));
        rcvcnt = msgrcv(msgid,&readbuf,sizeof(struct  MsgBuf)-4,100,0);//以阻塞收数据，读不到就休眠
        printf("first read recv 100,rcvcnt=%d ==vol=%s,curr=%s\n",rcvcnt,readbuf.voltage,readbuf.curr);
    }
    msgctl(msgid,IPC_RMID,NULL);
    system("ipcs -q");
}
```

```c
//两个进程都读写  用多进程实现 1个读 1个写
ser.c 发的data 类型是100，收的数据类型是200
int main()
{
    int i;
    int key=ftok("./",1);
        int msgid=msgget(key,IPC_CREAT|0777);
        struct  MsgBuf sendbuf={
            .type=100,
            .voltage="i am voltage",
            .curr="i am curr"
        };
        struct  MsgBuf recvbuf;
        int pid=fork();
        if(pid>0)//parent write
        {
         while(1)
         {
            printf("input vol and cur\n");
            fgets(sendbuf.voltage,124,stdin);
            fgets(sendbuf.curr,124,stdin);
            msgsnd(msgid,&sendbuf,sizeof(struct  MsgBuf)-4,0);
         }
        }else if(pid==0)//child read
        {
            while(1)
            {
                memset(&recvbuf,0,sizeof(struct  MsgBuf));
                msgrcv(msgid,&recvbuf,sizeof(struct  MsgBuf)-4,200,0);
                printf("rcv type=200,vol=%s,curr=%s\n",recvbuf.voltage,recvbuf.curr);
            }
        }
        msgctl(msgid,IPC_RMID,NULL);
        system("ipcs -q");
}

cli.c 发的data 类型是200，收的数据类型是100
int main()
{
    int i;
    int key=ftok("./",1);
        int msgid=msgget(key,IPC_CREAT|0777);
        struct  MsgBuf sendbuf={
            .type=200,
            .voltage="i am voltage",
            .curr="i am curr"
        };
        struct  MsgBuf recvbuf;
        int pid=fork();
        if(pid>0)//parent write
        {
         while(1)
         {
            printf("input cli type=200  vol and cur\n");
            fgets(sendbuf.voltage,124,stdin);
            fgets(sendbuf.curr,124,stdin);
            msgsnd(msgid,&sendbuf,sizeof(struct  MsgBuf)-4,0);
         }
        }else if(pid==0)//child read
        {
            while(1)
            {
                memset(&recvbuf,0,sizeof(struct  MsgBuf));
                msgrcv(msgid,&recvbuf,sizeof(struct  MsgBuf)-4,100,0);
                printf("rcv type=100,vol=%s,curr=%s\n",recvbuf.voltage,recvbuf.curr);
            }
        }
        msgctl(msgid,IPC_RMID,NULL);
        system("ipcs -q");
}
```

### 信号灯--信号量的集合
+ 与posix信号量的区别：
    + 信号量是一种数字量，可以占有sem_wait，释放sem_post
    + 是信号量的集合（含有多种信号量）
1. 函数
    + semget(key,nsems,flag)
        + key:IPC_PRIVATE 只能用于有亲缘关系的进程间
              ftok 创建的key 可以用于任何进程间
        + nsems:包含的信号种类 
        + 对信号灯集合的访问权限
    + semctl(semid,semnum,IPC_RMID,NULL);
        + 当cmd为IPC_RMID时，semnum与最后的属性都没用
        + 当cmd为SETVAL 时可以通过semnum设置信号灯种类
2. 用posix 信号量实现多线程同步
```c

```