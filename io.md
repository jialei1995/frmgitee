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

