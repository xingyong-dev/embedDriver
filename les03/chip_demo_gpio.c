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

#include "led_opr.h"
#include "leddrv.h"
#include "led_resource.h"

static int g_ledpins[100];
static int g_ledcnt = 0;

static int board_demo_led_init(int which)
{
    printk("init gpio : line %d, led %d\n", __FILE__,__FUNCTION__, __LINE__, which);
    switch(GROUP_PIN(g_ledpins[which]))
    {
        case 0:
        {
            printk("init pin of group 0");
            break;
        }
    }
}

static int board_demo_led_ctl(int which , char status)
{

}
struct led_operations led_src = {
    .init = board_demo_led_init,
    .ctl = board_demo_led_ctl,
};
struct led_operations *get_board_led_opr(void)
{
    return &led_src;
}