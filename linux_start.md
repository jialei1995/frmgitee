# 对于redhat的启动流程分析
grub==启动引导管理器
## start阶段
1. bios自检 硬件自检，找grub（即boot）
2. 加载运行grub（rhel6 为grub，rhel7为grub2）
3. 内核、镜像文件系统加载，加载基本设备驱动
    1. 确定启动分区->一般为整个磁盘的第一个分区
    2. 内核文件位置
    3. 镜像文件系统--实际是假的根--加载它之后可以识别磁盘上后续启动相关的配置文件、基本驱动
4. 启动第一个服务程序（以前init 现在systemd），进入指定运行级别，开启对应级别的服务


### grub配置修改
对于老版本可以直接修改
/etc/grub/grub.cfg 或  /etc/grub2/grub.cfg
对于新版本需要通过修改开关文件去修改
/etc/grub/default
配置文件：定义启动分区、内核、镜像文件系统 与 根文件系统位置
```c  
新版本的设置方式
set root='hd0,msdos1'  实际内核在/hd0,msdos1/boot 目录
linux   /boot/vmlinuz-5.4.0-84-generic 启动分区、内核root=UUID=e0ff9872-66de-411e-897d-bc1950349dd9 ro 根文件系统find_preseed=/preseed.cfg 
auto noprompt 
priority=critical locale=en_US quiet
initrd  /boot/initrd.img-5.4.0-84-generic  镜像文件系统
```

### grub的重装方法
### grub-shell语法
在菜单选择界面按'c'就会进入grub的命令行
命令行需要设置 4步
1. 根分区位置
    1. root (hd0,fenqu0)  可以tab补全，老版本设置方法
    2. set root=()         新版本设置根方法
2.  内核文件位置  根文件系统位置
    1. kernel /xx.img  ro root=/dev/mapper/volgroup 
3. initramfs
    initrd /xx.img
4. boot 启动系统
其实系统启动后，也可以直接grub，进入grub-shell中，quit退出即可


### grub与grub2的区别
### ramdisk原理与挂载切换过程
### 不同系统损坏产生的不同现象
### init与systemd不同的地方
运行级别
inittab
服务配置方法

### 相关命令
journalctl 比看日志有什么优点
systemctl
runlevel

## bios与uefi详解
位于系统rom中
第一阶段：检查硬件
第二阶段：选择启动引导管理器

新型bios->uefi  支持很大存储的设备2T以上
bios只支持16个分区，uefi支持几百个分区启动
efi速度快 bios速度慢

### rhel6之前版本 grub组成
1. stage1 由grub的stage1.s汇编生成，默认安装到mbr的头446字节
2. bios自检完成后会将stage1载入内存，stage1的唯一作用就是 将start.s装载到内存运行，然后访问 stage1_5或stage_2（只有手动安装grub的情况下才会先通过stage1_5，否则都是直接硬件跳转stage2）
    1. 若加载stage1_5,则可以通过文件系统跳转到stage2
    2. 若不加载stage1_5,则只能通过硬件跳转stage2
    3. stage1_5的作用：获得访问文件系统的能力
    4. stage2的作用：提供了grub的所有功能，包括启动菜单和交互的grub-shell--从grub菜单按'e' 进入grub-shell
        + 还包括管理 识别 grub的配置文件grub.cfg，自动和手动加载内核、ramdisk镜像文件系统等工作

系统启动过程中 能看到进入菜单界面就说明stage2完全加载成功了

grub的核心文件会在系统安装的最后阶段写入磁盘，并且可以选择安装到磁盘或者分区头部，默认就会安装在磁盘头部(/dev/sda)
+ 系统安装时会在文件系统产生stage1 stage1_5 stage2等文件，但默认情况下系统启动时，会在加载完stage1后直接跳转stage2
+ 只有通过grub-install 手动安装grub时，stage1_5才会被使用，用来获得访问文件系统 的能力，通过文件系统寻址加载stage2
grub被安装在分区头部时 
1.不安装stage1_5 
2.stage2安装于分区头部于文件系统之间 并通过文件系统方式对stage2进行访问

### grub.cfg内容
1. 内核和镜像文件系统所在分区--通常是启动分区
2. 根分区位置、根分区挂载方式 其他配置项
临时指定gurb-shell中的内容后 按’c‘ 开始启动
常用命令：设置根分区  设置内核位置  设置initramfs
老版本：root kernel initrd
新版本：set root=; linux/linux16 ;initrd/initrd16
boot 开始启动