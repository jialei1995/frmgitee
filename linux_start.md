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
/etc/grub/grub.cfg
对于新版本需要通过修改开关文件去修改
/etc/grub/default

### grub的重装方法
### grub-shell语法
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
