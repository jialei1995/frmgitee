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

## 进程

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
sem_t sem;
void * fun(void* str)
{
	int i=0;
	sem_wait(&sem);//获得信号量之前程序 子线程阻塞等待
	for(i=0;i<5;i++)
	{
		printf("in %s thread,i=%d\n",(char*)str,i);
		sleep(1);
	}
}
int main()
{
    int i;
	char str[]="hello linux";
	pthread_t tid;
	//secnod para:0-thread 1-process
	//third para:sem init val
	sem_init(&sem,0,0);  //初始化为0，主线程post后信号量变成1 则子线程可以占有
	int ret=pthread_create(&tid,NULL,fun,(void*)str);

	for(i=0;i<10;i++)
	{
		sleep(1);
		printf("in main i=%d\n",i);
	}
	sem_post(&sem);
	while(1);
}
```

用信号灯集替换上面的posix 信号量怎么实现？

```c
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>   
#include <sys/sem.h>
#include <semaphore.h>
//sem_t sem;
int semid;//define 信号灯集合 

union semun {
    int  val;                //* Value for SETVAL 
    struct semid_ds *buf;    // Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  // Array for GETALL, SETALL */
    struct seminfo  *__buf;  // Buffer for IPC_INFO (Linux-specific) */
};

struct sembuf mysembuf;
void * fun(void* str)
{
	int i=0;
//	sem_wait(&sem);
	mysembuf.sem_op=-1;//p 操作 消耗
	semop(semid,&mysembuf,1);//1表示 只操作1个信号
	for(i=0;i<5;i++)
	{
		printf("in %s thread,i=%d\n",(char*)str,i);
		sleep(1);
	}
}
int main()
{
    int i;
	char str[]="hello linux";
	pthread_t tid;
    union semun mysemun;//定义某个信号的属性

	//secnod para:0-thread 1-process
	//third para:sem init val
	//sem_init(&sem,0,1);  
	semid=semget(IPC_PRIVATE,3,0777);
	mysemun.val=0;
	//semget了3个信号量，这里只初始化第0个信号量，设置值为0
	semctl(semid,0,SETVAL,mysemun);

	mysembuf.sem_num=0;//要操作第0号信号量--看sem_op是增加还是减少
	mysembuf.sem_flg=0;//设为阻塞操作
	int ret=pthread_create(&tid,NULL,fun,(void*)str);

	for(i=0;i<10;i++)
	{
		sleep(1);
		printf("in main i=%d\n",i);
	}
	//sem_post(&sem);
	mysembuf.sem_op=1;
	semop(semid,&mysembuf,1);//v 操作
	while(1);
}

```

## 线程
目的：合理利用资源，提高cpu的效率
并发:时间片切换，让多个任务 切换着运行
并行:多个cpu 每个cpu管理一个任务，真正意义的 多个任务同时运行
同步:任务彼此有依赖，不应该同时发生。同步就是阻止同时发生，使任务按照顺序执行
异步:强调任务的独立性

多线程不一定多个处理器

线程id只是在创建它的进程中有效，一旦当前进程结束，则线程id就会被销毁。

#### 控制

终止  	连接(回收)  	取消	发信号	清除

#### 同步

互斥量		读写锁		条件变量

### 主线程
+ main函数内的线程就是主线程
+ 主线程的特殊性在于:一旦main返回，则进程结束，进程内的所有线程都会结束；
    为了让主线程等待别的线程都结束自己再退出，可以在主线程中调用`pthread_exit()`去结束主线程
+ 主线程接收参数的方式通过(int argc,char*argv[]),其他线程只能由一个参数(void*)
+ 主线程在默认堆栈运行，这个堆栈可以增长到足够的长度，普通线程的堆栈是受限的，一旦溢出就会报错

### demo
```c
void * fun(void* str)
{
//解析线程的入参的时候，先得((struct Stu*)ptr) 将指针包起来强制转换类型后，再去->访问成员。否则无法访问
printf("in  thread,id=%d,name=%s\n",((struct Stu*)str)->id,((struct Stu*)str)->name);
}
pthread_create(&tid,NULL,fun,(void*)&stu);
```

### 线程回收
+ 线程pthread_create的时候 可以传入分离属性；
+ 分离属性：并不会影响线程的运行，仅仅通知当前系统，该线程结束的时候资源可以回收
+ 没有分离属性的线程，终止时使用的资源不回被回收，创建线程时默认是非分离的
+ 线程结束的时候，自己分配的动态内存，mmap，ipc对象，可以由其他线程或自己去释放，若要其他线程去释放，需要自己解锁互斥量---否则自己锁着，别人没法帮你释放

###

+ 线程中调用exit，_exit ,_Exit 会导致整个程序退出  是危险的
+ 正确的退出方式
    + return 退出，返回值就是线程的退出码
    + 线程可以被同一进程中的其他线程取消
    + 调用pthread_exit(void*ret)  ret就是线程的退出码  与return类似


### 连接线程 pthread_join(pthread_t thread, void **retval)
+ 调用pthread_join的线程会一直阻塞，直到指定的线程pid 调用pthread_exit，return，或被取消
+ 如果指定的线程被取消，则retval被置为PTHREAD_CANCLED
+ 调用此函数会使 指定的线程处于分离态，如果指定线程已经处于分离态，则调用会失败(自己测试并没有失败)
+ pthread_detach 可以分离一个线程，线程也可以自己分离自己
```c
void * fun1(void* str)
{
    printf("in thread,id=%d\n",pthread_self());
    return (void*)1;
}
void * fun2(void* str)
{
    printf("in thread2,id=%d\n",pthread_self());
    int ret = pthread_detach(pthread_self());
    if(ret!=0)
    {printf("thread2 detach fail\n"); }
    //sleep(1); pthread_exit(3); 一样
    return (void*)3;
}
int main()
{
    pthread_t tid1,tid2;
    pthread_create(&tid1,NULL,fun1,NULL);
    pthread_create(&tid2,NULL,fun2,NULL);
    void * ret1;    void * ret2;
    printf("join1 ret=%d\n",pthread_join(tid1,&ret1));
    printf("thread 1 return %d\n",(int*)ret1);
    printf("join2 ret=%d\n",pthread_join(tid2,&ret2));
    printf("thread 2 return %d\n",(int*)ret2);
}
打印：
join1 ret=0
thread 1 return 1
in thread2,id=-2088839424
join2 ret=0
thread 2 return 3
```

### 线程取消
+ pthread_cancel (pid) 向指定的线程id 发取消信号，信号发送成功 并不意味目标tid 被取消
+ 设置当前线程对取消信号的反应
    pthread_setcancelstate(int state,int *oldstate)
    state:PTHREAD_CANCEL_ENABLE(收到信号取消) PTHREAD_CANCEL_DISABLE(收到信号忽略)
    oldstate如果不为NULL，则存入原来的cancel状态以便恢复
+ 立即取消/延时取消
    pthread_setcanceltype(type,*oldtype)
    type:PTHREAD_CANCEL_DEFERRED(延时至下个取消点再退出)  PTHREAD_CANCEL_ASYNCHRONOUS(立即退出)
    oldtype如果不为NULL，则存入原来的cancel取消状态以便恢复

+ 取消点是什么
    man pthreads 搜索： Cancellation points
    很多系统调用都是取消点 一般用这个 pthread_testcancel()  没啥意义就是作为取消点的

```c
void * fun2(void* str)
{
    printf("in thread2,id=%d\n",pthread_self());
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);//设置可以被取消
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
    sleep(3);
    pthread_exit((void*)4);
}
int main()
{
    pthread_t tid2;
    pthread_create(&tid2,NULL,fun2,NULL);
    void * ret2;
    int cancelret=pthread_cancel(tid2);
    pthread_join(tid2,&ret2);
    printf("cancelret=%d\n",cancelret);
    printf("thread 2 exit code %d\n",(int*)ret2);
}

in thread2,id=-1529141504
cancelret=0
join ret=0
thread 2 exit code -1  PTHREAD_CANCELED
```

```c
void *thread_fun(void*agr)
{
    int setstate;
    setstate=pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
    if(setstate!=0)printf("set cancel disable failed\n");
    printf("set disable ok\n");
    sleep(4);
    printf("can run to here\n");
    //pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);  若加入立即取消，则后面都打印不出来了
    setstate=pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);//默认线程是延时取消属性
    
    if(setstate!=0)printf("set cancel enable failed\n");
    printf("first cencel point\n");//printf 函数 本身就是取消点 到了取消点，执行完就退出
    printf("second cencel point\n");//printf 本身就是取消点
    return (void*)20;
}
int main()
{
    pthread_t tid;
    int err;
    err=pthread_create(&tid,NULL,thread_fun,NULL);
    if(err!=0)printf("create thread failed\n");
    sleep(2);
    int cval=pthread_cancel(tid);
    if(cval!=0)printf("cancel tid fail\n");

    void*jexitcode;
    int jret=pthread_join(tid,&jexitcode);
    if(jret!=0)printf("join err\n");
    printf("join exit code=%d\n",(int)jexitcode);
    return 0;
}
set disable ok
can run to here
first cencel point
join exit code=-1
```

### 线程发信号
`pthread_kill(tid,sig) `  向指定的id发信号，若线程内代码不处理，则按照信号默认处理影响整个进程
sigaction  实现收到信号的处理，sig不是0  都要处理
sig=0   保留信号，并没有发信号，用来判断线程是否活着

```c
void *thread_fun(void*agr)
{
    printf("woshi thread\n");
    return (void*)20;
}
int main()
{
    pthread_t tid;
    int err;
    err=pthread_create(&tid,NULL,thread_fun,NULL);
    if(err!=0)printf("create thread failed\n");
	
    sleep(1);//延时一下再去 kill 子线程，它已经退出了 则kill失败，返回 ESRCH，子线程不存在
    int s=pthread_kill(tid,0);//sig==0 时只是 判断子线程是否还活着
    if(s!=0)
    {
        printf("pthread_kill fail\n");
    }

    void*jexitcode;
    int jret=pthread_join(tid,&jexitcode);
    if(jret!=0)printf("join err\n");
    printf("join exit code=%d\n",(int*)jexitcode);
    return 0;
}


//子线程不退出
void *thread_fun(void*agr)
{
    sleep(1);
    printf("woshi thread\n");
    return (void*)20;
}
int main()
{
    pthread_t tid;
    int err;
    err=pthread_create(&tid,NULL,thread_fun,NULL);
    if(err!=0)printf("create thread failed\n");
	
    int s=pthread_kill(tid,SIGQUIT);//SITQUIT杀死子线程，子线程在阻塞，所以能杀死---一旦执行成功，整个进程都会退出，后面的打印也无法执行了
    if(s!=0)
    {
        printf("pthread_kill fail\n");
    }

    void*jexitcode;
    int jret=pthread_join(tid,&jexitcode);
    if(jret!=0)printf("join err\n");
    printf("join exit code=%d\n",(int*)jexitcode);
    return 0;
}

```

#### 信号的处理

`int sigaction(int signum, const struct sigaction *act,  struct sigaction *oldact);`

给特定信号设置处理函数，通过参数act

`act.sa_mask`  信号屏蔽字

`act.sa_handler`  信号集处理程序

``

```c
int sigemptyset(sigset_t *set);  清空信号集
int sigfillset(sigset_t *set);   将所有信号加入信号集
int sigaddset(sigset_t *set, int signum);  增加一个信号到信号集
int sigdelset(sigset_t *set, int signum);  删除一个信号到信号集

多线程的信号屏蔽,参考进程的信号屏蔽(sigprocmask)
int pthread_sigmask(int how, const sigset_t *set, sigset_t *oldset); `int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
```



demo

```c
void sig_handler1(int arg)
{
    printf("thread1 sig_handler\n");
    return ;
}

void sig_handler2(int arg)
{
    printf("thread2 sig_handler\n");
    return ;
}

void *thread_fun1(void*arg)
{
    printf("new thread1\n");
    struct sigaction act;
    memset(&act,0,sizeof(act));
    sleep(1);
    sigaddset(&act.sa_mask,SIGQUIT);//增加对信号SIGQUIT的处理-怎么处理后面设置
    act.sa_handler=sig_handler1;//调用这个处理函数
    sigaction(SIGQUIT,&act,NULL);//注册 使设置生效 按照最后执行的sigaction处理
    //设置屏蔽掩码--屏蔽该线程对信号的的处理
    pthread_sigmask(SIG_BLOCK,&act.sa_mask,NULL);
    sleep(3);
}

void *thread_fun2(void*arg)
{
    printf("new thread2\n");
    struct sigaction act;
    memset(&act,0,sizeof(act));
    sigaddset(&act.sa_mask,SIGQUIT);//增加对信号SIGQUIT的处理-怎么处理后面设置
    act.sa_handler=sig_handler2;//调用这个处理函数
    sigaction(SIGQUIT,&act,NULL);//注册 使设置生效
    //设置屏蔽掩码
    //pthread_sigmask(SIG_BLOCK,&act.sa_mask,NULL);
    sleep(3);
}

int main()
{
    pthread_t tid1,tid2;
    int err,s;
    err=pthread_create(&tid1,NULL,thread_fun1,NULL);
    if(err!=0)
    {
        printf("create thread1 fial\n");
    }
    err=pthread_create(&tid2,NULL,thread_fun2,NULL);
    if(err!=0)
    {
        printf("create thread2 fial\n");
    }
    sleep(2);//等待线程 设置 属性完成
    s=pthread_kill(tid1,SIGQUIT);
    if(s!=0)
    {
        printf("send kill to thread1 fail\n");
    }
    s=pthread_kill(tid2,SIGQUIT);
    if(s!=0)
    {
        printf("send kill to thread2 fail\n");
    }

    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    return 0;
}

所有线程中对信号的处理了按照sigaction最后执行时传入的哪个act，则处理函数就跳到对应sig_handler之中
所以即使屏蔽了线程1的信号处理，线程1就不会处理SIGQUIT信号，只要sigaction最后在线程1中注册，则线程2，还是会调用到sig_handler1中.
```



#### 线程的清理函数

`pthread_cleanup_push(handler,void*args)` //注册处理程序 args是清理函数的入参

`pthread_cleanup_pop(int execute)`//清理处理程序

 这两个函数要成对出现，否则编译无法通过

执行以下操作时才会调用清理函数：

+ 线程调用pthread_exit 退出  && pop非零
+ 响应线程取消请求
+ 用非零参数调用`pthread_cleanup_pop`



demo

```c
证明用`pthread_cleanup_pop`退出时必须为非0 才能调用，并且只能调用一次，先执行后push进去的回调函数
push与pop必须成对出现
void clean1(void* arg)
{
        printf("call clean1 %d\n",(int*)arg);
}
void clean2(void* arg)
{
        printf("call clean2 %d\n",(int*)arg);
}
void clean3(void* arg)
{
        printf("call clean3 %d\n",(int*)arg);
}
void clean4(void* arg)
{
        printf("call clean4 %d\n",(int*)arg);
}
void *thread_fun1(void*arg)
{
        pthread_cleanup_push(clean1,(void*)0);
        pthread_cleanup_push(clean2,(void*)1);

        pthread_cleanup_pop(1);
        pthread_cleanup_pop(0);
        return(void*)1;
}
void *thread_fun2(void*arg)
{
        pthread_cleanup_push(clean3,(void*)0);
        pthread_cleanup_push(clean4,(void*)1);

        pthread_cleanup_pop(0);
        pthread_cleanup_pop(0);
        return(void*)2;
}


int main()
{
    pthread_t tid1,tid2;
    int err,s;
    err=pthread_create(&tid1,NULL,thread_fun1,NULL);
    if(err!=0)
    {
        printf("create thread1 fial\n");
    }
    err=pthread_create(&tid2,NULL,thread_fun2,NULL);
    if(err!=0)
    {
        printf("create thread2 fial\n");
    }
    sleep(2);//等待线程 设置 属性完成
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    return 0;
}

```

### 线程同步

多个线程同时访问同一个变量，变量值不确定，不受程序控制

#### 互斥量`mutex`

##### 初始化

+ 动态分配的锁，用pthread_mutex_init()  初始化锁
+ 静态分配的锁，直接设为PTHREAD_MUTEX_INITIALIZER 即可
+ 动态分配的锁，释放内存之前要pthread_mutex_destroy

静态初始化：`pthread_mutex_t  mymutex=PTHREAD_MUTEX_INITIALIZER `

动态初始化：``pthread_mutex_t   mymutex`

​                    `pthread_mutex_init(&mymutex,NULL)`    属性不用设为空

##### 加锁解锁

`pthread_mutex_lock(*mutex)`

成功返回0，失败错误码。若已经被锁住，会导致阻塞

`pthread_mutex_trylock(*mutex)`

成功返回0，失败错误码。若已经被锁住，不回阻塞

解锁：

pthread_mutex_unlock   获得锁的线程才能解锁

#### 读写锁

与mutex类似，有更高的并行性，mutex 同一时刻只允许一个线程对其加锁。对于1个全局变量的读取，完全可以多个线程同时操作--只是访问

pthread_rwlock_t  rwlock

读写锁有3种状态：读模式下加锁  写模式下加锁 ，不加锁。

一次只有1个线程能占用写模式下的读写锁。但是多个线程可以同时占用读模式的读写锁

+ 写加锁时，被解锁之前，任何对这个锁进行加锁的线程都会被阻塞
+ 读加锁时，所有试图对其加锁的线程都会获得访问权。
+ 读加锁时，如果线程希望以写模式对其加锁，会阻塞之后的读模式的请求，避免锁长期占用，而写锁达不到请求---所有线程读完了，写锁就能占用了
+ 适合对数据结构读次数>写次数的程序。读加锁时，是以共享的方式锁住的，写模式加锁时，是以独占的模式锁住的

##### 初始化

`pthread_rwlock_init(*rwloc,*attr)`   使用之前必须初始化

`pthread_rwlock_destroy(*rwlock)`       使用之后必须销毁

##### 加锁解锁

`pthread_rwlock_rdlock`     读模式加锁

`pthread_rwlock_tryrdlock`        

`pthread_rwlock_wrlock`     写模式加锁

`pthread_rwlock_trywrlock`     

不管读锁还是写锁 ，锁住之后 处理完数据都要unlock

解锁：

`pthread_rwlock_unlock`     

demo：

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_rwlock_t rwlock;//创建读写锁
long int love;//love 小于520，5个写线程都写一次 然后sleep。写线程sleep  读线程才能抢占cpu--读完后，1s后  再次写线程占用资源去写

void *pth_wr(void *arg)//写操作
{
    int i = (int)arg;//参数类型转化
    while (love <= 520) 
    {
        pthread_rwlock_wrlock(&rwlock);//请求写锁
        printf("write================love = %ld, threadID = %d\n", love += 40, i + 1);//写操作，love每次加40
        pthread_rwlock_unlock(&rwlock);//写锁释放
        sleep(1);
    }
    return NULL;
}
void *pth_rd(void *arg)//读操作
{
    int i = (int)arg;
    while (love <= 520)
    {
        pthread_rwlock_rdlock(&rwlock);//请求读锁
        printf("love = %ld, threadID = %d-------------------- read\n", love, i + 1);
        pthread_rwlock_unlock(&rwlock);//读锁释放
        sleep(1);
    }
    return NULL;
}
int main(void)
{
    pthread_t pth[10];
    int i;
    pthread_rwlock_init(&rwlock, NULL);
    for (i = 0; i != 5; i++)
    {
        pthread_create(&pth[i], NULL, pth_wr, (void *)i);
    }
    for (i = 0; i != 5; i++)

    {
        pthread_create(&pth[5 + i], NULL, pth_rd, (void *)i);
    }
    while (1)
    {
        if (love >= 520)//love  >520   才去回收 否则等待 线程运行
        {
            for (int j = 0; j != 10; j++)
            {
                pthread_join(pth[j], NULL);
            }
            break;
        }
    }
   pthread_rwlock_destroy(&rwlock);
   return 0;
}


结果：开始10个线程5个读  5个写 同时抢占cpu，只有5个写线程能抢到资源，然后写进程都sleep，读进程才能调用去读。1s后，写线程占有资源 又开始写---rwlock的使用方法，需要一个全局变量去限制  根据他  控制线程的写入读出
write================love = 40, threadID = 1
write================love = 80, threadID = 2
write================love = 120, threadID = 3
write================love = 160, threadID = 4
write================love = 200, threadID = 5
love = 200, threadID = 1-------------------- read
love = 200, threadID = 2-------------------- read
love = 200, threadID = 3-------------------- read
love = 200, threadID = 4-------------------- read
love = 200, threadID = 5-------------------- read
write================love = 240, threadID = 1
write================love = 280, threadID = 2
write================love = 320, threadID = 3
write================love = 360, threadID = 4
write================love = 400, threadID = 5
love = 400, threadID = 1-------------------- read
love = 400, threadID = 2-------------------- read
love = 400, threadID = 3-------------------- read
love = 400, threadID = 4-------------------- read
love = 400, threadID = 5-------------------- read
write================love = 440, threadID = 3
write================love = 480, threadID = 4
write================love = 520, threadID = 2
write================love = 560, threadID = 1

```



#### 条件变量

实际运用中 互斥锁经常需要 配合条件变量去使用

解决的问题：

一条生产线有一个仓库，生产者生产或消费者消费时 都需要独占仓库。如果生产者发现仓库慢了，自己不能生产，阻塞线程，无法释放锁。消费者又无法 进入仓库消耗，造成死锁。

需要一种机制：互斥量被锁住后发现当前线程无法完成自己的操作释放互斥量，让其他线程工作

1、轮询，不停的查询需要的条件---------消耗cpu

2、让系统帮你查询条件，使用条件变量`pthread_cond_t`

##### 初始化

+ 静态：`pthread_cond_t cond=PTHREAD_COND_INITIALZER`
+ 动态：`pthread_cond_init(*cond,*attr)`
+ 销毁：pthread_cond_destroy(*cond)

#### demo

```c
#define BUFFER_SIZE     5		//库存大小，最多能存5个产品
#define PRODUCT_CNT             30//总共生产的总数，生产完就停止
struct Product_cons{
    int buffer[BUFFER_SIZE];    //仓库
    pthread_mutex_t lock;               //互斥锁
    int readpos,writepos;               //读写位置
    pthread_cond_t notempty;            //条件变量  非空
    pthread_cond_t notfull;             //条件变量  非满
}cangku;

void init(struct Product_cons*p)//init cangku
{
    pthread_mutex_init(&(p->lock),NULL);
    pthread_cond_init(&(p->notempty),NULL);
    pthread_cond_init(&(p->notfull),NULL);
    p->readpos=0;p->writepos=0;
}

void finish(struct Product_cons*p)//销毁 cangku
{
    pthread_mutex_destroy(&(p->lock));
    pthread_cond_destroy(&(p->notempty));
    pthread_cond_destroy(&(p->notfull));
    p->readpos=0;p->writepos=0;
}

void put(struct Product_cons*p,int data)//生产一个产品到仓库中
{
    pthread_mutex_lock(&p->lock);
    if((p->writepos+1)%BUFFER_SIZE == p->readpos)//仓库满了
    {
        printf("product wait for not full\n");
        pthread_cond_wait(&p->notfull,&p->lock);//这里就会释放锁 ，等条件满足了 再去运行
    }
    //仓库非满  往下执行
    p->buffer[p->writepos]=data;
    p->writepos++;
    if(p->writepos >= BUFFER_SIZE)
        p->writepos=0;
    pthread_cond_signal(&p->notempty);//发信号 当前是非空的状态
    pthread_mutex_unlock(&p->lock);
}

int  get(struct Product_cons*p)//使用一个产品
{
    int data;
    pthread_mutex_lock(&p->lock);
    if(p->readpos == p->writepos)//仓库是空的 没法消费
    {
        printf("consumer wait for not empty\n");
        pthread_cond_wait(&p->notempty,&p->lock);
    }
    //仓库里面有东西可以消费
    data=p->buffer[p->readpos];
    p->readpos++;
    if(p->readpos >= BUFFER_SIZE)
        p->readpos=0;
    pthread_cond_signal(&p->notfull);//发信号 当前是非满的状态
    pthread_mutex_unlock(&p->lock);
        return data;
}

//子线程---生产
void*  product(void*data)
{
    int n;
    for(n=0;n<=PRODUCT_CNT;n++)
    {
        sleep(1);
        printf("product the %d product...\n",n);
        put(&cangku,n);
        printf("put the %d ok\n",n);
    }
    return NULL;
}

//子线程----消费
void* consume(void*data)
{
        int num=0;
    while(1)
    {
        sleep(2);
        printf("consume ...\n");
        num=get(&cangku);
        printf("get the %d product success\n",num);
        num++;
        if(num==PRODUCT_CNT)
            break;
    }
    printf("consume stop\n");
    return NULL;
}

int main()
{
    pthread_t th1,th2;
    void*retval;
    pthread_create(&th1,NULL,product,NULL);
    pthread_create(&th2,NULL,consume,NULL);

    pthread_join(th1,&retval);//阻塞回收线程
    pthread_join(th2,&retval);

    finish(&cangku);//销毁锁 条件变量
    return 0;
}

```

```tex
product the 0 product...
put the 0 ok
consume ...
get the 0 product success
product the 1 product...
put the 1 ok
product the 2 product...
put the 2 ok
consume ...
get the 1 product success
product the 3 product...
put the 3 ok
product the 4 product...
put the 4 ok
consume ...
get the 2 product success
product the 5 product...
put the 5 ok
//生产第6号的时候发现仓库是慢的  ，需要等仓库不满的时候 才能去生产  这里阻塞
product the 6 product...
product wait for not full----阻塞
consume ...
put the 6 ok
get the 3 product success
product the 7 product...
product wait for not full
consume ...
put the 7 ok
get the 4 product success
product the 8 product...
product wait for not full
consume ...
put the 8 ok
get the 5 product success
product the 9 product...
product wait for not full
consume ...
put the 9 ok
get the 6 product success
product the 10 product...
product wait for not full
consume ...
put the 10 ok
get the 7 product success
product the 11 product...
product wait for not full
consume ...
put the 11 ok
get the 8 product success
product the 12 product...
product wait for not full
consume ...
put the 12 ok
get the 9 product success
product the 13 product...
product wait for not full
consume ...
put the 13 ok
get the 10 product success
product the 14 product...
product wait for not full
consume ...
get the 11 product success
put the 14 ok
product the 15 product...
product wait for not full
consume ...
get the 12 product success
put the 15 ok
product the 16 product...
product wait for not full
consume ...
get the 13 product success
put the 16 ok
product the 17 product...
product wait for not full
consume ...
get the 14 product success
put the 17 ok
product the 18 product...
product wait for not full
consume ...
get the 15 product success
put the 18 ok
product the 19 product...
product wait for not full
consume ...
get the 16 product success
put the 19 ok
product the 20 product...
product wait for not full
consume ...
get the 17 product success
put the 20 ok
product the 21 product...
product wait for not full
consume ...
get the 18 product success
put the 21 ok
product the 22 product...
product wait for not full
consume ...
get the 19 product success
put the 22 ok
product the 23 product...
product wait for not full
consume ...
put the 23 ok
get the 20 product success
product the 24 product...
product wait for not full
consume ...
get the 21 product success
put the 24 ok
product the 25 product...
product wait for not full
consume ...
get the 22 product success
put the 25 ok
product the 26 product...
product wait for not full
consume ...
get the 23 product success
put the 26 ok
product the 27 product...
product wait for not full
consume ...
get the 24 product success
put the 27 ok
product the 28 product...
product wait for not full
consume ...
get the 25 product success
put the 28 ok
product the 29 product...
product wait for not full
consume ...
get the 26 product success
put the 29 ok
product the 30 product...
product wait for not full
consume ...
get the 27 product success
put the 30 ok
consume ...
get the 28 product success
consume ...
get the 29 product success
consume stop

```



### 线程的高级属性

#### 一次性初始化

`mutex cond rwlock`等等变量不能直接使用，必须初始化后才能使用，并且只能初始化1次，否则会报错，怎么确定初始化一次？

+ 首先定义`pthread_once_t `类型变量，用宏`PTHREAD_ONCE_INIT`（即0）去初始化
+ 创建与控制变量相关的初始化函数

```c
void init_routine(void)
{
    //初始化互斥量
    //初始化读写锁
    ...
}
```

+ 接下来可以任意时刻调用`pthread_once(*pthread_once_t,回调init_routine)`去初始化
+ 尽管`pthread_once`可能在多个线程中被调用，但回调函数只会被调用一次，具体哪个线程中调用由内核决定

demo

```c
pthread_once_t once=PTHREAD_ONCE_INIT;//必须这样初始化，否则会异常
pthread_t tid;

void thread_init()//发现此回调函数只会执行1次，有可能在线程1中执行，有可能在2中执行，哪个线程先执行，就在哪个里面回调
{
    printf("i am in thread 0x%x\n",tid);
}

void *thread_fun1(void*agr)
{
    tid=pthread_self();
    printf("i'm thread1,id=0x%x\n",pthread_self());
    pthread_once(&once,thread_init);
    return (void*)20;
}
void *thread_fun2(void*agr)
{
    tid=pthread_self();
    printf("i'm thread2,id=0x%x\n",pthread_self());
    pthread_once(&once,thread_init);
    return (void*)20;
}
int main()
{
    pthread_t tid1,tid2,tid3;
    int err;

    err=pthread_create(&tid1,NULL,thread_fun1,NULL);
    err=pthread_create(&tid2,NULL,thread_fun2,NULL);
    if(err!=0)printf("create thread failed\n");
    while(1);
    return 0;
}

```



#### 线程的分离属性

设置步骤：---所有系统都支持线程的分离属性（自动回收）

+ 定义属性变量 pthread_attr_t  attr;
+ 初始化属性  `pthread_attr_init(&attr)`
+ 设置分离或非分离属性`pthread_attr_setdetachstate(&attr,detachstate)`
  + PTHREAD_CREATE_DETACHED  分离，调join会失败
  + PTHREAD_CREATE_JOINABLE    非分离，可join的
+ 创建线程 `pthread_create(&tid,&attr,thread_fun,NULL)`
+ `pthread_attr_destroy(&attr)`  销毁属性



#### 线程的栈属性

对于进程来说，虚拟地址空间大小是固定的，每个进程都有自己的栈大小通常够用。

但对于线程来说，同样的虚拟地址被所有线程共享，若使用了太多线程，导致累计超过可用的虚拟地址空间，这时就需要减小线程默认栈的大小。如果线程分配了大量的自动变量或线程栈帧太深（函数调用嵌套），则需要增加线程栈的大小。

如果用完了虚拟地址空间，可以用`malloc或mmap`为其他栈分配空间，并修改栈位置

+ 修改栈属性
  + stackaddr 是栈内存单元最低地址，并不一定是栈的开始，有的栈地址从高往低
  + `stacksize`不能小于 PTHREAD_STACK_MIN (16384) bytes  会报错
  + `int pthread_attr_setstack(pthread_attr_t *attr,void *stackaddr, size_t stacksize);`
+ 获取栈属性
  + ` int pthread_attr_getstack(const pthread_attr_t *attr, void **stackaddr, size_t *stacksize);`
+ 只修改栈大小  不修改栈属性
  + `int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);
    int pthread_attr_getstacksize(const pthread_attr_t *attr, size_t *stacksize);`

默认栈大小可以看:ulimit -a 查看

demo

```c
#include<limits.h>    包含栈最小值的宏，如果设置的最小值小于PTHREAD_STACK_MIN，系统会按照PTHREAD_STACK_MIN去设置，如果》PTHREAD_STACK_MIN，按照设置值去设置
void *thread_fun1(void*agr)
{
    sleep(1);
    size_t stacksize;
#ifdef _POSIX_THREAD_ATTR_STACKSIZE
    pthread_attr_getstacksize(&attr,&stacksize);
    printf("new thread stacksize=%d\n",stacksize);
    pthread_attr_setstacksize(&attr,16284);
    pthread_attr_getstacksize(&attr,&stacksize);
    printf("new thread stacksize=%d\n",stacksize);
#endif
    return (void*)1;
}
int main()
{
    pthread_t tid1,tid2,tid3;
    int err;
        pthread_attr_init(&attr);
#ifdef _POSIX_THREAD_ATTR_STACKSIZE
        pthread_attr_setstacksize(&attr,PTHREAD_STACK_MIN);
#endif
    err=pthread_create(&tid1,&attr,thread_fun1,NULL);
    if(err!=0)printf("create thread failed\n");
	while(1);
    return 0;
}

```

#### 线程的私有数据

虽然变量名一样，但是每个线程访问这个数据的时候获得的值不一样

```c
void *thread_fun1(void*agr)
{
    printf("thread1 start\n");
    int fd=open("1.c",O_RDONLY);
    sleep(2);
    printf("thread1 errno=%d\n",errno);
    return (void*)1;
}
void *thread_fun2(void*agr)
{
        sleep(1);
        open("t.c",O_RDWR);
        printf("thread2 errno=%d\n",errno);
    return (void*)1;
}
int main()
{
    pthread_t tid1,tid2,tid3;
    int err;

    err=pthread_create(&tid1,NULL,thread_fun1,NULL);
    err=pthread_create(&tid2,NULL,thread_fun2,NULL);
    if(err!=0)printf("create thread failed\n");

        while(1);
    return 0;
}
当前文件夹创建t.c  没有1.c。
thread1 start
thread2 start
thread2 errno=0
thread1 errno=2   include<errno.h>  这个变量在errno头文件中
两个函数都访问同一个全局变量，获得的值却是不一样的
```

私有数据的使用

```c
pthread_key_t  key;
void *thread_fun2(void*agr)
{
    sleep(1);
    printf("thread2 start\n");
    int a=2;
    pthread_setspecific(key,(void*)a);   设置key与val关联
    printf("thread2 key=%d\n",pthread_getspecific(key));  获得key关联的值
    return (void*)1;
}
void *thread_fun1(void*agr)
{
    printf("thread1 start\n");
    int a=1;
    pthread_setspecific(key,(void*)a);
    sleep(2);
    printf("thread1 key=%d\n",pthread_getspecific(key));
    return (void*)1;
}
int main()
{
    pthread_t tid1,tid2,tid3;    int err;
    pthread_key_create(&key,NULL);//第二个参数为析构函数，销毁key的时候调用，释放关联的空间
    err=pthread_create(&tid1,NULL,thread_fun1,NULL);
    err=pthread_create(&tid2,NULL,thread_fun2,NULL);
    if(err!=0)printf("create thread failed\n");
    pthread_key_delete(&key);  销毁key--
    while(1);
    return 0;
}

```

利用析构函数进行销毁怎么销毁,析构 键的 函数发生在线程退出之前。

```c
pthread_key_t key;
typedef struct Tsd
{
    pthread_t tid;
    char *str;
} tsd_t;

//用来销毁每个线程所指向的实际数据--入参不需要传，默认删除的是与key绑定的所有的地址空间数据
void destructor_function(void *ptr)
{
    free(ptr);
    printf("destructor..%p\n",ptr);
}

//初始化函数, 将对key的初始化放入该函数中,
void init_routine()
{
    pthread_key_create(&key, destructor_function);
    printf("init...\n");
}

void *thread_routine(void *args)
{
    //设置线程特定数据
    tsd_t *value = (tsd_t *)malloc(sizeof(tsd_t));
    value->tid = pthread_self();
    value->str = (char *)args;
    pthread_setspecific(key, value);
    //不同的线程 malloc不同的地址，之后销毁的时候 malloc了几个地址，就free几个地址
    printf("%s setspecific, address: %p\n", (char *)args, value);
    //获取线程特定数据
    value = (tsd_t *)pthread_getspecific(key);
    printf("tid: 0x%x, str = %s\n", (unsigned int)value->tid, value->str);
    sleep(2);
    //再次获取线程特定数据
    value = (tsd_t *)pthread_getspecific(key);
    printf("tid: 0x%x, str = %s\n", (unsigned int)value->tid, value->str);
    pthread_exit(NULL);
}

int main()
{
    pthread_t tid1, tid2;
    init_routine();
    pthread_create(&tid1, NULL, thread_routine, (void *)"thread1");
    pthread_create(&tid2, NULL, thread_routine, (void *)"thread2");

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_key_delete(key);
    return 0;
}

打印日志：
init...
thread1 setspecific, address: 0x7feffc000b60
tid: 0x1c41700, str = thread1
thread2 setspecific, address: 0x7feff4000b60
tid: 0x1440700, str = thread2
tid: 0x1c41700, str = thread1
tid: 0x1440700, str = thread2
destructor..0x7feffc000b60
destructor..0x7feff4000b60

```



#### 线程与fork

线程中怎么安全的使用fork

错误的使用demo:

```c
pthread_mutex_t mutex;
void *thread_fun1(void*agr)
{
    sleep(1);//延时使main 父进程 中先去抢占mutex
    pid_t pid;
    pid=fork();//fork之前锁被锁住了，父进程一把被锁住的锁，子进程一把被锁住的锁
    if(pid==0)
    {
        pthread_mutex_lock(&mutex);//子进程进来 直接发现自己的锁是锁住的，得先解锁，不然会阻塞 子进程的锁得自己去解锁 
        printf("child\n");
        pthread_mutex_unlock(&mutex);
    }else if(pid>0)
    {
        pthread_mutex_lock(&mutex) ;//父进程在main中解锁后 这里得以抢占，可往后运行。因为pid>0的部分只是进程的一个分支线程而已。main中还是会运行的
        printf("parent\n");
        pthread_mutex_unlock(&mutex);
    }
    return (void*)20;
}
int main()
{
    pthread_t tid1,tid2,tid3;
    int err;
    char* ret=0;
    err=pthread_create(&tid1,NULL,thread_fun1,NULL);
    if(err!=0)printf("create thread failed\n");
    pthread_mutex_lock(&mutex);
    sleep(2);//父进程抢占2s后 再释放
    pthread_mutex_unlock(&mutex);
    printf("main\n");
    void*jexitcode;
    int jret=0;
    jret=pthread_join(tid1,&jexitcode);
    return 0;
}

```



解决办法：

```c
pthread_mutex_t mutex;
void prepare()  //调用fork之前调用
{
    pthread_mutex_lock(&mutex);
    printf("in %s\n",__FUNCTION__);
}

void parent()   //fork返回父进程之前调用
{
    pthread_mutex_unlock(&mutex);
    printf("in %s\n",__FUNCTION__);
}

void child()    //fork返回子进程之前调用
{
    pthread_mutex_unlock(&mutex);
    printf("in %s\n",__FUNCTION__);
}
void *thread_fun1(void*agr)
{
    sleep(1);
    pid_t pid;
    pthread_atfork(prepare,parent,child);
    pid=fork();
    if(pid==0)
    {
        pthread_mutex_lock(&mutex)      ;
        printf("child\n");
        pthread_mutex_unlock(&mutex);
    }else if(pid>0)
    {
        pthread_mutex_lock(&mutex)      ;
        printf("parent\n");
        pthread_mutex_unlock(&mutex);
    }
    return (void*)20;
}
int main()
{
    pthread_t tid1,tid2,tid3;
    int err;
    char* ret=0;

    err=pthread_create(&tid1,NULL,thread_fun1,NULL);
    if(err!=0)printf("create thread failed\n");
    printf("in main after thread fork\n");
    pthread_mutex_lock(&mutex);
    sleep(2);
    pthread_mutex_unlock(&mutex);
    printf("main\n");
    void*jexitcode;
    int jret=0;
    jret=pthread_join(tid1,&jexitcode);
    return 0;
}

```



## `tcp`

### 其他

`ip+port` 绑定唯一程序

通信域--在哪个范围通信：

`ipv4 ipv6  unix  `

套接字类型：

`dgram  raw  stream`



`ip`地址转换：数字+点--->32位整数

`int inet_aton(const char *cp, struct in_addr *inp);`   字符串转ip

`int inet_aton(const char *cp, struct in_addr *inp);`   ip转字符串



字节序转成网络字节序:（大端）------处理端口号 port的

`htons---u short`		主机转网络

`htonl---u  long	`	主机转网络

`ntohs--u  short `  	 网络转主机

`ntohl--u long 	`	  网络转主机



accept 返回一个新的socket关联到客户端，传递给accept的socketfd并没有关联客户端，需要继续保持可用状态接收其他cli请求-----是个阻塞函数，一直等客户端请求







void*arg 看传参的时候怎么传的解的时候先转化成传参的类型，然后用`*`解引用

```c
例如给线程传参写的(void*)1;   想用把数据解为int类型
void * thread_fun(void*arg)
{
    printf("arg==%d",*(int*)arg);//先把arg转成int*的指针  再* 指向对应的区域
}
```

