/*
* Lab 4
* ECE477 Alyssa Wardwell and Joseph Albert
*/


#include <stdio.h>
#include <stdbool.h>
#include <wiringPi.h>

// Function Prototype
bool get_bit(int number, int index);

int main()
{
    // Initialize the wiringPi Library
    wiringPiSetup();

    int number;
    const int SWITCH_A_PIN = 23;
    const int SWITCH_B_PIN = 24;
    int refresh_rate = 1024;
    bool pin_state_a = 1;
    bool pin_state_b = 1;
    bool direction = 1;
    char led_value = 1;

   // Read input for each button
   pinMode(SWITCH_A_PIN, INPUT);
   pinMode(SWITCH_B_PIN, INPUT);

   pullUpDnControl(SWITCH_A_PIN, PUD_UP);
   pullUpDnControl(SWITCH_B_PIN, PUD_UP);

    for (int pin=0; pin <=7; pin++)
        pinMode(pin, OUTPUT);

    while(1)
    {
    	if (pin_state_a || (pin_state_a = digitalRead(SWITCH_A_PIN)))
    	{
    	    pin_state_a = digitalRead(SWITCH_A_PIN);
    	    if (!pin_state_a && refresh_rate > 32)
    		refresh_rate/=2;
    	    else if (!pin_state_a)
    		direction = !direction;
    	}

        if (pin_state_b || (pin_state_b = digitalRead(SWITCH_B_PIN)))
    	{
    	    pin_state_b = digitalRead(SWITCH_B_PIN);
    	    if (!pin_state_b && refresh_rate < 1024)
    		refresh_rate *=2;
    	    else if (!pin_state_b)
    		direction = !direction;
    	}

    	if (!pin_state_a && !pin_state_b)
    	    return (0);

    	if (direction)
    	{
    	    led_value <<= 1;
    	    led_value = (led_value==0) ? 1:led_value;
    	}
    	else
    	{
    	    led_value >>= 1;
    	    led_value = (led_value==0) ? 128:led_value;
    	}

    	//Display led_value
    	for (int i = 7; i >= 0; i--)
    	{
    	    if (get_bit(led_value, i))
    		digitalWrite(i, HIGH);
    	    else
    		digitalWrite(i, LOW);
    	}
    	delay(refresh_rate);
    }
    
}

bool get_bit(int number, int index)
{
    unsigned flag = 1;

    flag = flag << index;

    return number & flag;
}
