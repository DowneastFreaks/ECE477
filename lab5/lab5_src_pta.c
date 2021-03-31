#include<stdio.h>
#include<wiringPi.h>
#include<time.h>
#include<stdbool.h>

// GLOBAL
int cycles = 0;

int init(void);
void increment_cycles(void);

int main(void)
{
    int pin_state = 0;
    bool cont = 1;
    int x = 0;


    init();
    time_t start_time = time(NULL);

    while (cont)
    {

        if ((time(NULL) - start_time) >= 10)
        {
            printf("%d cycles in the last %f seconds, or %dHz, which is %s\n", cycles, difftime(time(NULL), start_time), cycles / 10, ((cycles / 10)>100)? "too high": "too low");
            cycles = 0;
            cont = false;
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
