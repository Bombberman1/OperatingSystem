#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h> 

#define MODULE_NAME "Oleg_Kernel"

static int init_function(void) {
	pr_info("%s: %s\n", MODULE_NAME, "Module created");
		
	return 0;
}

static void exit_function(void) {
	pr_info("%s: %s\n", MODULE_NAME, "Module deleted");
}

module_init(init_function);
module_exit(exit_function);

MODULE_AUTHOR("Oleg");
MODULE_LICENSE("GPL");
