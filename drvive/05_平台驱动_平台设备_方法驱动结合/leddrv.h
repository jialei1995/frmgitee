
#ifndef _LEDDRV_H
#define _LEDDRV_H

#include "led_opr.h"
//对应c文件中export的函数得在头文件声明，别的文件包含此头文件即可使用这些函数
void led_class_create_device(int minor);
void led_class_destroy_device(int minor);
void register_led_operations(struct led_operations *opr);

#endif /* _LEDDRV_H */

