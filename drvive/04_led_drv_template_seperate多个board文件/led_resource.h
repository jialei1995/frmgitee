#ifndef _LED_RESOURCE_H
#define _LED_RESOURCE_H

/* GPIO3_0 */
/* bit[31:16] = group */
/* bit[15:0]  = which pin */
#define GROUP(x) (x>>16)
#define PIN(x)   (x&0xFFFF)
#define GROUP_PIN(g,p) ((g<<16) | (p))

struct led_resource {
	int pin;//代表整个led资源，此led属于哪个port，哪个pin
};

struct led_resource *get_led_resouce(void);

#endif

