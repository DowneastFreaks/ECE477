#include <stdio.h>
#include <wiringSerial.h>
#include <errno.h>
#include <string.h>

int main()
{
    int fd;
    char write_value[100];
    FILE * serial_fp = NULL;
    FILE * log_fp = NULL;
    float reading = 0;

    if ((fd = serialOpen("/dev/ttyS0",9600)) < 0)
    {
        fprintf(stderr, "Unable to open serial device: %s", strerror(errno));
        return 1;
    }

    serial_fp = fdopen(fd, "r");

    serialPrintf(fd, "Start\n\r");

    while (1)
    {
        fscanf(serial_fp, "The power rail is approximately %fV\n", &reading );
        log_fp = fopen("./rail_voltages.dat", "a");
        fprintf(log_fp, "%.6f\n\r", reading);
        fclose(log_fp);
        
    }

}

