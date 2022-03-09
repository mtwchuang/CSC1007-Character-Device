#include <stdio.h>  // default c library
#include <stdio.h>  // malloc, free, exit

int main()
{
    // create a memory buffer of 32 char space
    int checker;
    int takeinput = 1;
    char *memory_buffer = (char*) malloc(33);
    // File pointer to device location
	FILE *device_file = fopen("/dev/mychardev","r+");

    // print a message and take input from users
    char data[100];
    printf("Enter message, up to 32 characters to write into kernel");
    fgets(data,60,stdin);
    int num_valid = strcspn(data,"\r\n");
    printf("\n");
    fwrite(data, sizeof(char), num_valid, device_file);
    // write into the kernel

    // if input is enter or space? implement?
    // reads out from kernel
}