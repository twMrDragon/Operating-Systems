#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/jiffies.h>
#include <linux/param.h>
#include <asm/uaccess.h>
#include <linux/version.h>

#define PROC_NAME "seconds"
static unsigned long start_jiffies;

ssize_t seconds_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos) {
    char buffer[64];
    int len;
    static int completed = 0;
    
    if (completed) {
        completed = 0;
        return 0;
    }
    completed = 1;
    
    len = sprintf(buffer, "%lu\n", (jiffies - start_jiffies) / HZ);
    copy_to_user(usr_buf, buffer, len);
    return len;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,6,0)
#define HAVE_PROC_OPS
#endif

#ifdef HAVE_PROC_OPS
static struct proc_ops seconds_fops = {
    .proc_read = seconds_read,
};
#else
static struct file_operations seconds_fops = {
    .owner = THIS_MODULE,
    .read = seconds_read,
};
#endif

int seconds_init(void){
    start_jiffies = jiffies;
    proc_create(PROC_NAME, 0666, NULL, &seconds_fops);
    return 0;
}

void seconds_exit(void){
    remove_proc_entry(PROC_NAME, NULL);
}

module_init(seconds_init);
module_exit(seconds_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Seconds Module");
