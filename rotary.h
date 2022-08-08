/*
 * rotary.h
 *
 * Created: 07.08.22
 *  Author: Clemens Hauser
 *	Simple timer interrupt based algorithm to read out a gray code based rotary encoder. See readme!
 */ 


#ifndef ROTARY_H_
#define ROTARY_H_

#define SIGNAL_DDR			DDRD
#define	SIGNAL_PIN			PIND 
#define SIGNAL_A			PD7
#define SIGNAL_B			PD6
#define SW					PD4

#define F_CPU		8000000

#define TIMER_CONTROL	0x0B		//Prescaler 64, clear timer on compare match active
#define TIMER_COMPARE	124

volatile char status;
volatile short cnt;
volatile char button;

void rotary_init();

#endif /* ROTARY_H_ */