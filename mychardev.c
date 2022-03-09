#include<linux/module.h>
#include<linux/init.h>
#include<linux/fs.h>        // functions that are related to device driver coding
#include<linux/slab.h>      // functions kfree, kmalloc
#include<linux/uaccess.h>   // functions copy_to_user, copy_from_user

MODULE_LICENSE("GPL");  // license type
MODULE_AUTHOR("Matthew C and Ming Le"); //authors
MODULE_DESCRIPTION("Read and Write Character Device");
MODULE_VERSION("0.1");

#define BUFFER_SIZE 1024                // buffer size
#define DEVICE_NAME "mychardev"         // device name

// function header prototypes
int mychardev_init(void);
void mychardev_exit(void);
static int mychardev_open (struct inode *pinode, struct file *pfile);
static int mychardev_close (struct inode *pinode, struct file *pfile);
static ssize_t mychardev_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset);
static ssize_t mychardev_write (struct file *pfile, const char __user *buffer, size_t length, loff_t *offset);

// global variables
static char *device_buffer;             // device/kernel buffer
static int number_times_read = 0;       // counter for times device has been read
static int number_times_written = 0;    // counter for times device has been written
static int major;

// aliasing file operations for the system calls
static struct file_operations chardev_fileops = 
{
    .read       = mychardev_read,
    .write      = mychardev_write,
    .open       = mychardev_open,
    .release    = mychardev_exit
};

/* functions for module loading/unloading*/
// function for module loading//
int mychardev_init(void)
{
    // print message to kernel log
    printk(KERN_INFO "Loading module mychardev: init: allocating device buffer and registering device");
    // allocating memory for device buffer
    device_buffer = (char *) kmalloc(BUFFER_SIZE, GFP_KERNEL);
    // register device as major number 240, name as mychardev and inputs file operations
	major = register_chrdev(240, DEVICE_NAME, &chardev_fileops);
    // registration checking
    if(major < 0)
    {
        printk(KERN_INFO "mychardev registration failed");
        return major;
    }
    return 0;
}

// function for module unloading
void mychardev_exit(void)
{
    // print message to kernel log
	printk(KERN_INFO "Unloading module mychardev: exit: freeing device buffer and unregistering device");
    // free memory for device buffer
    kfree(device_buffer);
    unregister_chrdev(240,DEVICE_NAME);
}

// points to corresponding init and exit functions
module_init(mychardev_init);
module_exit(mychardev_exit);

/* functions for device file opening/closing*/
// functions for device file opening
int mychardev_open (struct inode *pinode, struct file *pfile)
{
    // print message to kernel log
	printk(KERN_INFO "mychardev: file has been opened");
	return 0;
}
// functions for device file closing
int mychardev_close (struct inode *pinode, struct file *pfile)
{
    // print message to kernel log
	printk(KERN_INFO "mychardev: file has been opened");
	return 0;
}

/* functions for device file reading/writing*/
// function for device file reading
ssize_t mychardev_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{

    // calculate bytes available with buffer size subtract file offset
    int bytes_to_read;
    int bytes_available = (int) BUFFER_SIZE - (int) *offset;
    // increment counter on how many times device has been read from
    number_times_read++;
    // if message length bigger than available bytes
    if((int)length > bytes_available)
    {
        // bytes used to read would be max bytes available
        bytes_to_read = bytes_available;
    }
    // if message length is equal or smaller than bytes available, bytes to read would be length
    else
    {
        bytes_to_read = (int)length;
    }
    // call function to copy message from device buffer to user space
    copy_to_user(buffer, device_buffer + *offset, bytes_to_read);
    // print how many message bytes has been read
    printk(KERN_INFO "mychardev: read %d bytes from device", (int)bytes_to_read);
    // just read some characters, so need to adjust the offset
    *offset += bytes_to_read;
    return (int)bytes_to_read;
}

// function for device file writing
ssize_t mychardev_write (struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
    // increment counter on how many times device has been written to
    number_times_written++;
    // call function to copy message from user space to device buffer
    copy_from_user(device_buffer + *offset, buffer, length);
    // just wrote some characters, so need to adjust the offset
    *offset += length;
    // print how many message bytes has been written
	printk(KERN_INFO "mychardev: wrote %d bytes to device", (int)length);
	return length;
}