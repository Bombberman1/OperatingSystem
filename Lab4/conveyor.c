#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/device/class.h>
#include <linux/fs.h>
#define MODULE_NAME "Conveyor_Module"
#define DEVICE_NAME "Conveyor_Driver"
#define DEVICE_CLASS "Conveyor_File"
#define SIZE 20


struct class *conveyor_class;
static struct device *conveyor_device;
static int majorNumber;
char kernel_buffer[SIZE];
loff_t write_pos = 0;
bool ovf = false;
loff_t read_pos = 0;
bool is_readed = false;
bool data_available = false;


static int conveyor_open(struct inode *this_inode, struct file *this_file) {
  pr_info("%s: %s - %s: %d\n", DEVICE_NAME, "Open Callback", __FUNCTION__, __LINE__);

  return 0;
}

static ssize_t conveyor_write(struct file *this_file, const char __user *user_buffer, size_t count, loff_t *fpos) {
  pr_info("%s: %s - %s: %d, count: %lu\n", DEVICE_NAME, "Write Callback", __FUNCTION__, __LINE__, count);
  if(count > SIZE) {
    return -ENOMEM;
  }

  if(data_available) {
    return -ENOMEM;
  }

  if(write_pos + count >= SIZE) {
    write_pos = 0;
    ovf = true;
  }

  if(copy_from_user(kernel_buffer + write_pos, user_buffer, count)) {
    return -EFAULT;
  }

  write_pos += count;
  data_available = true;
    
  return count;
}

static ssize_t conveyor_read(struct file *this_file, char __user *user_buffer, size_t count, loff_t *fpos) {
  pr_info("%s: %s - %s: %d, count: %lu\n", DEVICE_NAME, "Read Callback", __FUNCTION__, __LINE__, count);
  if(is_readed) {
    is_readed = false;
    return 0;
  }
  
  if(read_pos == write_pos && !data_available) {
    return -EIO;
  }

  is_readed = true;
  if(read_pos + count >= write_pos) {
    if(ovf) {
      count = write_pos;
      read_pos = 0;
    } else {
      count = write_pos - read_pos;
    }
  }

  if(copy_to_user(user_buffer, kernel_buffer + read_pos, count)) {
    return -EFAULT;
  }

  read_pos += count;
  data_available = false;
  
  return count;
}

static int conveyor_release(struct inode *this_inode, struct file *this_file) {
  pr_info("%s: %s - %s: %d\n", DEVICE_NAME, "Release Callback", __FUNCTION__, __LINE__); 

  return 0;
}

static const struct file_operations conveyor_fops = {
  .owner = THIS_MODULE,
  .open = conveyor_open,
  .write = conveyor_write,
  .read = conveyor_read,
  .release = conveyor_release,
};

static int init_function(void) {
  pr_info("%s: %s - %s: %d\n", DEVICE_NAME, "INIT", __FUNCTION__, __LINE__);
  majorNumber = register_chrdev(0, DEVICE_NAME, &conveyor_fops);
  conveyor_class = class_create(DEVICE_CLASS);
  conveyor_device = device_create(conveyor_class, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);

  return 0;
}

static void exit_function(void) {
  device_destroy(conveyor_class, MKDEV(majorNumber, 0));
  class_unregister(conveyor_class);
  class_destroy(conveyor_class);
  unregister_chrdev(majorNumber, DEVICE_NAME);
  pr_info("%s: %s - %s: %d\n", DEVICE_NAME,  "EXIT", __FUNCTION__, __LINE__);
}


module_init(init_function);
module_exit(exit_function);

MODULE_AUTHOR("Oleg");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Conveyor device");
