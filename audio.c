#include <stdint.h>
#include <stdio.h>

#include "juno.h"
#include "wave.h"
#include "audio.h"

#if defined(_AVR_)
static volatile uint8_t queue[256];
static volatile uint8_t head, tail;
// samples are pushed to the tail and pulled from the head

ISR(TIMER1_COMPA_vect)
{
	if (head != tail)
		OCR2A = queue[head++];
	else
		OCR2A = 128;
}

// TODO
#define SLEEP() do;while(0)

void audio_play_sample(mono8 s)
{
	uint8_t t = tail + 1;
	while (t == head) {
		SLEEP();
	}
	queue[t] = s + 128;
	tail = t;
}

void audio_init(void)
{
	// PD6 is output
	DDRD |= _BV(PD6);
	// 50% duty cycle
	OCR0A = 128;
	// non-inverting mode
	TCCR0A |= _BV(COM0A1);
	// fast PWM mode
	TCCR0A |= _BV(WGM01) | _BV(WGM00);
	// enable PWM with no prescaler
	// 16MHz clock => 62.5kHz PWM
	TCCR0B |= _BV(CS00);





	// set up PWM

	// PWM uses PB3
	DDRB |= _BV(PB3);

	// setup PWM
	TCCR2A |= _BV(WGM21) | _BV(WGM20);
	//TCCR2B &= ~_BV(WGM22);

	// non-inverting
	TCCR2A |= _BV(COM2A1) | _BV(COM2B1);
	// no prescaler
	TCCR2B |= _BV(CS20);



	// setup Timer1
	OCR1A = 50 - 1; //65535U;
	OCR1B = 50 - 1; //65535U;
	ICR1 = 1;

	TCCR1A = 0;
	TCCR1B |= _BV(WGM12);

	// /1024 prescaler
	//TCCR1B |= _BV(CS12) | _BV(CS10);
	// /256 prescaler
	//TCCR1B |= _BV(CS12);
	// /64 prescaler
	//TCCR1B |= _BV(CS11) | _BV(CS10);
	// /8 prescaler
	//TCCR1B |= _BV(CS11);
	// no prescaler
	TCCR1B |= _BV(CS10);

	OCR1A = ((double)F_CPU / SAMPLE_RATE + 0.5) - 1;
	// enable timer compare interrupt
	TIMSK1 = _BV(OCIE1A);

}
#else
static FILE *audiofile;

void audio_play_sample(mono8 s)
{
#if BIG_TARGET
	putc(s + 128, audiofile);
#else
	// TODO collect 4 mono samples into a single byte per the Punix audio format
#endif
}

void audio_init(void)
{
#if BIG_TARGET
	audiofile = stdout;
#else
	audiofile = fopen("/dev/audio", "wb");
#endif
}
#endif
