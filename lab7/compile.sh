#!/bin/bash
avr-gcc -mmcu=atmega88 lab7_src.c -o lab7.out
avr-objcopy -j .text -j .data -O ihex lab7.out lab7.hex
sudo avrdude -C avrdude_gpio.conf -c pi_1 -p atmega88p -U flash:w:lab7.hex:i

