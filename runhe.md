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

```c
grub2-editenv list == 列出来的就是grubenv里面的变量，可以通过grub2-set-default 去设置
initramfs生成：dracut -H -f /path/initramfs-$(uname -r)  $(uname -r)
    	其实安装内核的时候再post阶段就会有这个命令
    	rpm -qa --scripts xxx包名
lsblk：看硬盘挂载

```



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

```shell
$(( xxx ))  双层括号内可以进行数学计算
```



### 软件包版本beta完美  alpha很多bug还在测试

## linux基本命令

realpath  fielname    filename的绝对路径

who 显示在那个终端ttyx

lsblk  查看当前挂载的磁盘（硬盘）分区

brctl  show:查看我的网桥连接的哪些网卡  ---桥接

blkid 看磁盘uuid

strings  xxx.so     在库中提取字符串并打印出来

dd iflag=xxx oflag=yyy if=uImage of=/dev/sdb    seek=1000扇区号，将内核存储到1000扇区

md.b  30008000 40 看内存中的数据

memory display   yi  byte

gzip -d xxx.gzip   解压

list --dependencies  xx.target

```c
ls > 1.txt ==== ls 1>1.txt 标准输出流到1.txt
ls 1>file  2>&1  把标准错误流给标准输出流，把标准输出流 存到 file中
ls 2>file  1>&2  把2给1，把1给file

```

静态库与动态库实际都是许多二进制的集合。

```c
addr2line  0x11111  -f -e  a.out  找地址对应的代码行
size  a.out   看每个数据段大小
ar crs libxx.a x.o  y.o  打包静态库
ar -x  libxx.a  解压静态库
objdump -d a.out  反汇编二进制为汇编代码
objcopy -j .text  a.out  onlytexta.out   只抽出来.text段放到后面的文件中

```

解压.gz文件  gzip -d xxx.gz

##### ctags

- ctrl+]  调到定义处

- ctrl+o 返回

- 在有tags的目录执行vim才可以
- vim -t 函数名、宏名 可以直接跳到定义处

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

### git add过的文件修改后直接git commit -a 啥都不用加就好了

vim 查看二进制文件：：%!xxd + enter

---

 ## python

+ os.getcwd()  获取当前路径
+ os.listdir(“路径”)     列出当前文件夹下所有文件与文件夹
+ os.path.isdir('filename')    判断filename是不是个文件夹
+ os.mkdir('xxx')   新建文件夹名字为xxx--单个文件夹
+ os.makedirs('1层/2ceng/3层/xxx')
+ os.path.exists('xxx')  判断文件是否存在

```python
strA.startswith(strB)  字符串A是否以字符串B开始，结束  
strA.endswith(strB)
```

+ glob

  + ```python
    print(glob.glob("lesson*.txt"))  打印所有lesson开头并且txt结尾的文件
    ```

+ recursive  递归的

```python
import fnmatch   #子串匹配返回true or false
print(fnmatch.fnmatch("lesson.py","l*.py"))   返回true

--- 
获取文件信息
print(filename.stat())    创建时间

---
import time
print(time.ctime(12345654))   将数字转换为人类可读的时间写法
---
读写文件
f = open("1.txt",'r',encodeing='utf-8')
text = f.readlines()		每行都读取进来
f.close()	必须关闭
更建议用以下方式：
with open("1.txt",'r'，encoding='utf-8')  as f:
    text = f.readlines()     这种方式自动关闭，不用自己关close
f.write(字符串)     写的都是字符串

----
from tempfile import TemporaryFile
f = TemporaryFile()
---
import shutil   移动文件
shutils.copy('filename','./xxdir')  把文间移入dir
shutils.copy('filename','./xxdir/newname')  把文间移入dir并重新命名为newname
shutils.copytree('filedir','../')   递归复制本文件夹及文件夹内的所有到上个文件夹中
shutils.move('可以是文件也可以是文件夹'，'移动到哪里呢')
---  重命名文件
os.rename('filename','newname')    可以时文件也可以是文件夹名字
name = 'myadd'+'name'  字符串拼接赋值也可以重命名

---  删除文件
os.remove('filename')  只能删除文件
shutils.rmtree('xxidr')  删除文件夹

--- 读压缩包内的文件
import zipfile      只针对zip文件
with zipfile.ZipFile('xxx.zip','r')  as f:
    print(f.namelist())   可以打印出来所有的压缩包内的文件名  
    for i in f.namelist():
        print(i.encode('cp437').decode('gbk'))  如果文件名有中文这么处理打印出来
---numpy
numpy.arange(10)   生成n个数字的一维数组
data.reshape(2,5)  把它变成二维的
data.reshape(2,5).T   再转置
numpy.sqrt(data)   每个数字都开根
np.genfromtxt('1.txt',delimiter=',')  提取1.txt中数据，1.txt中数据是以‘，’分割的
data.astype(int)  data中每个数据都转化为int类型
---pandas
处理series 1列的数据
处理dataframe  x列的数据
切片是一段连续的数字才可以切片，切下来的数字是连续的


----- zip压缩
解压缩：
with zipfile.ZipFile(xx.zip，‘r’) as zp:
    zp.extrat('1.txt')   将压缩中的1.txt取出来
    zp.extractall()   全部解压到当前目录
压缩：
with zipfile.ZipFile(xx.zip，‘w’) as zp:
    for i in files:
        zp.write(i)     1个1个文件塞进去
 with zipfile.ZipFile(xx.zip，‘a’) as zp:
        zp.write(‘3.txt’)   zip中已有，再往里压压  

 ---- excel  openpyxl
wb = load_workbook(filename='xx.xlsx')  只能加载已存在的
wb.sheetnames
sheet = wb['sheet1']
wb.save(filename='xx.xlsx')
cell = sheet('A1')  cell = sheet(row = 1,clumn = 1)
data = cell.value
cell.row  cell.column
cell.corodinate   cell的坐标
cells= sheet('A1:B6')  范围
cells= sheet('1:5')
cells= sheet('A:Z')

```

```c
systemctl rescue  进入rescue模式，单用户
systemd-analyze   查看开机启动耗时

```

---

### LVM逻辑卷管理器

```c
物理存储介质：/dev/hda,vda,sda,。。。 最底层的存储单元
物理卷（PV）：物理介质硬盘分区。再建立卷组时确定PE最小单元大小，确定后不可更改
卷组（vg）：至少包含1个物理卷
逻辑卷（LV）：建立在卷组里面，卷组中未分配的空间都可以用于建立新的逻辑卷，最大的好处就是可以动态扩容或缩小，不像物理硬盘，多大就多大。
LE：逻辑区域，是逻辑分区中的可分配的最小单元 === PE，物理区域
命令：
创建物理卷：pvcreate /dev/sda1  sda1就是个物理卷，再往下分就是逻辑卷了
查看物理卷：pvdisplay
卷组：vgcreate lvmdisk /dev/sda1  /dev/sdb1  lvmdisk就是卷组名字，用两个物理卷做出来的
		创建卷组的时候PE大小就确定了，	这个物理卷可以动态扩容的
逻辑卷：lvcreate 逻辑卷中LE大小==卷组中PE的大小
在lv中创建文件系统fs：mkfs -t ext2  /dev/lv的path  我们的根文件系统就挂载在这里
创建好fs之后挂载：mount /dev/openeuler/root  /  挂载后即可使用，为了开机自动挂载修改/etc/fstab文件
    			mount /dev/lvpath1  /home
根目录空间不够了：如果此时vg还有剩余的PE，可以：
lvextend  l+1122(空闲PE，可通过vgdiaplay查询)  /dev/openeuler/root   扩了硬件
ext2online /    同时扩容fs，扩fs
还可以这样扩硬件：lvextend L+55G  /dev/openeuler/root   ext2online  /

若卷组空间不够了：
vgextened  lvmdisk  /dev/sdd1   往卷组中增加物理卷-->之后再扩容lv

```

Elapesed time：运行时间



### 系统移植

+ tftp 410000 uImage          movi write kernel 410000   先拉倒内存，再去写到emmc
+ tftp  410000 exynos.dtb    movi write dtb  4100000 
+ tftp  410000  ramdisk.img    movi write  rootfs 4100000 
+ emmc分几块：kernel  dtb   rootfs  还有3个好像
+ setenv bootcmd movi read kernel 410000;movi read dtb 420000;movi read rootfs 430000;bootm 41000 430000  42000;先加载内核，接下来fs，（没fs你在哪里存放设备树呀），最后设备树

```c
make mrproper    是清除，比make clean清除的更干净一些
make xxx_config   执行xxx类型板子的config   在arch/arm/configs中
---
加驱动在driver/char里面，加好还得修改char目录下的makefile，根据模块是否被选中对
  它进行编译
```

```C
参照自己/lib/moduels/uname-r/build/Makefile中的cross_compile去修改makefile

```

---

### 算法

```c
哈希表又称散列表：
	把数据存储起来，根据数组下标，搜索起来速度O1，很方便
取余法：经典
如果数据跨度不大，可以直接建立数组，a[100]，将对应的需要存储的数字数组中下标置1.
鸽巣原理：500只鸟存不到499个笼子里面。md5值理论上说不是唯一的，2^128次方

二分搜索树：BST，就是二分法，理论上一球样的。搜索效率logn。

```

### gdb

```c
c : 继续运行程序，直到遇到下一个断点，若没有断点，程序就会运行完。
set var 变量名=‘xxx’  或者数字  set var i=10
./a.out arg1 arg2  程序如果运行时这样传参
set args arg1 arg2  用gdb调试的时候就这样传   run 之前
gdb里面的数字代表的是：行数 + 代码内容
info  b:查看断点信息
display num1:每次next都会主动将num1变量的值打印出来，看他是否变化
watch:
---进程：
set follow-fork-mode parent  默认就是父进程，这个不用set
set follow-fork-mode child
设置调试模式：set detach-on-fork [on/off]默认是on
on：调试一个进程另一个进程继续运行完
off:调试一个进程另一个进程被gdb挂起 阻塞
info inferiors  查看当前调试的进程信息
inferior  进程id ：切换当前调试的进程  1/2
---线程：
info threads  查看线程
thread 线程id:切换线程
set scheduler-locking on只运行当前进程
set scheduler-locking off 运行全部的线程
thread apply 线程id  cmd  指定某线程执行某gdb命令，id是1、2、3不是系统的线程id
thread apply all  cmd  指定全部的线程执行某gdb命令
---gcc
-I ：如果不指定，默认从/usr/include 中寻找.h
-L：如果不指定，默认从/usr/lib 中寻找.so
-O：优化选项，不能与-g 一同使用
-Wall：将warning视为error，有warning就退出，不会继续运行
time ./a.out 计算a.out的运行时间
```

---

### coredump

```c
sleep 100 & ：将此程序运行放后台 此时会显示他的进程号 通过jobs也可查看
kill -11 pid：将pid触发coredump
coredump gdb pid:去调试这个dump的进程
coredumpctl list：列出来当前的coredump事件
watch：gdb调试的时监控某个变量    *****显示的更详细，还带的上次的值类似： dispaly num1
    Hardware watchpoint 3: j
    Old value = 104
    New value = 105
LD_PRELOAD=/home/xxx.so   优先加载这里面的库文件，即使lib里面有，先从这里找
ldd  /lib/xx.so   查看库文件的依赖关系
更改了/usr/lib/systemd/system/xx.service 后要systemctl daemon-reload  xx.service 再重启服务
这个service 中的Limitcore=xxx 就是限制调用栈的大小
cat /proc/pid(进程号)/limits   看进程的调用栈限制****
systemd  资源控制初探，看看文章。
```

#### 断言assert 调用栈（很重要）



