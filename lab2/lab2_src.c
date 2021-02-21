/*
* Lab 2 convert arguments to binary and display on 8 LEDs
*
*
*/


#include <stdio.h>
#include <math.h>

int main(int argc, char * argv[])
{
	int number;
	int gate;
	
	
	if (argc > 1 && sscanf(argv[1], "%i", &number))
	{
		
		for (int i = 7; i >= 0; i--)
		{
			gate = 0;
			if (i < 7)
			{
				for (int x = i + 2; x > i + 1; x--)
					gate += (pow(2, x-1) -1);
			}
			else
				gate = 255;
			

			if ((number & gate) >> i)
			{
				printf("Turn on LED: %d\n", i+1);
			}
			else
				printf("Turn off LED: %d\n", i + 1);
			
		}

	}

	return 0;
}


