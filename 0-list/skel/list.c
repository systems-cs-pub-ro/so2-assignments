#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

#include <linux/slab.h>
#include <linux/list.h>

#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#include <asm/uaccess.h>

MODULE_DESCRIPTION("List Data Structure in procfs");
MODULE_AUTHOR("Laura Vasilescu");
MODULE_LICENSE("GPL");

#define procfs_dir_name		"list"
#define procfs_file_read	"preview"
#define procfs_file_write	"management"

static struct proc_dir_entry *proc_list;
static struct proc_dir_entry *proc_list_read;
static struct proc_dir_entry *proc_list_write;

#define PROCFS_MAX_SIZE		1024

/* TODO: define your list! */

static int list_proc_show(struct seq_file *m, void *v)
{
	/* TODO: print your list. One element / line. */
	seq_puts(m, "Remove this line\n");

	return 0;
}

static int list_read_open(struct inode *inode, struct  file *file)
{
	return single_open(file, list_proc_show, NULL);
}

static int list_write_open(struct inode *inode, struct  file *file)
{
	return single_open(file, list_proc_show, NULL);
}

static ssize_t list_write(struct file *file, const char __user *buffer,
	size_t count, loff_t *offs)
{
	char local_buffer[PROCFS_MAX_SIZE];
	unsigned long local_buffer_size = 0;

	local_buffer_size = count;
	if (local_buffer_size > PROCFS_MAX_SIZE)
		local_buffer_size = PROCFS_MAX_SIZE;

	memset(local_buffer, 0, PROCFS_MAX_SIZE);
	if (copy_from_user(local_buffer, buffer, local_buffer_size))
		return -EFAULT;

	/* local_buffer contains your command written in /proc/list/management
	 * TODO: parse the command and add/delete elements.
	 */

	return local_buffer_size;
}

static const struct file_operations r_fops = {
	.owner		= THIS_MODULE,
	.open		= list_read_open,
	.read		= seq_read,
	.release	= single_release,
};

static const struct file_operations w_fops = {
	.owner		= THIS_MODULE,
	.open		= list_write_open,
	.write		= list_write,
	.release	= single_release,
};

static int list_init(void)
{
	proc_list = proc_mkdir(procfs_dir_name, NULL);
	if (!proc_list)
		return -ENOMEM;

	proc_list_read = proc_create(procfs_file_read, 0000, proc_list,
		&r_fops);
	if (!proc_list_read)
		goto proc_list_cleanup;

	proc_list_write = proc_create(procfs_file_write, 0000, proc_list,
		&w_fops);
	if (!proc_list_write)
		goto proc_list_read_cleanup;

	return 0;

proc_list_read_cleanup:
	proc_remove(proc_list_read);
proc_list_cleanup:
	proc_remove(proc_list);
	return -ENOMEM;
}

static void list_exit(void)
{
	proc_remove(proc_list);
}

module_init(list_init);
module_exit(list_exit);
