/*
* @author Alyssa Wardwell
* @author Joseph Albert
*
*/

#include<stdio.h>
#include<wiringPi.h>
#include<unistd.h>

void turn_on_leds(int max_led);

int main(void)
{
    FILE * load_file;
    char line[256];
    float load_avg;

    // setup for pin output
    wiringPiSetup();
    for (int pin = 0; pin <= 7; pin++)
        pinMode(pin, OUTPUT);
    
    while(1)
    {
        load_file = fopen("/proc/loadavg", "r");

        fgets(line, sizeof line, load_file);

        sscanf(line, "%f", &load_avg);

        printf("%d", load_avg);

        fclose(load_file);
 
        if (load_avg >=4)
            turn_on_leds(8);
        else if load_avg
        
        sleep(1);
    }
    return 0;
}

void turn_on_leds(int max_led)
{
    for (int pin = 0; pin < 8; pin++)
    {
        if (pin + 1 <= max_led)
            digitalWrite(pin, HIGH);
        else
            digitalWrite(pin, LOW);
    }
}

