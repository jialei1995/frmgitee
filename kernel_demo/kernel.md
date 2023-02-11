### IPC组件

1. 管道
   + 单向通信
2. 命名管道
3. 消息队列
4. 信号量
5. 共享内存
   + 两个进程 共享一片内存

`netstat -anop `监听的是系统监听的端口

我们写用户态的程序 占用端口后，通过netstat -anop是查不到的

可以直接写个虚拟网卡程序，可以insmod 安装虚拟网卡，安装之后 ifconfig就能看到网卡出现了 -- 而且网卡可以ping 通

__asm__(

    汇编模板：

    输出：

    输入：

    描述：

)

直接在c语言中嵌入汇编代码

qemu 直接在linux中运行虚拟 内核程序--方便

搭建内核源码：

```c
wget https://mirrors.edge.kernel.org/pub/linux/kernel/v3.x/linux-3.9.3.tar.xz  
xz -d linux-3.9.3.tar.xz
  
error: code model kernel does not support PIC mode
修改${kernel_path}/arch/x86/Makefile文件中的BITS := 64脚本代码块，添加一行来修改gcc的编译参数，KBUILD_CFLAGS += -fno-pic

make i386_defconfig//make x86_64_defconfig  看自己cpu是什么架构的
make   需要30min   如果报gcc7.h不存在  则include/linux/compile.xxx.h   改成gcc7.h即可重新编译

制作根文件系统
```

make报错：

error: code model kernel does not support PIC mode
修改${kernel_path}/arch/x86/Makefile文件中的BITS := 64脚本代码块，添加一行来修改gcc的编译参数，KBUILD_CFLAGS += -fno-pic

### fatal error: curses.h: No such file or directory

```
sudo apt-get install libncurses5-dev libncursesw5-dev
```

fatal error: linux/compiler-gcc7.h: No such file or directory.
提示缺少compiler-gcc7.h这个文件，是由于内核版本较低和我的gcc版本不匹配造成的有三种解决方法：

1.在内核文件夹中include/linux目录下找到compiler-gcc4.h文件，不同内核版本可能不一样，也有可能是compiler-gcc3.h,将它重命名为compiler-gcc7.h。然后重新编译一下就好了。--重命名的gcc3编译通过
2.在新的内核源码中拷贝一个compiler-gcc7.h，将它拷贝到内核文件夹include/linux目录下，重新编译即可。
3.重装一个版本低一点的gcc。

以上方法编译 x86_64的内核失败（当前硬件是X86_64不知道为啥总是编译失败）---气死了--因为 自己的系统内核版本 与要编译的内核不匹配

下载18.04的ubuntu就可以编译

---

start_kernel分析：

中断上下文切换：

将用户态的15个reg 与 cpsr存起来---然后进入内核态  pc指向内核态代码

    恢复现场：从栈中把reg值全吐出来（一个进程只有一个栈，切换的时候数据全存到栈里面了）

进程上下文切换，进程间切换，每个进程都有自己的栈数据网哪里存呀？

系统调用：

read-->__asm__ intter 0x80--->sys_read

跳到80号对应的中断服务函数

软中断 inter 有明确的中断号

### 启动流程
