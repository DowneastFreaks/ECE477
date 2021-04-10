#!/bin/bash
avr-gcc -mmcu=atmega88 -Wl,-u,vfprintf -lprintf_flt -lm lab6_src_partA.c -o lab6.out
avr-objcopy -j .text -j .data -O ihex lab6.out lab6.hex
sudo avrdude -C avrdude_gpio.conf -c pi_1 -p atmega88p -U flash:w:lab6.hex:i

