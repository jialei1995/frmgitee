KERN_DIR=/lib/modules/5.4.0-42-generic/build #要用当前系统的内核makefile去编译，否则安装不到当前系统中
all:
	make -C $(KERN_DIR) M=`pwd` modules
	$(CROSS_COMPILE)gcc -o hello_drv_test  app.c #编译测试程序，测试驱动安装后能否执行
clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules*
	rm -rf hello_drv_test
#编译成模块
obj-m +=	hello_drv.o #加的o文件名字要与模块的名字一致，如果模块文件为hello.c则这里应该是hello.o
