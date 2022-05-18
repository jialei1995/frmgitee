KERN_DIR=/lib/modules/5.4.0-42-generic/build
all:
	make -C $(KERN_DIR) M=`pwd` modules
	$(CROSS_COMPILE)gcc -o hello_drv_test  app.c
clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules*
	rm -rf hello_drv_test
#±àÒë³ÉÄ£¿é
obj-m +=	led_drv.o
