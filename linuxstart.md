# linux启动时grub报错

### grub

+ （hd0,gptx）（hd1,gptx）这都是分区名
+ ls (hd0，gpt1)  查看分区格式     带linux的就是存放grub的分区(filesystem is ext2) ext2才是存grub的分区
+ set prefix = (分区名)/boot/grub
+ set root = 分区名
+ insmod normal  安装模块
+ normal 运行模块
+ 进入终端后update-grub

### 启动过程

- 现在centos7的启动1个systemd接管所有的unit，即使b依赖a。在a还没启动的时候就会发出1个a已经启动成功的信号，所以b就可以直接启动了。这样所有unit都一起启动。
- 启动配置文件
- 1. 优先级最低：/usr/lib/systemd/system 每个服务的启动脚本
  2. 比上面的高一些：/run/systemd/system 系统执行过程中产生的服务脚本
  3. 优先级最高：/etc/systemd/system 管理员建立的执行脚本

### unit类型

+ target unit  模拟运行级别的 xx.target===runlevelx
+ swap unit  标识swap设备

### service unit文件通常由3部分组成

+ unit  定义与unit无关的通用选项，描述unit，行为、依赖
+ 1. description
  2. after 当前servoce应晚于哪些unit启动
  3. requires  依赖其他的units，强依赖：其他unit不激活，自己也不能用
  4. wants  依赖其他units  弱依赖
  5. conflicts  units间的冲突关系，我启动某些unit就不能启动了

+ service  特定类型相关选项
+ 1. execstart 指明启动unit要运行的脚本的绝对路径
  2. execstop  指明停止unit要运行的脚本的绝对路径
+ install  定义由systemctl enable 、systemctl disable实现或禁用时用到



### sed 默认时对每一行都要处理的

sed -i '/xxx/d'  file  删除

sed -n '/xxx/p'  file   打印

sed -n -f  xx.sedscript file   根据sed脚本对file的内容进行修改输出

sed -n 's/xx/yy/g'  file 文件内容xx改为yy打印出来

sed -[opt]  '动作（p,i,a,d,c替换）'

sed -i '$chello'  最后一行换成hello

sed -i '/xxx/cyyy'  带xxx的行换成yyy

sed -i '3ixxx'   3行前面加xxx

sed -i '1,4d'   删除1-4行

sed -i '/[0-9]/d'   删除包含数字的行

删注释:sed -i 's/^#//'  1.c  

### shell函数传参

定义时：fun（）{$1xxx    $2xxx}    

返回值只能返回函数执行状态0成功非0失败。要其他的话用定义全局变量接受

调用时fun xx yy

read -p 'xxxxx'  varname

### 软件包版本beta完美  alpha很多bug还在测试

## linux基本命令

realpath  fielname    filename的绝对路径

who 显示在那个终端ttyx

lsblk  查看当前挂载的磁盘（硬盘）分区

brctl  show:查看我的网桥连接的哪些网卡  ---桥接

strings  xxx.so     在库中提取字符串并打印出来

dd iflag=xxx oflag=yyy if=uImage of=/dev/sdb    seek=1000扇区号，将内核存储到1000扇区

md.b  30008000 40 看内存中的数据

memory display   yi  byte

#### os的功能，向上对用户提供接口，向下操控硬件

Yahoo是www的服务器。apache是httpd的服务器。伺服器===服务器

网关就是路由器的ip？？？反正是192.168.1.1 还是.0来着

### docker 

docker load --input   xx.tar.gz   将这个镜像压缩包安装到os中

### chroot 在oscbuild 后在/osc /buildroot 目录中:chroot .

拷贝要安装的包至/osc /buildroot/home/xxxrpmbuild/sources/中

### vim 

+ vim 1.c +18直接打开就是第18行
+ diff -Nur在那个目录执行   打patch时就在那个补录打可以时-p0  在目录里面就开始-p1  -p2  

### selinux状态：enforcing permissive disable

+ 某进程只能执行某类文件
+ ls -Z查看文件的属性
+ 用户 1. user system undefined 
+ 角色  :文件-object    程序-system   用户-
+ 类型  
+ 进程 domain

### 开机自动挂载在/etc/fatab设置

### mount -t iso9660 -o ro /dev/cdrom /mnt

​                    类型 ISO           挂载方式   只读  

### make

make -j4编译时，如果出问题其他线程还会继续编译，不会卡停，4条线同时向下进行

make时有时候是需要联网的。

### osc mkpac xxx 命令行新建package

### obs上新建的工程home:jialei:不能改，后面的可以改

#### 磁盘要存储东西要先格式化（弄好文件系统，否则不能存储）

### 桥接与NAT

+ 桥接：虚拟机与宿主机在同一网段，外界能访问虚拟机虚拟机也能访问外界（同一网段内其他主机）
+ NAT：虚拟机可以访问外部其他主机，但是其他主机看不到它