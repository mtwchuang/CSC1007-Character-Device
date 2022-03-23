#include <stdio.h>      // default c library
#include <fcntl.h>      // provides functions access, 
#include <string.h>     // provides string utilities (strlen, memset)
#include <malloc.h>     // provides memory allocation utilties (malloc, free)

// constants
#define DEVICE "/dev/charDev"   // file directory to be written into or read from
// global variables
int  fd = 0;            // declare file pointer
int ppos = 0;           // declare seek position
int write_length = 0;   // declare length of bytes written 

int write_device()
{
        ssize_t ret; //declare to store return values
        char *data = (char *)malloc(1024 * sizeof(char)); // allocate maximum bytes allowed

        printf("please enter the data to write into device\n");
        scanf(" %[^\n]", data); // a space added after"so that it reads white space, %[^\n] is   addeed so that it takes input until new line
        write_length = strlen(data); // find the length of data
        ret = write(fd, data, write_length, &ppos); // write to kernel
        if (ret == - 1 )  // determine if write succeeds or fails
                printf("writting failed\n");
        else
                printf("writting success\n");
        free(data); // deallocate memory
        return 0;
}

int  read_device ()
{
        ssize_t ret; //declare to store return values
        char *data = (char *)malloc(1024 * sizeof(char)); //allocate maximum bytes allowed

        memset(data, 0, sizeof(data)); // to fill first sizeof data bytes of the memory pointed to by data with 0
        data[0] = '\0'; //signify end of string
        ret = read(fd, data, write_length, &ppos); // execute read function
        printf("DEVICE_READ : %s\n Byte size: %d", data,strlen(data)); // print data read from kernel
        if (ret == - 1 ) // check if reading succeeded or failed
                printf("reading failed\n");
        else
                printf("reading success\n");
        free(data); //deallocate memory
        return 0;
}



int main()
{
        int value = 0;// declare for switch case
        if (access(DEVICE, F_OK) == -1) { // check if module is successfully loaded
                printf("module %s not loaded\n", DEVICE);
                return 0;
        } else
                printf("module %s loaded, will be used\n", DEVICE);
        int run = 1; // declared to determine when to exit program
        while (run==1) {

                printf("please enter:\n\
                    \t 1 to write and read\n\
                    \t 2 to exit\n");
                scanf("%d", &value); //take in user input
                switch (value) { // decide what to do depending on user input
                case 1 :
                        fd = open(DEVICE, O_RDWR); // open file
                        write_device(); // execute write function
                        close(fd); //closing the device
                        fd = open(DEVICE, O_RDWR);// open file
                        read_device(); //execute read function
                        close(fd); /*closing the device*/
                        break;
                case 2 : 
                        run = 0;// end the while loop
                        break;
                default : printf("unknown  option selected, please enter right one\n"); // if user does not enter any input
                        break;
                }
        }
        return 0;
}