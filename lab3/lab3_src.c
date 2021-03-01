/*
* ECE 477 Lab 3 File input & output
* @author Alyssa Wardwell
* @author Joseph Albert
*
*/

#include<stdio.h>
#include<wiringPi.h>
#include<unistd.h>

#define LOAD_AVG_MAX_RESOLUTION 4.0       // the min load_avg for all LEDs to be on


int main(void)
{
    FILE * load_file;
    char line[256];
    float temp_threshold = LOAD_AVG_MAX_RESOLUTION;
    float pin_thresholds[8];
    float load_avg;

    // onetime setup
    // setup for pin output
    wiringPiSetup();
    for (int pin = 0; pin <= 7; pin++)
        pinMode(pin, OUTPUT);
    

    /*
    * set thresholds to the minimum value for pins to turn on
    * uses temp_threshold to calculate the consecutively halving threshold
    */
    for (int i=7; i >= 0; i--)
    {
        pin_thresholds[i] = temp_threshold; 
        temp_threshold /= 2;
    } 


    while(1)
    {
        load_file = fopen("/proc/loadavg", "r");

        fgets(line, sizeof line, load_file);

        sscanf(line, "%f", &load_avg);

        fclose(load_file);
 
        /*
        * loop through pins and compare actual load average value to threshold
        * needed in order to turn pin on
        */
        for (int pin = 0; pin < 8; pin++)
        {
            if (load_avg >= pin_thresholds[pin])
                digitalWrite(pin, HIGH);
            else
                digitalWrite(pin, LOW);
        }
        
        sleep(1);
    }

    return 0;
}

