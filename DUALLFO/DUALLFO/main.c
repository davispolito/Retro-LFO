/*
 * DualLFO.c
 *
 * Created: 1/9/2019 10:25:38 AM
 * Author : Davis Polito
 */ 
#define CLOCK1_MAX 0xFFFF
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
#define NUMPOTS 4
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
typedef uint8_t(*wave_ptr)(uint8_t*);
uint8_t sinewave_(uint8_t* i);
typedef struct LFOs {
	uint8_t i;
	//uint8_t wave;
	uint16_t delay;
	wave_ptr wave;
} LFO;


static LFO lfo[2] = {
	{0, 100,
		&sinewave_
	},
	{0, 100, 
		&sinewave_
	}
};
volatile uint16_t conversion = 0;
volatile uint16_t new_conversion = 0;
//volatile uint16_t mod_scale = 5;
volatile uint16_t wconversion = 0;
volatile uint16_t dconversion = 0;
volatile uint16_t mod_delay_moving = 500;
volatile uint8_t half = 8;
uint8_t bias = 127;
volatile uint16_t  mod_delay = 500;


uint8_t dither(LFO * lfo, uint8_t old_val){
	uint8_t next_val = lfo->wave(&(lfo->i));
	
	while(lfo->delay){
		_delay_us(1);
		lfo->delay--;
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
ISR(TIM1_COMPA_vect){
	LFO *lfo_curr = &lfo[0];
	OCR0A = lfo_curr->wave(&(lfo_curr->i));
	uint16_t curr_timer = ((TCNT1H << 8) | TCNT1L);
	uint16_t time_diff = CLOCK1_MAX - curr_timer;
	uint16_t new_time = (time_diff < lfo_curr->delay) ? 
						(time_diff - lfo_curr->delay) : (curr_timer + lfo_curr->delay);
	OCR1A = new_time;
}

ISR(TIM1_COMPB_vect){
	LFO *lfo_curr = &lfo[1];
	OCR0B = lfo_curr->wave(&(lfo_curr->i));
	uint16_t curr_timer = ((TCNT1H << 8) | TCNT1L);
	uint16_t time_diff = CLOCK1_MAX - curr_timer;
	uint16_t new_time = (time_diff < lfo_curr->delay) ?
						(time_diff - lfo_curr->delay) : (curr_timer + lfo_curr->delay);
	OCR1B = new_time;
}


uint16_t map_m(uint16_t input_end, uint16_t input_start, uint16_t output_end, uint16_t output_start,  uint16_t input){
	uint16_t slope = (output_end - output_start) / (input_end - input_start);
	uint16_t output = output_start + slope * (input - input_start);
	return output;
}

int16_t VoltageToDelay (uint16_t volt){
	if(volt < 500){
		return map_m(500, 50, 30, 1, volt);
	} else {
		return map_m(1024, 500, 100, 30, volt);
	}
	
	//return 60 + (volt * 4);			//9000 top value (.3 Hz)  60 - 30 Hz
}

void VoltageToWave(uint16_t volt, LFO* lfo){
	
	if (volt < 100){
		lfo->wave = &sinewave_;
	} else if (volt < 200){
		lfo->wave = &sawtooth;
	} else if (volt < 300){
		lfo->wave = &reversesaw;
	} else if (volt < 400){
		lfo->wave = &triangle;
	} else if (volt < 700){
		lfo->wave = &square_p;
	} else if (volt < 800){
		lfo->wave = &square_n;
	} else if (volt < 900) {
		lfo->wave = &square_p;
	} else {
		lfo->wave = &sampleAndHold;
	}
}

void enableADC(){
	DDRA &= ~(1 << PINA0) | ~(1 << PINA1) | ~(1 << PINA2) | ~(PINA3);
	ADMUX = 0;							// select PB3
	ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);	 //use 128 division factor
	ADCSRA |= (1 << ADEN);									//ADC enable
}

void calibrate(){
	cli();
    uint16_t new_cal = 0;
	uint16_t calibration = 0;
	while(!(PINB & (1 << PINB1))){
		ADMUX = (1 << MUX1);
		
		// start single conversion
		// write '1' to ADSC
		ADCSRA |= (1<<ADSC);
	   
		// wait for conversion to complete
		// ADSC becomes '0' again
		// till then, run loop continuously
		while(ADCSRA & (1<<ADSC));
		uint16_t new_cal = 0;
		new_cal = ADCL;
		new_cal |= ADCH << 8;
		if ((new_cal <= (calibration - DEADBAND) || new_cal >= (calibration + DEADBAND))){
			calibration = new_cal;
			
		//	bias = map_m(1024, 100, 138, 122, calibration);
			//delay = VoltageToDelay(conversion);
			if (calibration < 100){
				bias = 120;
				} else if (calibration < 200){
				bias = 121;
				} else if (calibration < 300){
				bias = 122;
				} else if (calibration < 400){
				bias = 123;
				} else if (calibration < 500){
				bias = 124;
				} else if (calibration < 600) {
				bias = 125;
				} else if (calibration < 700) {
				bias = 126;
				} else if (calibration < 800) {
				bias = 127;
				} else if (calibration < 1000) {
				bias = 128;
			}
		}
		
		
		OCR0A = bias;
	
		//OCR0A = 25;
	}
	//EEPROM_WRITE(0, bias);
}
//CLK FREQUENCY IS 8 MHz (be sure to set the system clock prescale to 0)
void adc_conversion(LFO* lfo, uint8_t input){
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
				VoltageToWave(wconversion, &lfo[input/2]);
			}
		} else {
			if ((new_conversion <= (conversion - DEADBAND) || new_conversion >= (conversion + DEADBAND))){
				conversion = new_conversion;
				lfo[input / 2].delay = VoltageToDelay(conversion);
			}
		}
		
}

int main(void) {
	//Set PORTB1 pin as output
	cli();
	DDRB = (1 << PORTB2);
	DDRA = (1 << PORTA7);
	//PORTB = (1 << PINB0) | (1 << PINB1);
	// initial OCR0A value
	OCR0A = 80;
	OCR0B = 80;
	//Output compare OC0A 8 bit non inverted PWM
	TCCR0A = (1 << COM0A1) | (1 << COM0B1)| (1 << WGM01) | (1 << WGM00);
	//start timer without prescaler
	
	//enable output compare interrupt for OCR0A and OCR0B
	TIMSK1 = (1 << OCIE1A) | (1 << OCIE1B);
	
	TCCR1A = 0;
	TCCR1B = 0;
	//clock change
	
	//CLKPR = 0;
    TCCR0B= (1 << CS00);
	TCCR1B = (1 << CS00) | (1 << CS02);
	//CLKPR = (1 << CLKPCE); 
	//ADC
	enableADC();
	//enable global interrupts
// 	OCR1AH = 0;
// 	OCR1AL = 80;
// 	OCR1BH = 0;
//     OCR1BL = 80;
// 	TCNT1L = 0;
// 	TCNT1H = 0;
	OCR1A = 80;
	TCNT1 = 0;
	OCR1B = 60;
	sei();

	//PORTB |= (1 << PB1);
	while (1){
		/*if(!(PINB & (1 << PINB1))){
			calibrate();
		}*/
		for(int i = 0; i < NUMPOTS; i++) {
			adc_conversion(lfo, i);
			int a = TCNT0;
			TCNT0 = a;
		}
	
		//printf("%u", TCNT1);
	}
}






