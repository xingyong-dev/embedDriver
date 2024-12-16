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

#include "led_opr.h"


static int major = 0 ;
static struct class *led_class;
struct led_operations *p_led_opr;


#define MIN(a, b) (a > b ? b : a)
int led_drv_open(struct inode *inode, struct file *file)
{
    
    int minor = iminor(inode);
    printk("%s %s line %d, minor: %d\n", __FILE__, __FUNCTION__, __LINE__, minor);
    p_led_opr->init(minor);
    return 0;
}
int led_drv_close(struct inode *inode, struct file *file)
{
    printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	return 0;
}
ssize_t led_drv_read (struct file *file, char __user *buf, size_t count, loff_t *pos)
{
    printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	return 0;
}
ssize_t led_drv_write(struct file *file, const char __user *buf, size_t count, loff_t *pos)
{
    int err;
    char status ;
    struct inode *inode = file_inode(file);
    int minor = iminor(inode);

    printk("%s %s line %d, minor: %d\n", __FILE__, __FUNCTION__, __LINE__, minor);
    err = copy_from_user(&status, buf, 1);
    p_led_opr->ctl(minor,status);
    return 0;

}
static struct file_operations led_drv = {
    .owner  = THIS_MODULE , 
    .open   = led_drv_open,
    .read   = led_drv_read,
    .write  = led_drv_write,
    .release = led_drv_close, 
};

static int __init led_init(void)
{
    int err = 0;
    int i ;

    major = register_chrdev(0, "forlinx_led", &led_drv);

    led_class = class_create(THIS_MODULE, "forlinx_led_class");
    err = PTR_ERR(led_class);
    if(IS_ERR(led_class))
    {
        printk("%s %s line %d, err: %d\n", __FILE__, __FUNCTION__, __LINE__, err);
        goto err4;
    }
    p_led_opr = get_board_led_opr();
    for(i = 0 ; i < p_led_opr->num; i++)
    {
        device_create(led_class, NULL , MKDEV(major, i), NULL, "forlinx_led%d",i);
    }
    
    return 0;
err4:
    unregister_chrdev(major, "forlinx_led");
    return 0;

    
}
static void __exit led_exit(void)
{
    int i ;
    printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);

    for(i = 0 ; i < p_led_opr->num; i++)
    {
        device_destroy(led_class, MKDEV(major, i));

    }
    device_destroy(led_class, MKDEV(major, 0));
    class_destroy(led_class);
    unregister_chrdev(major, "forlinx_led");

}

module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");





