
#include <linux/module.h>

#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/kmod.h>
#include <linux/gfp.h>
#include <linux/platform_device.h>

#include "led_resource.h"

static struct resource board_A_led = { 
    {
        .pin = GROUP_PIN(1,10),
        .flags = IORESOURCE_IRQ,
        .name  = "forlinx_led_pin",
    }

};
static int __init led_dev_init(void)
{
    int err;
    err = platform_device_register(&board_A_led);
    return 0;
}
static void __exit led_dev_exit(void)
{
    platform_device_unregister(&board_A_led);
}

module_init(led_dev_init);
module_exit(led_dev_exit);

MODULE_LICENSE("GPL");