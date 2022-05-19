
#include "led_resource.h"

//定义属于board_a的资源，还可以新建board_b的资源，放到board_B_led变量中去
static struct led_resource board_A_led = {
	.pin = GROUP_PIN(3,1),//此资源是3port的第1pin脚
};

struct led_resource *get_led_resouce(void)
{
	return &board_A_led;
}


