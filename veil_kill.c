#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>

MODULE_LICENSE("Dual BSD/GPL");

static int vk_write(struct file *file, const char __user *buffer,
			   unsigned long count, void *data)
{
	printk(KERN_ALERT "VK Write called!\n");
	return 0;
}

static struct proc_dir_entry* vk_overwrite_entry(struct proc_dir_entry *entry)
{
	struct proc_dir_entry *parent, *tmp;

	parent = entry->parent;

	/*
	 * Walk LL looking for a write function to 
	 * replace
	 */
	for(tmp = parent->subdir; tmp; tmp = tmp->next) {
		if(tmp == entry)
			continue;
		if(tmp->write_proc) {
			tmp->write_proc = &vk_write;
			return tmp;
		}
	}

	return NULL;
}

static void __init proc_init(void)
{
	struct proc_dir_entry *temp_entry, *replaced_entry;

	/*static const struct file_operations vk_operations = {
		.write = vk_write;
	}*/

	/*
	 * Create temp proc entry to get pointer to a 
	 * proc_dir_entry
	 */
	temp_entry = proc_create_data("temp_entry", 0, NULL, NULL, NULL);

	/*
	 * Now in PDE LL, scan list to replace write function of
	 * an entry with my write function
	 */
	replaced_entry = vk_overwrite_entry(temp_entry);

	if(replaced_entry)
		printk(KERN_ALERT "Replaced Entry %s\n", replaced_entry->name);
	else
		printk(KERN_ALERT "No proc entry found for write replacement!\n");
}

static int __init vk_init(void)
{
	printk(KERN_ALERT "VK Init\n");
	
	/*
	 * Replace fop of a proc dir entry with my 
	 * function, ideally want to replace a write_proc
	 * (more intuitive to me)
	 */
	proc_init();
	return 0;
}

static void __exit vk_exit(void)
{
	printk(KERN_ALERT "VK Exit\n");
}

module_init(vk_init);
module_exit(vk_exit);
