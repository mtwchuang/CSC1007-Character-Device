#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>   /* printk() */
#include <linux/slab.h>     /* kmalloc() */
#include <linux/fs.h>       /* everything... */
#include <linux/errno.h>    /* error codes */
#include <linux/types.h>    /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h>    /* O_ACCMODE */
#include <asm/uaccess.h>    /* copy_from/to_user */
MODULE_LICENSE("Dual BSD/GPL");

// Header prototypes of functions in memory.c
int memory_open(struct inode *inode, struct file *filp);
int memory_release(struct inode *inode, struct file *filp);
ssize_t memory_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);
ssize_t memory_write(struct file *filp,const char *buf, size_t count, loff_t *f_pos);
void memory_exit(void);
int memory_init(void);

// structure "file_operations" that defines the common file operations in "memory" driver
struct file_operations memory_fops = 
{
    read: memory_read,
    write: memory_write,
    open: memory_open,
    release: memory_release
};

/* Declaration of the init and exit functions */
module_init(memory_init);
module_exit(memory_exit);

// Global variables of the memory driver
int memory_major = 60;  // major number
char *memory_buffer;    // memory buffer to store data

// registration of the memory device, called when "memory" driver is loaded
int memory_init(void) 
{
    int result;
    // call register_chrdev to register the memory driver
    result = register_chrdev(memory_major, "memory", &memory_fops);
    // checks if "memory" device registration unsuccessful
    if (result < 0) 
    {
        // error message
        printk("<1>memory: cannot obtain major number %d\n", memory_major);
        return result;
    }
    // kernel memory allocation for the memory buffer
    memory_buffer = kmalloc(1, GFP_KERNEL);
    // check if memory allocation is unsuccessful
    if(!memory_buffer) 
    {
        // throws error code for no memory, jumps to fail clause
        result = -ENOMEM;
        goto fail;
    }
    // fills memory buffer with first 1 byte
    memset(memory_buffer, 0, 1);
    printk("<1>Inserting memory module\n");
    return 0;
    // clause for failed allocation, exits memory
    fail:
        memory_exit();
        return result;
}

// unregistration of the memory device, called when "memory" driver is unloaded
void memory_exit(void) 
{
    // unregister device, frees memory major
    unregister_chrdev(memory_major, "memory");
    // frees buffer memory
    if (memory_buffer) 
    {
        kfree(memory_buffer);
    }
    printk("<1>Removing memory module\n");
}

int memory_open(struct inode *inode, struct file *filp) {
 /* Success */
 return 0;
}

int memory_release(struct inode *inode, struct file *filp) {
 /* Success */
 return 0;
}

/*
    COMMENTS FROM MATT
    memory_read/write is design only for single char, anything other than that will fail
    offsets in read program only accounts for offset of one char
    read program doesn't even check if bytes available (buffer length - offset) is bigger than length
    both programs only return 1, return values determines how many char read/write
*/
// function to read from device
ssize_t memory_read(struct file *filp, char *buf, size_t count, loff_t *f_pos) 
{
    /* Transfering data to user space */
    copy_to_user(buf,memory_buffer,1);
    /* Changing reading position as best suits */
    if (*f_pos == 0) 
    {
        *f_pos+=1;
        return 1;
    } 
    else 
    {
        return 0;
    }
}

// function to write to a device
ssize_t memory_write( struct file *filp,const char *buf, size_t count, loff_t *f_pos) 
{
    const char *tmp;
    tmp=buf+count-1;
    // function transfer data from user space to kernel
    copy_from_user(memory_buffer,tmp,1);
    return 1;

}




