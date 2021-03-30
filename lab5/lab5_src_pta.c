#include<stdio.h>
#include<wiringPi.h>
#include<time.h>
#include<stdbool.h>

int init(void);

int main(void)
{
    int pin_state = 0;
    int cycles = 0;
    bool cont = 1;
    int x = 0;
    time_t start_time = time(NULL);

    init();
    while (cont)
    {
        if (pin_state == 0)
            pin_state = digitalRead(24);

        if (pin_state == 1)
        {
            pin_state = digitalRead(24);
            if (pin_state == 0)
                cycles++;
        }

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
    

}
