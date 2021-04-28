#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/eeprom.h>
#include <stdio.h>
#include <avr/sleep.h>
#include <string.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

void init(void);
void init_adc(void);
int read_adc(void);
void USART_Init(unsigned int ubrr);
void update_clock_speed(void);
int serial_putchar(char, FILE *);
int serial_getchar(FILE *);

static FILE serial_stream = FDEV_SETUP_STREAM(serial_putchar, serial_getchar, _FDEV_SETUP_RW);



void main(void)
{   
    int adc_value = 0;

    init();

    while(1)
    {
        adc_value = read_adc();
        printf("ADC:%d\n\r", adc_value);
        _delay_ms(1000);
    }
}

void init()
{
    update_clock_speed();
    USART_Init(MYUBRR);
    init_adc();
    _delay_ms(1000);
}

void init_adc()
{

    ADMUX = ((3<<REFS0) | 1 << MUX3);   //  use 1.1V as ref voltage and measure ADC8
    ADCSRA = (1<<ADEN) | (6 << ADPS0);  //  Enable ADC with prescaler = 64
    DIDR0 = 0;
    ADCSRB = 0;
}

int read_adc()
{
    ADCSRA |= (1<<ADSC);
    while (ADCSRA & (1<<ADSC));
    return (ADCH<<8)+ADCL;
}

void USART_Init(unsigned int ubrr)
{
    /* set baud rate */

    UBRR0H = (unsigned char) (ubrr>>8);
    UBRR0L = (unsigned char) ubrr;
    UCSR0A = 0;

    /* Enable receiver and transmitter */
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    /* Set frame format: 8data, 2stop bit */
    UCSR0C = (1<<USBS0)|(3<<UCSZ00);

    stdin = &serial_stream;
    stdout = &serial_stream;
}

int serial_getchar(FILE * fp)
{
    while((UCSR0A & (1<<RXC0)) == 0);

    return UDR0;
}

int serial_putchar(char val, FILE * fp)
{
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = val;
    return 0;
}

void update_clock_speed(void)
{
    char temp;
    char mod;

    temp = eeprom_read_byte((void *)1);

    if (temp==0 || temp==1)
    {
        mod = eeprom_read_byte((void *) 0);
        if (temp==0)
        {
            if (mod != 0xff) OSCCAL+=mod;
        }
        else
        {
            if (mod != 0xff) OSCCAL-=mod;
        }
    }
}
