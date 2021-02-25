/*
* Lab 2 convert arguments to binary and display on 8 LEDs
*
* ECE477 Alyssa Wardwell and Joseph Albert
*/


#include <stdio.h>
#include <stdbool.h>
#include <wiringPi.h>

// Function Prototype
bool get_bit(int number, int index);

int main(int argc, char * argv[])
{
    int number;
    int gate;

    wiringPiSetup();

    for (int pin=0; pin <=7; pin++)
        pinMode(pin, OUTPUT);


    if (argc > 1 && sscanf(argv[1], "%i", &number))
    {
        for (int count = 0; count <= number; count ++)
	{
	    for (int i = 7; i >= 0; i--)
	    {
		if (get_bit(count, i))
		    digitalWrite(i, HIGH);
		else
		    digitalWrite(i, LOW);
	    }
	    delay(50);
	}
    }

    return 0;
}

bool get_bit(int number, int index)
{
    unsigned flag = 1;

    flag = flag << index;

    return number & flag;
}
