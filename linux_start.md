# 对于redhat的启动流程分析
grub==启动引导管理器
## start阶段
1. bios自检 硬件自检，找grub（即boot）
2. 加载运行grub（rhel6 为grub，rhel7为grub2）
3. 内核、镜像文件系统加载，加载基本设备驱动
    1. 确定启动分区->一般为整个磁盘的第一个分区
    2. 内核文件位置
    3. 镜像文件系统--实际是假的根，临时根文件系统--加载它之后可以识别磁盘上后续启动相关的配置文件、基本驱动
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
#### systemctl
+ systemctl reload xx 重载配置文件
+ systemctl list-units --type service --all
列出所有服务
+ systemctl daemon-reload 修改某个服务需重新加载配置（如 /etc/fatab）
+ systemctl --failed --type=service 列出所有失败的服务
+ systemctk list-dependencied ssh.serivce 列出此服务的依赖关系
+ systemctl cat graphical.target 查看配置
+ systemctl set-default multi-user.target 默认进入字符界面
+ systemctl set-default graphical.target 默认进入图形界面

runlevel

#### 服务状态
active(running)  运行中
inactive        已关闭
active(exited)  运行一次就退出
active(waiting)  正在执行中，需要其他服务或事件，才能继续running

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

### rhel7之后版本 grub组成
/boot 分区不再有stage1 stage2等文件，启动开机后会通过文件系统寻找启动分区grub2目录的 *.mod ，grub2的完整功能由 mod文件提供
mod文件损坏会进入rescue模式

#### 对比 
grub的完整功能无法实现 导致原因
老版本  stage2损坏 会导致 无法进入grub-shell 
新版本  mod损坏    会导致 无法进入grub-shell

### grub.cfg内容
1. 内核和镜像文件系统所在分区--通常是启动分区
2. 根分区位置、根分区挂载方式 其他配置项
临时指定gurb-shell中的内容后 按’c‘ 开始启动
常用命令：设置根分区  设置内核位置  设置initramfs（临时跟文件系统）
老版本：root kernel initrd
新版本：set root=; linux/linux16 ;initrd/initrd16
boot 开始启动
grub-set-default  设置默认启动菜单，默认启动项
grub-install  安装grub

3. default=x  表示选择对应的菜单栏进入系统。有的系统菜单栏有多个内核对应多个选择菜单
4. timeout=x 在菜单栏停留几秒 再进入，给时间选择是否进入grub-shell
5. quiet 屏蔽启动log  一般都把quiet删了 别用
6. title 启动项菜单栏的描述--可以随便写

针对新版cfg文件配置，需要修改/etc/default/grub,修改后需要执行命令重新生成cfg文件：
1. 无efi的
    1. grub2-mkconfig -o /boot/grub2/grub.cfg
2. 有efi的
    1. grub2-mkconfig -o /boot/efi/EFI/grub.cfg

grub-shell只有在stage2成功加载gurb全部功能后才能使用，所以grub-shell是否出现，能正常使用也可以作为启动故障点判断的重要依据

### 内核与镜像文件系统的加载
#### 内核阶段加载
+ 内核被解压并装载到内存 并初始化
+ 挂载镜像文件系统，以只读方式挂载其中的静态驱动模块
+ 退出镜像文件系统 挂载真正的根分区
#### 系统镜像文件加载：
包含大量的ko文件 
+ 在rhel6之前，kernel安装后，通过mkinitrd建立
mkinitrd /boot/initrd.img  $(uname -r)
+ 在rhel7以后，kernel安装后，通过dracut建立
dracut -H -f /boot/initramfs-$(uname -r).img $(uname -r)

rpm -qa --scripts kernel  可以看到内核安装后的post 阶段 scripts执行的命令


### 虚拟机挂载光盘
在虚拟机配置里面把iso文件放到对应位置，挂载给对应的虚拟机
在虚拟机中执行
mount /dev/cdrom /mnt  即可

### 操作系统启动过程
#### 老版本
1. 运行init 1号进程
2. 执行/etc/rc.d/rc.sysinit 脚本--系统初始化
3. 读取/etc/fstab     挂载对应的文件系统和设备
4. 读取/etc/inittab   定义要到的运行级别
进入到指定的运行级别
5. 启动指定级别应该启动的服务/etc/rc.d/rcX.d/*
rcX.d  X表示对应的级别的目录

runlevel：查看当前运行级别
init X：在当前级别基础上继续运行，更新至X运行级别 

#### 新版本
1. systemd 进程取代了init成为1号进程
2. systemd 优势
    1. 启动速度快（并行执行）
    2. 管理功能丰富（服务，挂载点，开关机，定时任务）
3. systemd unit 相关文件
    1. /usr/lib/systemd/system rpm包安装时原始unit文件
    2. /etc/systemd/system 执行systemctl enable 创建的unit文件，每个文件固定语法
    3. /run/systemd/system systemd运行时创建的文件
    4. /etc/systemd/syetem/ 目录中的大多文件为/usr/lib/systemd/system的符号链接
    5. 官方建议修改的时候修改/etc下的配置文件
4. systemctl 所管理的配置文件主要有4种；在/usr/lib/systemd/system目录可以看到多个文件都是这4类
    1. .mount 
        里面配置了what where type 3个数据项，等同于mount -t hugetlbfs /dev/hugepages hugetlbfs
    2. .target  运行级别
        + 定义了一些基础的组件，供service文件调用
    3. .service 
        + 定义了一个服务，分为Unit、Service、Install 共3段
        + [Unit]
            + Description：描述
            + After：在xx xx启动后，本服务才启动
            + ConditionPathExist：执行条件
        + [Service]
            + EnvironmentFile:变量所在文件
            + ExecStart:启动脚本
            + Restart：fail时重启
        + [Install]
            + Alias：服务别名
            + WantedBy：多用户模式需要-被哪个阶段需要
    4. .wants 某个target需要哪些命令
        + 定义了要执行的文件集合，每次执行 里面的文件都会执行

## 启动故障排查
### 错误的grub配置参数
+ 报错常见：/dev/mapper/xxx  does not exist 根文件系统写错了
+ dracut warning LVM not found  initramfs 写错了
修复方法：
1. 在菜单栏选项 ‘c’ 进入grub-shell指定正确的参数 boot启动
2. 在菜单栏选项 ‘e’ 进入cfg配置文件中修改  按'c'或'b' 启动系统

### 错误的驱动导致无法启动
解决方法：
+ 光盘引导进入rescue模式
+ 执行chroot 判断根文件系统是否完好
+ 手动强制覆盖安装内核（会重建initramfs --force）

### 启动分区破坏
+ grub-install重装grub
+ chroot后挂载光盘覆盖安装内核
+ 修改/etc/fatab 确保自动挂载正确
+ 若grub文件丢失，会自动进入grub-shell中，自己配置去启动--并不是大问题
#### 删除整个boot分区 或者 umount /boot