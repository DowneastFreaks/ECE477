/*
* ECE 477 Lab 3 File input & output
* @author Alyssa Wardwell
* @author Joseph Albert
*
*/

#include "RPI.h"
#include<stdio.h>
#include<unistd.h>

#define LOAD_AVG_MAX_RESOLUTION 4.0       // the min load_avg for all LEDs to be on

void set_pin_output(int pin);
int get_pin(int bcm_pin);
void turn_pin_on(int pin);
void turn_pin_off(int pin);

int main(void)
{
    FILE * load_file;
    char line[256];
    float temp_threshold = LOAD_AVG_MAX_RESOLUTION;
    float pin_thresholds[8];
    float load_avg;

    // onetime setup
    // setup for pin output

    if(map_peripheral(&gpio) == -1)
    {
        printf("Failed to map the physical GPIO registers into virtual memory space.\n");
        return -1;
    }

    for (int pin = 0; pin <= 7; pin++)
        set_pin_output(pin);
    

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
                turn_pin_on(pin);
            else
                turn_pin_off(pin);
        }
        
        sleep(1);
    }

    return 0;
}

void turn_pin_on(int pin)
{
    int bcm_pin = get_pin(pin);
    
    if (bcm_pin > 0)
    {
        // each bit corresponds to a pin, when the bit is on the pin turns on
        // setting the bit to 0 does not shut off the pin instead set a 1 to
        // the respective bit in the GPIO_CLR register
        GPIO_SET = 1 << bcm_pin;
    }
}

void turn_pin_off(int pin)
{
    int bcm_pin = get_pin(pin);

    if (bcm_pin > 0)
    {
        GPIO_CLR = 1 << bcm_pin;
    }

}


void set_pin_output(int pin)
{
    int bcm_pin = get_pin(pin);

    if (bcm_pin > 0)
    {
        // set pinmode as input to set bits corresponding to pin to 000
        // OUT_GPIO then sets the first bit within the subset of the register to 1
        INP_GPIO(bcm_pin);
        OUT_GPIO(bcm_pin);
    }

}

// This method maps the pins to the wiringPi numbering to avoid rewiring the pi when switching between modes
// return -1 on error
int get_pin(int bcm_pin)
{
    switch (bcm_pin)
    {
        case 0:
            return 17;
        case 1:
            return 18;
        case 2:
            return 27;
        case 3:
            return 22;
        case 4:
            return 23;
        case 5:
            return 24;
        case 6:
            return 25;
        case 7:
            return 4;
        default:
            return -1;
    }
}
