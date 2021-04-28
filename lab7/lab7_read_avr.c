#include <stdio.h>
#include <wiringSerial.h>
#include <errno.h>
#include <string.h>
#include <time.h>

int main()
{
    int fd;
    char write_value[100];
    FILE * serial_fp = NULL;
    FILE * log_fp = NULL;
    int reading = 0;
    time_t current_time;
    struct tm * timeinfo;

    if ((fd = serialOpen("/dev/ttyS0",9600)) < 0)
    {
        fprintf(stderr, "Unable to open serial device: %s", strerror(errno));
        return 1;
    }

    serial_fp = fdopen(fd, "r");

    while (1)
    {
        time (&current_time);
        timeinfo = localtime (&current_time);
        fscanf(serial_fp, "ADC:%d\n\r", &reading );
        log_fp = fopen("./temperatures.dat", "a");
        fprintf(log_fp, "%s\t%d\n\r", asctime(timeinfo),  reading);
        fclose(log_fp);
        
    }

}

