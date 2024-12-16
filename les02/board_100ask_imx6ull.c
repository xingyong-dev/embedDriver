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
#include <asm/io.h>

#include "led_opr.h"

static volatile unsigned int *CCM_CCGR1 ;
static volatile unsigned int *IOMUXC_SW_MUX_CTL_PAD_JTAG_MOD ;
static volatile unsigned int *GPIO1_GDIR;
static volatile unsigned int *GPIO1_DR;
static unsigned int r_CCGR1_ADDR = 0X20C4000 ;
static unsigned int r_CCGR1_GPIO1_ADDR = 0X6C; 
static unsigned int r_CCGR1_GPIO1_ByteLen = 0x04;
static unsigned int r_GPIO1_BASE = 0x209C000;
static unsigned int r_GPIO1_DR   = 0x00;
static unsigned int r_GPIO1_GDIR = 0x04;
// static unsigned int r_GPIO1_PSR  = 0x08;
// static unsigned int r_GPIO1_ICR1  = 0x0C;
// static unsigned int r_GPIO1_ICR2  = 0x10;
// static unsigned int r_GPIO1_IMR   = 0x14;
// static unsigned int r_GPIO1_ISR   = 0x18;
// static unsigned int r_GPIO1_EDGE_SEL  = 0x1C;

static int board_demo_led_init(int which)
{
    unsigned int val = 0;
    printk("%s %s line %d, which: %d\n", __FILE__, __FUNCTION__, __LINE__, which);
    if(which == 0)
    {
        if(!CCM_CCGR1 )
        {
            printk("%s %s line %d, CCM_CCGR1: %d\n", __FILE__, __FUNCTION__, __LINE__, 0);

            CCM_CCGR1                       = ioremap((r_CCGR1_ADDR | r_CCGR1_GPIO1_ADDR), r_CCGR1_GPIO1_ByteLen);

            IOMUXC_SW_MUX_CTL_PAD_JTAG_MOD  = ioremap(0x20E0044, 4);

            GPIO1_GDIR                      = ioremap(r_GPIO1_BASE | r_GPIO1_GDIR, 4);

            GPIO1_DR                        = ioremap(r_GPIO1_BASE | r_GPIO1_DR, 4);
        }
        /* GPIO1_IO10 */
            /* a. 使能GPIO1
            * set CCM to enable GPIO1
            * CCM_CCGR1[CG13] 0x20C406C
            * bit[27:26] = 0b11
            */
        *CCM_CCGR1 |= (3 << 26);

        /* b. 设置GPIO1_IO10用于GPIO
        * set IOMUXC_SW_MUX_CTL_PAD_JTAG_MOD
        *      to configure GPIO1_IO10 as GPIO
        * IOMUXC_SW_MUX_CTL_PAD_JTAG_MOD   Address: 20E_0000h base + 44h offset = 20E_0044h
        * bit[3:0] = 0b0101 alt5
        */
        val = *IOMUXC_SW_MUX_CTL_PAD_JTAG_MOD ;
        val &= ~(0xf);
        // val = 0b101;
        val |= 5;
        *IOMUXC_SW_MUX_CTL_PAD_JTAG_MOD = val;

        *GPIO1_GDIR |= (1 << 10);
    }
    return 0;
}
static int board_demo_led_ctl(int which, char status)
{
    printk("%s %s line %d, which: %d, status:%d\n", __FILE__, __FUNCTION__, __LINE__, which,status);
    if(which == 0)
    {
        if(status == 1)
        {
            *GPIO1_DR &= ~(1<<10);
        }else{
            *GPIO1_DR |= (1<<10);
        }
    }
    return 0;
}

static struct led_operations board_demo_led_opr = {
    .num = 1,
    .init = board_demo_led_init,
    .ctl  = board_demo_led_ctl,
};

struct led_operations* get_board_led_opr(void)
{
    return &board_demo_led_opr;
}

