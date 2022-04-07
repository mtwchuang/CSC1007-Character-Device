# character-device-csc1007 in C Programming
Develop a loadable kernel module and a user space application program to communicate between the kernel and user space using C language. The structure of the device driver should include these four functions: devi_open, devi_read, devi_write, devi_release. Device driver must be capable of allowing users to type in more than 10 times of messages, with correct messages plus length of characters on screen. 
## Basic functionalities
The functions are to 
-	enable users to enter random messages from user space application by the keyboard
-	user space application will count characters for entered sentences/messages
-	write into device driver with either memory buffer through write() system call
-	print into kernel space message how many times device has been written
-	user space application program invokes read() system call once enter key pressed
-	device driver should print into kernel space message of how many characters sent from kernel space into user space application
-	print into kernel space how many times device has been read

## Input for the Character Device
Input random message and press enter. Each time the console prompt user if he wants to continue (1 to continue, 2 to exit), enter 1. Repeat 10 times. After 10th time, when prompted to continue, enter 2, program will exit. 

## References
https://gist.github.com/ksvbka/0cd1a31143c1003ce6c7

https://embetronicx.com/tutorials/linux/device-drivers/cdev-structure-and-file-operations-of-character-drivers/
