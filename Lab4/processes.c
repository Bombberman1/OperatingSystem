#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/device/class.h>
#include <linux/fs.h>
#define MODULE_NAME "Processes_Module"
#define DEVICE_NAME "Processes_Driver"
#define DEVICE_CLASS "Processes_File"
#define NUM_SIZE 10
#define PAGE 4096


static struct class *process_class;
static struct device *process_device;
static int majorNumber;
pid_t wr_pid;
bool is_readed = false;


static int process_open(struct inode *this_inode, struct file *this_file) {
  pr_info("%s: %s - %s: %d\n", DEVICE_NAME, "Open Callback", __FUNCTION__, __LINE__);
  
  return 0;
}

static ssize_t process_write(struct file *this_file, const char __user *user_buffer, size_t count, loff_t *fpos) {
  pr_info("%s: %s - %s: %d, count: %lu\n", DEVICE_NAME, "Write Callback", __FUNCTION__, __LINE__, count);
  if(count >= NUM_SIZE) {
    return -ENOMEM;
  }

  char buff[NUM_SIZE];

  if(copy_from_user(buff, user_buffer, count)) {
    return -EFAULT;
  }
  buff[count] = 0;
  if(kstrtoint(buff, 10, &wr_pid)) {
    return -EINVAL;
  }
  
  if(wr_pid == 0) {
    wr_pid = task_pid_nr(current);
  }

  return count;
}

static ssize_t process_read(struct file *this_file, char __user *user_buffer, size_t count, loff_t *fpos) {
  pr_info("%s: %s - %s: %d, count: %lu\n", DEVICE_NAME, "Read Callback", __FUNCTION__, __LINE__, count);
  if(is_readed) {
    is_readed = false;
    return 0;
  }
  
  if(wr_pid == 0) {
    return -EINVAL;
  }

  struct task_struct *task;
  is_readed = true;
  
  rcu_read_lock();
  
  for_each_process(task) {
    if(task -> pid == wr_pid) {
      pr_info("%s: %s - PID:\t%d,\tState:\t%d,\tPrio:\t%d,\tVM Size:\t%lu,\tRSS:\t%lu\n",
        DEVICE_NAME,
        "PID info",
        task -> pid,
        task -> __state,
        task -> prio,
        task -> mm == NULL ? 0 : task -> mm -> total_vm * PAGE / 1024, 
        task -> mm == NULL ? 0 : task -> mm -> hiwater_rss * PAGE / 1024);
      
      char buffer[256];
      int length = snprintf(buffer, sizeof(buffer), "PID:\t%d,\tState:\t%d,\tPrio:\t%d,\tVM Size:\t%lu,\tRSS:\t%lu\n",
        task -> pid,
        task -> __state,
        task -> prio,
        task -> mm == NULL ? 0 : task -> mm -> total_vm * PAGE / 1024, 
        task -> mm == NULL ? 0 : task -> mm -> hiwater_rss * PAGE / 1024);
        
      if(copy_to_user(user_buffer, buffer, length)) {
        return -EFAULT;
      }
      
      rcu_read_unlock();
      
      return length;
    }
  }
  
  rcu_read_unlock();
  
  is_readed = false;
  
  return -EINVAL;
}

static int process_release(struct inode *this_inode, struct file *this_file) {
  pr_info("%s: %s - %s: %d\n", DEVICE_NAME, "Release Callback", __FUNCTION__, __LINE__); 
    
  return 0;
}

static const struct file_operations process_fops = {
  .owner = THIS_MODULE,
  .open = process_open,
  .write = process_write,
  .read = process_read,
  .release = process_release,
};

static int init_function(void) {
  pr_info("%s: %s - %s: %d\n", DEVICE_NAME, "INIT", __FUNCTION__, __LINE__);
  majorNumber = register_chrdev(0, DEVICE_NAME, &process_fops);
  process_class = class_create(DEVICE_CLASS);
  process_device = device_create(process_class, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
  
  struct task_struct *the_process;
  
  rcu_read_lock();
  
  for_each_process(the_process) {
    pr_info("%s\tPID:\t%d,\tState:\t%d,\tPrio:\t%d,\tVM Size:\t%lu,\tRSS:\t%lu\n",
      DEVICE_NAME,
      the_process -> pid,
      the_process -> __state,
      the_process -> prio,
      the_process -> mm == NULL ? 0 : the_process -> mm -> total_vm * PAGE / 1024, 
      the_process -> mm == NULL ? 0 : the_process -> mm -> hiwater_rss * PAGE / 1024);
  }
  
  rcu_read_unlock();

  return 0;
}

static void exit_function(void) {
  device_destroy(process_class, MKDEV(majorNumber, 0));
  class_unregister(process_class);
  class_destroy(process_class);
  unregister_chrdev(majorNumber, DEVICE_NAME);
  pr_info("%s: %s - %s: %d\n", DEVICE_NAME,  "EXIT", __FUNCTION__, __LINE__);
}

module_init(init_function);
module_exit(exit_function);

MODULE_AUTHOR("Oleg");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Process Id Device");

