#include <stdio.h>
#include <wiringSerial.h>
#include <errno.h>
#include <string.h>

int main()
{
    int fd;
    
    if ((fd = serialOpen("/dev/ttyS0",9600)) < 0)
    {
        fprintf(stderr, "Unable to open serial device: %s", strerror(errno));
        return 1;
    }

    serialPrintf(fd, "Start\n\r");

    while (1)
    {
        putchar(serialGetchar(fd));
        fflush(stdout);
    }

}

