	
////FAILED CREATED DROPS IN RAMP SUCH THAT IT COUNTED LIKE THIS 
	//1234 2345 3456 4567
/*
 * GccApplication1.c
 *
 * Created: 1/9/2019 10:25:38 AM
 * Author : Davis Polito
 */ 
#define F_CPU 8000000
#define SINE 0
#define SAW 1
#define RSAW 2
#define TRI 3
#define SQUAREP 4
#define SH 5
#define SQUAREN 6
#define SQUAREF 7
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h> 
#include <avr/eeprom.h>
#define DEADBAND 10
#define CYCLE_US 300

const uint8_t  sinewave[] PROGMEM= //256 values
{
	0x80,0x83,0x86,0x89,0x8c,0x8f,0x92,0x95,0x98,0x9c,0x9f,0xa2,0xa5,0xa8,0xab,0xae,
	0xb0,0xb3,0xb6,0xb9,0xbc,0xbf,0xc1,0xc4,0xc7,0xc9,0xcc,0xce,0xd1,0xd3,0xd5,0xd8,
	0xda,0xdc,0xde,0xe0,0xe2,0xe4,0xe6,0xe8,0xea,0xec,0xed,0xef,0xf0,0xf2,0xf3,0xf5,
	0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfc,0xfd,0xfe,0xfe,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xfe,0xfe,0xfd,0xfc,0xfc,0xfb,0xfa,0xf9,0xf8,0xf7,
	0xf6,0xf5,0xf3,0xf2,0xf0,0xef,0xed,0xec,0xea,0xe8,0xe6,0xe4,0xe2,0xe0,0xde,0xdc,
	0xda,0xd8,0xd5,0xd3,0xd1,0xce,0xcc,0xc9,0xc7,0xc4,0xc1,0xbf,0xbc,0xb9,0xb6,0xb3,
	0xb0,0xae,0xab,0xa8,0xa5,0xa2,0x9f,0x9c,0x98,0x95,0x92,0x8f,0x8c,0x89,0x86,0x83,
	0x80,0x7c,0x79,0x76,0x73,0x70,0x6d,0x6a,0x67,0x63,0x60,0x5d,0x5a,0x57,0x54,0x51,
	0x4f,0x4c,0x49,0x46,0x43,0x40,0x3e,0x3b,0x38,0x36,0x33,0x31,0x2e,0x2c,0x2a,0x27,
	0x25,0x23,0x21,0x1f,0x1d,0x1b,0x19,0x17,0x15,0x13,0x12,0x10,0x0f,0x0d,0x0c,0x0a,
	0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x03,0x02,0x01,0x01,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x02,0x03,0x03,0x04,0x05,0x06,0x07,0x08,
	0x09,0x0a,0x0c,0x0d,0x0f,0x10,0x12,0x13,0x15,0x17,0x19,0x1b,0x1d,0x1f,0x21,0x23,
	0x25,0x27,0x2a,0x2c,0x2e,0x31,0x33,0x36,0x38,0x3b,0x3e,0x40,0x43,0x46,0x49,0x4c,
	0x4f,0x51,0x54,0x57,0x5a,0x5d,0x60,0x63,0x67,0x6a,0x6d,0x70,0x73,0x76,0x79,0x7c
};
volatile uint8_t i = 0;
volatile uint16_t conversion = 0;
volatile uint16_t new_conversion = 0;
volatile uint16_t delay = 160;
volatile uint16_t wave = 4;
//volatile uint16_t mod_scale = 5;
volatile uint16_t wconversion = 0;
volatile uint16_t dconversion = 0;
volatile uint16_t mod_delay_moving = 500;
volatile uint8_t half = 8;

volatile uint16_t pwm = 0;
volatile uint16_t  mod_delay = 500;
uint8_t(*wave_ptr)(uint8_t*);


uint8_t dither(int16_t delay, uint8_t old_val){
	uint8_t next_val = wave_ptr(&i);
	
	while(delay){
		_delay_us(1);
		delay--;
	}
	return next_val;
}



void my_delay(int16_t delay){
	while (delay){
		_delay_us(1);
		delay--;
	}
}

uint8_t sawtooth(uint8_t* step){
	(*step)++;
	return *step;
}

uint8_t reversesaw(uint8_t* step){
	(*step)--;

	return *step;
}

uint8_t triangle(uint8_t* step){
	static int ud = 0;
	if (ud){
		*step -= 2;
		if(!*step){
			ud = 0;
		}
		} else {
		*step += 2;
		if (*step == 254){
			ud = 1;
		}
	}
	return *step;
}

uint8_t square_p(uint8_t* count){
	static int freq = 126;
	(*count)++;
	static uint8_t val = 0x7F;
	if(*count < freq){
		val = 0x7F;
		} else {
		val = 0xFE;
	}
	*count %= 2*freq;

	return val;
}

uint8_t square_n(uint8_t* count){
	static int freq = 126;
	(*count)++;
	static uint8_t val = 0x01;
	if(*count < freq){
		val = 0x01;
		} else {
		val = 0x7F;
	}
	*count %= 2*freq;
	
	return val;
}

uint8_t square_f(uint8_t* count){

	static int freq = 126;
	(*count)++;
	static uint8_t val = 0x01;
	if(*count < freq){
		val = 0x01;
		} else {
		val = 0xFE;
	}
	*count %= 2*freq;

	return val;
}

uint8_t sampleAndHold(uint8_t* count){
	static uint8_t b = 0;
	static uint8_t i = 0;
	if(i++ == 0){
		int a = rand();
		b = 0xFF & a;
	}
	//(*count)++;
	
	return b;
}
uint8_t sinewave_(uint8_t* i){
	uint8_t val = pgm_read_byte(&sinewave[(*i)++]);
	(*i) %= 256;
	return val;
}
///ROUND UP WHEN UP OR ROUND FOWN WHEN DOWN?????? 
ISR(TIM0_COMPA_vect){
	switch (wave) {
		case SINE : {
			wave_ptr = &sinewave_;
			break;
		}
		case SAW : {
			wave_ptr = &sawtooth;
			break;
		}
		
		case RSAW : {
			wave_ptr = &reversesaw;
			break;
		}
		
		case TRI : {
			wave_ptr = &triangle;
			break;
		}
		
		case SQUAREP: {
			wave_ptr = &square_p;
			break;
		}
		
		case SH : {
			wave_ptr = &sampleAndHold;
			break;
		}
		
		case SQUAREN: {
			wave_ptr = &square_n;
			break;
		}
		case SQUAREF: {
			wave_ptr = &square_f;
			break;
		}
	}
	//my_delay(mod_scale); interseting
	OCR0A = dither(delay, OCR0A);
	
}



void enableADC(){
DDRA &= ~(1 << PINA0) | ~(1 << PINA1) | ~(1 << PINA2) | ~(PINA3);
ADMUX = 0;							// select PB3
ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);	 //use 128 division factor
ADCSRA |= (1 << ADEN);								//ADC enable
}
uint16_t map_m(uint16_t input_end, uint16_t input_start, uint16_t output_end, uint16_t output_start,  uint16_t input){
	uint16_t slope = (output_end - output_start) / (input_end - input_start);
	uint16_t output = output_start + slope * (input - input_start);
	return output;
}

int16_t VoltageToDelay (uint16_t volt){
	if(volt < 500){
		return map_m(500, 50, 800, 60, volt);
	} else {
		return map_m(1024, 500, 9200, 700, volt);
	}
	//9000 top value (.3 Hz)  60 - 30 Hz
}

void VoltageToWave (uint16_t volt){
	if (volt < 100){
		wave = SINE;
	} else if (volt < 200){
		wave = SAW;
	} else if (volt < 300){
		wave = RSAW;
	} else if (volt < 400){
		wave = TRI;
	} else if (volt < 700){
		wave = SQUAREP;
	} else if (volt < 800){
		wave = SQUAREN;
	} else if (volt < 900) {
		wave = SQUAREF;
	} else {
		wave = SH;
	}
}

void adc_conversion(uint8_t input){
	//clear lower 3 bits of admux
	ADMUX &= 0xF8;
	ADMUX |= input;
	// start single conversion
	// write '1' to ADSC
	ADCSRA |= (1<<ADSC);
	
	// wait for conversion to complete
	// ADSC becomes '0' again
	// till then, run loop continuously
	while(ADCSRA & (1<<ADSC));
	new_conversion = 0;
	new_conversion = ADCL;
	new_conversion |= ADCH << 8;
	ADCSRA |= (1<<ADSC);
	if (input % 2){
		if ((new_conversion <= (wconversion - 30) || new_conversion >= (wconversion + 30))){
			wconversion = new_conversion;
			VoltageToWave(wconversion);
		}
		} else {
		if ((new_conversion <= (conversion - DEADBAND) || new_conversion >= (conversion + DEADBAND))){
			conversion = new_conversion;
			delay = VoltageToDelay(conversion);
		}
	}
	
}

//CLK FREQUENCY IS 8 MHz (be sure to set the system clock prescale to 0)
int main(void) {
	//Set PORTB1 pin as output
	cli();
	DDRB = (1 << PORTB2);
	DDRA = (1 << PORTA7);
	// initial OCR1A value
	OCR0A=80;
	//Output compare OC1A 8 bit non inverted PWM
	TCCR0A = (1 << COM0A1) | (1 << WGM01) | (1 << WGM00);
	//start timer without prescaler
	
	//enable output compare interrupt for OCR0A
	TIMSK0 = (1 << OCIE0A);
	CLKPR = (1 << CLKPCE); 
	CLKPR = 0;
    TCCR0B= (1 << CS00);
	//ADC
	enableADC();
	//enable global interrups
	sei();
	int a = 0;
	while (1){
		adc_conversion(a);
		a++;
		a %=2;
	}
	
}	
		
	


