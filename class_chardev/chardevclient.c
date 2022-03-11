/*
A simple C application program named chardevclient.c, to access the 
“character device” via the new read-only device driver, mychardev.c in the kernel. 
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    // fd is file directory
    int fd, c, rtn;
    fd = open("/dev/mychardev", O_RDWR);  // open the mychardev driver
    if (fd == -1)   // if fail to open the driver (the drive is not in kernel)
    {
        perror("open /dev/mychardev");
        exit(EXIT_FAILURE);
    }

    // Successfully open the mychardev driver
    printf("Reading from /dev/mychardev: \n");
    while ((rtn = read(fd, &c, 1)) > 0) // read info from the driver
    {
        printf("%c", c);
    }
    if (rtn == -1)  // fail to ready info
    {
        perror("reading /dev/mychardev");
    } 
    else 
    {
        printf("\n");
    }

    printf("Writing to /dev/mychardev: \n");  // write info into the driver
    c = 'h';
    while ((rtn = write(fd, &c, 1)) > 0) 
    {
        printf("wrote %c\n", c);
    }
    if (rtn == -1) // fail to write info
    {
        perror("writing /dev/mychardev");
    }

    exit(EXIT_SUCCESS);
}