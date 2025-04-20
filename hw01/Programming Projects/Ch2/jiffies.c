#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/jiffies.h>
#include <asm/uaccess.h>
#include <linux/version.h>

#define PROC_NAME "jiffies"

ssize_t jiffies_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos) {
    char buffer[64];
    int len;
    static int completed = 0;
    
    if (completed) {
        completed = 0;
        return 0;
    }
    completed = 1;
    
    len = sprintf(buffer, "%lu\n", jiffies);
    copy_to_user(usr_buf, buffer, len);
    return len;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,6,0)
#define HAVE_PROC_OPS
#endif

#ifdef HAVE_PROC_OPS
static struct proc_ops jiffies_fops = {
    .proc_read = jiffies_read,
};
#else
static struct file_operations jiffies_fops = {
    .owner = THIS_MODULE,
    .read = jiffies_read,
};
#endif

int jiffies_init(void){
    proc_create(PROC_NAME, 0666, NULL, &jiffies_fops);
    return 0;
}

void jiffies_exit(void){
    remove_proc_entry(PROC_NAME, NULL);
}

module_init(jiffies_init);
module_exit(jiffies_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Jiffies Module");