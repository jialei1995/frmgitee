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