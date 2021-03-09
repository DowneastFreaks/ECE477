/*
* Lab 4
* ECE477
* @author Alyssa Wardwell
* @author Joseph Albert
*
* Lit LED moves across 8 LEDs
* Switch A increases speed, Switch B decreases speed, A + B closes program
* When max and min speed are reached, maxed switch will reverse direction of LED movement.
*/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wiringPi.h>

// Constants
#define SWITCH_PIN_A 23
#define SWITCH_PIN_B 24

// Structs
typedef struct led_parameters
{
    bool direction;
    int refresh_rate;
    unsigned char value;
} LedParameters;

// Function Prototypes
void switch_a_pressed();
void switch_b_pressed();
void init(void);
void display_leds(unsigned char value);
bool get_bit(int number, int index);
bool confirm_low_signal(unsigned pin);
LedParameters update_parameters(LedParameters param, unsigned switch_pin);
LedParameters update_led_value(LedParameters param);


// GLOBAL VARIABLES
volatile bool pin_a_state = 0;
volatile bool pin_b_state = 0;


int main()
{
    LedParameters led_settings = {1, 1024, 0};  // direction, refresh_rate, value

    init();


    while(1)
    {
        // update direction and refresh rate if button is pushed
    	if (pin_a_state || pin_b_state)
            led_settings = (pin_a_state) ? update_parameters(led_settings, SWITCH_PIN_A): update_parameters(led_settings, SWITCH_PIN_B);


        // update value to be displayed on LEDs
        led_settings = update_led_value(led_settings);

        display_leds(led_settings.value);

    	delay(led_settings.refresh_rate);
    }
}


/*
* One time setup of hardware
*/
void init(void)
{
    wiringPiSetup();

    pinMode(SWITCH_PIN_A, INPUT);
    pinMode(SWITCH_PIN_B, INPUT);

    pullUpDnControl(SWITCH_PIN_A, PUD_UP);
    pullUpDnControl(SWITCH_PIN_B, PUD_UP);

    wiringPiISR(SWITCH_PIN_A, INT_EDGE_FALLING, &switch_a_pressed);
    wiringPiISR(SWITCH_PIN_B, INT_EDGE_FALLING, &switch_b_pressed);

    for (int pin = 0; pin < 8; pin++)
        pinMode(pin, OUTPUT);
}


/*
* Interrupt triggered when switch A pressed
* if low signal is confirmed set pin_a_state to true
* shuts off method if switch B is also pressed
*/
void switch_a_pressed()
{
    bool event_triggered = 0;

    event_triggered = confirm_low_signal(SWITCH_PIN_A);

    if (event_triggered)
    {
        pin_a_state = 1;

        if (!digitalRead(SWITCH_PIN_B))
        {
            display_leds(0);
            exit(0);
        }
    }
}


/*
* Interrupt triggered when switch B pressed
* if low signal is confirmed set pin_b_state to true
* shuts off method if switch A is also pressed
*/
void switch_b_pressed()
{
    bool event_triggered;

    event_triggered = confirm_low_signal(SWITCH_PIN_B);

    if (event_triggered)
    {
        pin_b_state = 1;

        if (!digitalRead(SWITCH_PIN_A))
        {
            display_leds(0);
            exit(0);
        }
    }
}


/*
* Turn on LEDs to match binary representation of value
*/
void display_leds(unsigned char value)
{
    for (int i = 7; i >= 0; i--)
    {
        if (get_bit(value, i))
        digitalWrite(i, HIGH);
        else
        digitalWrite(i, LOW);
    }
}


/*
* Update direction and refresh_rate of LEDs based on current values and
* which switch was pushed.
*/
LedParameters update_parameters(LedParameters param, unsigned switch_pin)
{
    // Reset global variables to 0
    pin_a_state = pin_b_state = 0;

    if (switch_pin == SWITCH_PIN_A)
    {
        if (32 == param.refresh_rate)
            param.direction = !param.direction;
        else
            param.refresh_rate >>= 1;

    }
    else
    {
        if (1024 == param.refresh_rate)
            param.direction = !param.direction;
        else
            param.refresh_rate <<= 1;
    }

    return param;
}


/*
* Uses current LedParameters to determine the next value in the sequence
*/
LedParameters update_led_value(LedParameters param)
{
    unsigned char led_value = param.value;

    if (param.direction)
    {
        led_value <<= 1;
        led_value = (led_value==0) ? 1:led_value;
    }
    else
    {
        led_value >>= 1;
        led_value = (led_value==0) ? 128:led_value;
    }

    param.value = led_value;

    return param;

}


/*
* Returns the state of a bit in a number
*/
bool get_bit(int number, int index)
{
    unsigned flag = 1;

    flag = flag << index;

    return number & flag;
}


/*
* Function to debounce signal from button
*/
bool confirm_low_signal(unsigned pin)
{
    unsigned char buffer = 0;

    for (int i = 0; i < 16; i++)
    {
        buffer <<= 1;
        buffer |= digitalRead(pin);

        if (i > 7 && 0x00 == buffer)
            return 1;

        delay(3);
    }

    return 0;

}
