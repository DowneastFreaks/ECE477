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
    char buffer[100] = "notStart";
    int adc_voltage = 0;

    init();
    while(strncmp("Start", buffer, strlen("Start"))!=0) fgets(buffer, 100, stdin);

    while(1)
    {
        adc_voltage = read_adc();
        printf("The power rail is approximately %.6fV\n\r", (0x400 * 1.1)/ adc_voltage);
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

    ADMUX = ((1<<REFS0) | 0xE);   //  use AVCC as ref voltage and measure 1.1V rail
    ADCSRA = (1<<ADEN) | (6 << ADPS0);  //  Enable ADC with prescaler = 64
}

int read_adc()
{
    ADCSRA |= (1<<ADSC);
    while (ADCSRA & (1<<ADSC));
    return ADC;
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
