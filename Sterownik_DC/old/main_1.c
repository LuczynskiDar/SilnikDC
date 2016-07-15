/*
 * Sterownik_DC.c
 *
 * Created: 2016-07-07 17:49:16
 * Author : w14571
 */ 

#define F_CPU 1000000UL 

#include <avr/pgmspace.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define KEYS 2
// Definition of default PWM, which is 40% duty cycle
#define PWMDEF 102
#define PWMGND 0

//To set the LED display
static const uint8_t PROGMEM DIRECTION[2]={0xC7, 0xAF};

#define LEDDISPNO 4 
	
//To set the LEFt on the LED display
static const uint8_t PROGMEM DIRECTIONL[LEDDISPNO]={0xC7, 0x86, 0x8E,0x87};
//To set the rGht on the LED dispaly
static const uint8_t PROGMEM DIRECTIONR[LEDDISPNO]={0xAF, 0xC2, 0x8B,0x87};

// To set a 4 element table
volatile uint8_t LEDDIGITS[LEDDISPNO] = {0 , 1, 2, 3};

// The direction indicator flag
static uint8_t dir_flag = 0xFF;

static uint8_t handler = 0;

volatile uint8_t pwm_flag=0xFF;

volatile uint8_t pwm_handler=102;

// flag to read pins
volatile uint8_t pin_reader;

// Initialization of Timer1 for the PWM outputs
void timer1_initialize(void)
{
	// Set the output pin direction
		// Fast PWM, 8-bit WGM10=1, WGM11=0,  WGM12=1, WGM13=0
	// PWM Phase Correct, 8-bit WGM10=1, WGM11=0,  WGM12=0, WGM13=0
	//TCCR1A=(1<<WGM10) | (1<<COM1A1) | (1<<COM1B1);
	
/*	DDRB |= (1<<DDB0) | (1<<DDB1);
	TCCR1A = (1<<WGM10) | (1<<COM1A1) | (1<<COM1B1);
	OCR1A = 255;
	OCR1B = 255;
	TCCR1B = (1<<CS10)|(1<<CS12);

*/
	
/*	
	DDRB |= (1<<PB0) | (1<<PB1); // 0C0A, 0c0b
	TCCR1A |= (1<<WGM10) | (1<<COM1A1) | (1<<COM1B1);
	OCR1A = 255;
	OCR1B = 255;
	TCCR1B = (1<<CS10);
	TCCR1B |= (1<<WGM20) | (1<<COM1A1) | (1<<COM1B1);
*/

/*
	DDRD |= 0x30;
	DDRB |= (1<<PB0) | (1<<PB1); // 0C0A, 0c0b
	
	TCCR1A = (1<<WGM10) | (1<<COM1A1) | (1<<COM1B1); 
	TCCR1B = (1<<WGM12) | (1<<CS10);
	
	OCR1A = 100;
	OCR1B = 100;
*/

 
/*	TCCR1A=(_BV(COM1A1) | _BV(COM1B1));
	OCR1A=128;
	OCR1B=255;
	TCCR1A|=_BV(WGM10);
	TCCR1B=_BV(WGM12) | _BV(CS10);	//Preskaler 1, FastPWM 8-bitowy
	DDRB|=(_BV(PB1) | _BV(PB2));

*/

/*  PWM-Beispiel für Mega16/32 (beiden haben den gleichen Timer)
	Benutzt wird Timer1 im Fast PWM Mode, 8 Bit Auflösung
	Die PWM-Signale liegen auf PD5/OC1A und PD4/OC1B
*/	

// 1. Den Prescaler einstellen, der die Frequenz festlegt
	TCCR1B |= (1<<CS12) | (1<<CS10); //Prescaler 256 i 1024
	
// 2. Den Timer in den Fast PWM Mode, 8 Bit schalten
//    ACHTUNG: Die WGM-Bits sind auf beide Konfigurationsregister verteilt!
	TCCR1A |= (1<<WGM10);
	TCCR1B |= (1<<WGM12);
	
// 3. Compare Output mode einstellen: Pin geht auf high bei Compare match, auf low bei Überlauf. 
//    Ergibt nichtinvertierte PWM.	
	TCCR1A |= (1<<COM1A1) | (1<<COM1B1) ;	

// 4. Die Pins als Ausgänge konfigurieren. Erst jetzt liegt das PWM-Signal an den Pins an!
	DDRD |= (1<<PD4) | (1<<PD5);
	
// In diesen Registern wird der gwünschte PWM-Wert abgelegt. Erlaubter Bereich: 0 bis 255.
	OCR1A =	PWMDEF;
	OCR1B = PWMGND;	

}

/*
// Initialization of the Timer0
void timer0_initialize(void)
{
	TCCR0=(1<<CS01);
	TIMSK|=(1<<TOIE0);
	TIMSK|=(1<<OCIE0);
	OCR0=255;
}
*/

//To chose from the tables a proper value for a proper LED display
//Inline do not recall the function just puts the code of the function there
/*
static inline void led_key_action(uint8_t val)
{
	// Use temp value, "ones" means that the LED display is not turned on
	uint8_t tmp=0xFF;
	
	if(((val & 0x7F)<4) && (dir_flag==0xFF)) tmp=pgm_read_byte(&DIRECTIONL[val & 0x7F]);
	if(((val & 0x7F)<4) && (dir_flag==0x00)) tmp=pgm_read_byte(&DIRECTIONR[val & 0x7F]);
	PORTA=tmp;	
	//pin_reader=PINB;
}
*/

void scan_key(void)
{
	uint8_t temp_dir = dir_flag;
	uint8_t temp_hand = handler;
	uint8_t temp_pwm = pwm_handler;
	
	uint8_t counter=0;
	if(counter==0)
	
	{
		uint8_t check = PIND;
		pin_reader = ((~PIND) & 0x0F);
		// Set the column and calculate the key number
		//if(( pin_reader & 0x0F)!=0x0F)
		//if((pin_reader & 0x0F)!=0x0F)
		//{
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
			//temp_pwm--;
			temp_pwm-=10;
		}
		else if((pin_reader == 0x08) && ( temp_pwm < 255))
		{
			temp_pwm+=10;
		}
		counter=100;
	} else counter--;
	//}
	dir_flag = temp_dir;
	handler = temp_hand;
	pwm_handler = temp_pwm;
}

void pwm_set(void)
{
	//OCR1A = 255;
	if (pwm_flag == 0xff) OCR1A = pwm_handler;
	if (pwm_flag == 0x00) OCR1B = pwm_handler;
}

void show_direction( void)
//void show_direction( uint8_t number)
{
	uint8_t temp = 0xFF;
	uint8_t pwm_holder;
	PORTA = 0xFF;
	if(dir_flag == 0xFF)
	{
		temp = pgm_read_byte(&DIRECTION[0]);
		//temp = pgm_read_byte(&DIRECTIONL[number]);
		pwm_holder = OCR1A;
		DDRD = (0<<PD4);
		DDRD = (1<<PD5);
		OCR1B = pwm_holder;
		pwm_flag = 0xFF;
	}
	if(dir_flag == 0x00)
	{
		temp = pgm_read_byte(&DIRECTION[1]);
		//temp = pgm_read_byte(&DIRECTIONR[number]);
		pwm_holder = OCR1B;
		DDRD = (0<<PD5);
		DDRD = (1<<PD4);
		OCR1A = pwm_holder;
		pwm_flag = 0x00;
	}
	PORTA = temp;
}


/*
ISR(TIMER0_OVF_vect)
{
	// To indicate current display and row of the keyboard, which are connected to the same line
	static uint8_t ledno;
	//volatile uint8_t pinB_reader;
	
	scan_key();
	// Turn ofF the LED display drivers drivers and set ones o columns
	PORTB|=0x0F;
	asm volatile("NOP"::);
	
	//PORTB|=0x3F;
	//pinB_reader=PINB;
	
	led_key_action(LEDDIGITS[ledno]);
	// Choose a next display and keep ones on the columns
	//PORTB = (~(1<<ledno));
	PORTB = (PORTB & 0xF0) | (~(1<<ledno) & 0x0F);
	asm volatile("NOP"::);
	show_direction( ledno );

	ledno=(ledno+1)%LEDDISPNO;
}

*/

int main(void)
{
    timer1_initialize();
	DDRA = 0xFF;
	PORTD |= 0x0F;
    while (1) 
	//sei();
    {
		scan_key();
		show_direction();
		pwm_set();
		_delay_ms(10);
    }
}

