#include <linux/module.h>       // provides basic module utilties (author, licensing, description, init, exit)
#include <linux/kernel.h>       // provides basic kernel utilities (printk, KERN_INFO, KERN_ALERT)
#include <linux/fs.h>           // provides file structure (open,close,read,write) and character device registration/unregistration
#include <linux/cdev.h>         // provide cdev utitlies (cdev alloc, add, del) 
#include <linux/uaccess.h>      // provides data transfer techniques (copy_to_user, copy_from_user)

// function headers within charDev.c
int chardev_init(void);                                                         // declare chardev_int method
void chardev_exit(void);                                                        // declare chardev_exit method
static int device_open(struct inode *, struct file *);                          //declare device_open method
static int device_close(struct inode *, struct file *);                         //declare device_close method
static ssize_t device_read(struct file *, char *, size_t, loff_t *);            //declare device_read method
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);     //declare device_write method

// global variables and constants
#define BUFFER_SIZE 1024                        // used to define device_buffer size
#define DEVICENAME "charDev"                    // define device name
static char device_buffer[BUFFER_SIZE];         // memory buffer for the device, size defined by BUFFER_SIZE
static int read_counter;                        // counter for how many times read
static int write_counter;                       // counter for how many times written
struct cdev *mcdev;                             // name of the char driver to be registered*/
int major_number;                               // stores the major number extracted by dev_t
int ret;                                        // used to return values
dev_t dev_num;                                  // holds major number in dev_t format from kernel

// called when device is to be used for read/write. opens the device
static int device_open(struct inode *inode, struct file *filp) // inode being file on disk and file pointer
{
        printk(KERN_INFO "charDev : device opened succesfully\n"); // show that device has successfully been opened
        return 0; // let client know device successfully opened
}

// called when device is to be used to read from kernel
static ssize_t device_read(struct file *fp, char *buff, size_t length, loff_t *ppos) //file pointer, buffer, buffer length and seek position
{
        int maxbytes;                   //maximum bytes that can be read from ppos to BUFFER_SIZE
        int bytes_to_read;              // gives the number of bytes to read
        int bytes_read;                 // number of bytes actually read
        maxbytes = BUFFER_SIZE - *ppos; //determine maximum bytes 
        if (maxbytes > length)          // check if max bytes is more than length
                bytes_to_read = length; //if yes set bytes to read as length
        else
                bytes_to_read = maxbytes; //if no set bytes to read as maxbytes

        bytes_read = copy_to_user(buff, device_buffer + *ppos, bytes_to_read); //send read information to user and  determine if read succeeded or fail
        printk(KERN_INFO "charDev :  %d bytes has been read from kernel space\n", bytes_read);
        *ppos += bytes_read;            // change seek position to the end of bytes_read
        printk(KERN_INFO "charDev : Read %d times.", ++read_counter); //print number of times read function has been called into kernel

        return bytes_read; //return reading failed or successful
}

// called wen device is to be used to write into kernel
static ssize_t device_write(struct file *fp, const char *buff, size_t length, loff_t *ppos)     //file pointer, buffer, buffer length and offset position
{
        int maxbytes;                           // maximum bytes that can be read from ppos to BUFFER_SIZE
        int bytes_to_write;                     // gives the number of bytes to write
        int bytes_writen;                       // number of bytes actually writen
        maxbytes = BUFFER_SIZE - *ppos;         //determine maximum bytes 
        if (maxbytes > length)                  // check if max bytes is more than length
                bytes_to_write = length;        //if yes set bytes to read as length
        else
                bytes_to_write = maxbytes;      //if no set bytes to read as maxbytes

        bytes_writen = bytes_to_write - copy_from_user(device_buffer + *ppos, buff, bytes_to_write);    // takes write information from user and determine if write succeeded or fail
        printk(KERN_INFO "%d bytes has been written to kernel space\n", bytes_writen);                  // prints to kernel bytes written to kernel space
        *ppos += bytes_writen;                                                                          // change offset position to the end of bytes_written
        printk(KERN_INFO "charDev : Wrote %d times.", ++write_counter);                                 //print number of times write function has been called into kernel
        return bytes_writen;                                                                            //return writing failed or successful

}


static int device_close(struct inode *inode, struct file *filp) // inode being file on disk and file pointer
{
        return ret; // release successful
}

struct file_operations fops = {
        // these are the file operations provided by our driver 
        .owner = THIS_MODULE,   // prevents unloading when operations are in use
        .open = device_open,    // to open the device
        .write = device_write,  // to write to the device
        .read = device_read,    // to read the device
        .release = device_close,// to close the device
};

// runs on when character device module is loaded
int chardev_init(void) //on initializing
{
        /* we will get the major number dynamically this is recommended please read ldd3*/
        ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICENAME); //Allocates a range of char device numbers.
        if (ret < 0) { //check if allocation succeeded
                printk(KERN_ALERT " charDev : failed to allocate major number\n");
                return ret; // return allocation failed
        } else
                printk(KERN_INFO " charDev : mjor number allocated succesful\n");
        major_number = MAJOR(dev_num); //convert dev_t to int for storage
        printk(KERN_INFO "charDev : major number of our device is %d\n", major_number);
        printk(KERN_INFO "charDev : to use mknod /dev/%s c %d 0\n", DEVICENAME, major_number);

        mcdev = cdev_alloc();           // create, allocate and initialize our cdev structure, mcdev references to it
        mcdev->ops = &fops;             // defines structure operations as fops
        mcdev->owner = THIS_MODULE;     // prevents unloading when operations are in use

        ret = cdev_add(mcdev, dev_num, 1);//add device structure to kernel
        if (ret < 0) { // check if succeeded or failed
                printk(KERN_ALERT "charDev : device adding to the kerknel failed\n");
                return ret; //if failed
        } else
                printk(KERN_INFO "charDev : device additin to the kernel succesful\n");

        return 0; //return succeeded
}
// runs when character device module is exiting
void chardev_exit(void)
{
        cdev_del(mcdev); //removing the structure that we added previously

        unregister_chrdev_region(dev_num, 1); // unregister device numbers
        printk(KERN_ALERT " charDev : character driver is exiting\n");
}
// module description
MODULE_AUTHOR("CSC1007OS GROUP 7");
MODULE_DESCRIPTION("Q1 OF CS1007  PROJECT");

// points to corresponding init and exit functions
module_init(chardev_init);
module_exit(chardev_exit);
