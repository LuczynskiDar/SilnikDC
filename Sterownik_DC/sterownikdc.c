/*
 * Sterownik_DC.c
 *
 * Author : Dariusz Luczynski
 */ 



#include "constants.h"
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

//To set the two element LED  display
static const uint8_t PROGMEM DIRECTION[2]={0xC7, 0xAF};

// The direction indicator flag
static uint8_t dir_flag = 0xFF;

static uint8_t handler = 0;
// Direction flag indicator
volatile uint8_t pwm_flag=0xFF;

volatile uint8_t pwm_handler=102;

// Read pins indicator
volatile uint8_t pin_reader;

// Initialization of Timer1 for the PWM outputs
void timer1_initialize(void)
{

// To set the prescaler to divide the frequency per 8, resulting 488 Hz
	TCCR1B |= (1<<CS11); 
//  To set the Waveform Generator Mode to 8-bit Fast PWM
	TCCR1A |= (1<<WGM10);
	TCCR1B |= (1<<WGM12);
//  Set the PINs to zero, after the compare match event,
// when the minimum value is being reached 
	TCCR1A |= (1<<COM1A1) | (1<<COM1B1) ;	

// Set to the direction registers, PIN 4 and PIN5 
	DDRD |= (1<<PD4) | (1<<PD5);
	
//Initialize output compare registers
	OCR1A =	PWMDEF;
	OCR1B = PWMGND;	

}


//To chose from the tables a proper value for a proper LED display
//Inline do not recall the function just puts the code of the function there

void scan_key(void)
{
	uint8_t temp_dir = dir_flag;
	uint8_t temp_hand = handler;
	uint8_t temp_pwm = pwm_handler;
	
	uint8_t counter=0;
	if(counter==0)
	{
		pin_reader = ((~PIND) & 0x0F);
		// Set the column and calculate the key number
		if((pin_reader == 0x01) && ( (temp_dir & temp_hand) == 0))
		{
			temp_dir = 0xFF;
			temp_hand = 0x00;
		}
		else if((pin_reader ==0x02) && ( (dir_flag & temp_hand) == 0))
		{
			temp_dir = 0x00;
			temp_hand = 0xFF;
		}
		else if((pin_reader == 0x04) && ( temp_pwm > 0))
		{
			temp_pwm--;
		}
		else if((pin_reader == 0x08) && ( temp_pwm < 255))
		{
			temp_pwm++;

		}
		counter=100;
	} else counter--;
	
	dir_flag = temp_dir;
	handler = temp_hand;
	pwm_handler = temp_pwm;
}

void pwm_set(void)
{
	if (pwm_flag == 0xff) OCR1A = pwm_handler;
	if (pwm_flag == 0x00) OCR1B = pwm_handler;
}

void show_direction( void)

{
	uint8_t temp = 0xFF;
	uint8_t pwm_holder;
	PORTA = 0xFF;
	if(dir_flag == 0xFF)
	{
		temp = pgm_read_byte(&DIRECTION[0]);

		pwm_holder = OCR1A;
		DDRD &= ~(1<<PD4);
		DDRD = (1<<PD5);
		OCR1B = pwm_holder;
		pwm_flag = 0xFF;
	}
	if(dir_flag == 0x00)
	{
		temp = pgm_read_byte(&DIRECTION[1]);
		pwm_holder = OCR1B;
		DDRD &= ~(1<<PD5);
		DDRD = (1<<PD4);
		OCR1A = pwm_holder;
		pwm_flag = 0x00;
	}
	PORTA = temp;
}


int main(void)
{
    timer1_initialize();
	DDRA = 0xFF;
	PORTD |= 0x0F;
    while (1) 

    {
		scan_key();
		show_direction();
		pwm_set();
		_delay_ms(10);
    }
}

