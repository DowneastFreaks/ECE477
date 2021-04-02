#include<stdio.h>
#include<stdlib.h>
#include<wiringPi.h>
#include<time.h>
#include<stdbool.h>

// GLOBAL
#define NUMBER_OF_MEASUREMENTS 6
#define TIME_OF_MEASUREMENT 10
int cycles = 0;

int init(void);
void increment_cycles(void);

int main(void)
{
    int pin_state = 0;
    int sum = 0;
    int average = 0;
    char command[512];
    int modifier = 0;
    bool cont = 1;
    int x = 0;
    unsigned char clock_speeds[NUMBER_OF_MEASUREMENTS];
    time_t start_time;

    init();

    while (average != 100)
    {
        sum = 0;

        // take ten samples
        for (int i = 0; i < NUMBER_OF_MEASUREMENTS;i++)
        {
            start_time = time(NULL);
            cycles = 0;

            while (difftime(time(NULL), start_time) < TIME_OF_MEASUREMENT)
                continue;

            clock_speeds[i] = cycles / TIME_OF_MEASUREMENT;
        }

        // average 10 samples
        for (int i = 0; i < NUMBER_OF_MEASUREMENTS;i++)
            sum += clock_speeds[i];

        average = sum / NUMBER_OF_MEASUREMENTS;

        if (average != 100)
        {
            printf("The ATMEGA88P clock is running at %dHz, which is %s\n", average, (average>100)? "too high": "too low");

            if (average > 100)
                modifier -= 1;
            else
                modifier += 1;

            sprintf(command, "avrdude -C ./avrdude_gpio.conf -c pi_1 -p atmega88p -U eeprom:w:%#04X,%#04X:m", abs(modifier), (modifier < 0)? 1:0);
            system(command);
            
        }
        else printf("Average is 100Hz!!\n");

    if (abs(modifier) > 10)
    {
        printf("dangerous modifier range\n");
        exit(0);
    }
    }
    return 0;
}

int init()
{
    wiringPiSetup();
    pinMode(24, INPUT);
    wiringPiISR(24, INT_EDGE_RISING, increment_cycles);

}

void increment_cycles(void)
{
    cycles++;
}
