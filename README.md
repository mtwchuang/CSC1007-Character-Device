# character-device-csc1007 in C Programming
Project Requirements
- Develop Loadable Kernel Module and User Space Program
- Must consist of four device functions open, release, read, write

Functionalities
- enable users to enter any random messages from user space via keyboard
- user space application program counts characters for entered message
- allow users to type more than 10 times of messages

- entered message written into character device buffer through write() system call
- device driver also prints into kernel space message of how many characters received from user space
- print message device has been written %d times

- user space program waits for user keyboard input "enter/space"
- which invokves the read() system call next
- device driver should return received sentences/messages from device driver to user space
- device driver prints into kernel space message of number of characters sent from kernel to user space
- print message device has been read %d times into kernel space
- prints out received messages plus length of characters on screen