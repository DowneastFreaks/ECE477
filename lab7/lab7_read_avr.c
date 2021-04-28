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
    int reading = 0;
    struct tm * timeinfo;

    if ((fd = serialOpen("/dev/ttyS0",9600)) < 0)
    {
        fprintf(stderr, "Unable to open serial device: %s", strerror(errno));
        return 1;
    }

    serial_fp = fdopen(fd, "r");

    while (1)
    {
        fscanf(serial_fp, "ADC:%d\n\r", &reading );
        log_fp = fopen("./temperatures.dat", "a");
        fprintf(log_fp, "%d\n\r", reading);
        fclose(log_fp);
        
    }

}

