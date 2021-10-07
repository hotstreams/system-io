#include <linux/init.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/device.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Eddie");
MODULE_DESCRIPTION("lab1");
MODULE_VERSION("1.0.0");

struct dev_info_ {
    dev_t first;
    struct cdev c_dev;
    struct class *cl;

} dev_info;

int create_dev(void);
void delete_dev(void);

int sum_count = 0;
int* sums = NULL;
ssize_t dev_read(struct file* f, char __user* ubuf, size_t count, loff_t* ppos);
ssize_t dev_write(struct file* f, const char __user* ubuf, size_t count, loff_t* ppos);

struct proc_dir_entry* entry;
ssize_t proc_write(struct file* f, const char __user* ubuf, size_t count, loff_t* ppos);
ssize_t proc_read(struct file* f, char __user* ubuf, size_t count, loff_t* ppos);

int __init init_(void) {
    if (create_dev() == -1) {
        printk(KERN_INFO "FAILED TO CREATE DEV\n");
        return -1;
    }
    return 0;
}

void __exit exit_(void) {
    delete_dev();
}

int dev_open(struct inode* i, struct file* f) {
    printk(KERN_NOTICE "dev_open\n");
    return 0;
}

int dev_close(struct inode* i, struct file* f) {
    printk(KERN_NOTICE "dev_close\n");
    return 0;
}

ssize_t dev_read(struct file* f, char __user* ubuf, size_t count, loff_t* ppos) {
    printk(KERN_NOTICE "dev_read\n");
    size_t len = strlen(THIS_MODULE->name);
    if (*ppos > 0 || count < len) {
        return 0;
    }

    char* buf = (char*)kmalloc(sizeof(char) * 512, GFP_KERNEL);
    size_t llen = 0;

    size_t i = 0;
    for (; i < sum_count; ++i) {
        llen += sprintf(buf + llen, "%d\n", sums[i]);
    }

    printk(KERN_NOTICE "BUFFER = \n %s \n", buf);

    if (copy_to_user(ubuf, THIS_MODULE->name, len) != 0) {
        return -EFAULT;
    }

    *ppos += len;

    kfree(buf);
    return len;
}

ssize_t dev_write(struct file* f, const char __user* ubuf, size_t count, loff_t* ppos) {
    printk(KERN_NOTICE "dev_write\n");

    char dig[10] = {0};
    int d_count = 0;
    int sum = 0;
    int num = 0;
    char c;

    size_t i = 0;
    for (; i < count; ++i) {
        if (copy_from_user(&c, ubuf + i, 1) != 0) {
            return -EFAULT;
        } else {
            if (c >= '0' && c <= '9') {
                dig[d_count++] = c;
            } else {
                if (d_count > 0) {
                    kstrtoint(dig, 10, &num);
                    memset(dig, 0, 10);
                    d_count = 0;
                    sum += num;
                }
            }
        }
    }

    sums[sum_count++] = sum;

    printk(KERN_NOTICE "dev write = %d\n", sum);
    return count;
}

const struct file_operations dev_fops = {
    .owner = THIS_MODULE,
    .open = dev_open,
    .release = dev_close,
    .read = dev_read,
    .write = dev_write
};

const struct proc_ops proc_fops = {
    .proc_read = proc_read,
    .proc_write = proc_write
};

char* set_devnode(struct device* dev, umode_t* mode) {
    if (mode != NULL) {
        *mode = 0666;
    }
    return NULL;
}

int create_dev(void) {
    printk(KERN_NOTICE "create dev\n");
    if (alloc_chrdev_region(&dev_info.first, 0, 1, "ch_dev") < 0) {
        return -1;
    }

    if ((dev_info.cl = class_create(THIS_MODULE, "chardrv")) == NULL) {
        unregister_chrdev_region(dev_info.first, 1);
        return -1;
    }

    dev_info.cl->devnode = set_devnode;

    if (device_create(dev_info.cl, NULL, dev_info.first, NULL, "var3") == NULL) {
        class_destroy(dev_info.cl);
        unregister_chrdev_region(dev_info.first, 1);
        return -1;
    }

    cdev_init(&dev_info.c_dev, &dev_fops);

    if (cdev_add(&dev_info.c_dev, dev_info.first, 1) == -1) {
        device_destroy(dev_info.cl, dev_info.first);
        class_destroy(dev_info.cl);
        unregister_chrdev_region(dev_info.first, 1);
        return -1;
    }
    printk(KERN_NOTICE "dev created\n");

    sums = (int*)kmalloc(sizeof(int) * 512, GFP_KERNEL);
    if (!sums) {
        printk(KERN_NOTICE "Failed to allocate mem!\n");
        return -1;
    }

    entry = proc_create("var3", 0444, NULL, &proc_fops);
    if (!entry) {
        return -ENOMEM;
    }

    return 0;
}

void delete_dev(void) {
    printk(KERN_NOTICE "delete dev\n");
    cdev_del(&dev_info.c_dev);
    device_destroy(dev_info.cl, dev_info.first);
    class_destroy(dev_info.cl);
    unregister_chrdev_region(dev_info.first, 1);
    printk(KERN_NOTICE "dev deleted\n");

    kfree(sums);
    proc_remove(entry);
}

ssize_t proc_write(struct file* f, const char __user* ubuf, size_t count, loff_t* ppos) {
    printk(KERN_NOTICE "proc write\n");
    return -1;
}

ssize_t proc_read(struct file* f, char __user* ubuf, size_t count, loff_t* ppos) {
    printk(KERN_NOTICE "proc read\n");

    char* buf = (char*)kmalloc(sizeof(char) * 512, GFP_KERNEL);
    size_t len = 0;

    size_t i = 0;
    for (; i < sum_count; ++i) {
        len += sprintf(buf + len, "%d\n", sums[i]);
    }

    if (*ppos > 0 || count < len) {
        return 0;
    }

    if (copy_to_user(ubuf, buf, len) != 0) {
        return -EFAULT;
    }
    *ppos += len;

    kfree(buf);
    return len;
}

module_init(init_);
module_exit(exit_);