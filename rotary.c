/*
 * rotary.c
 *
 * Created: 07.08.22
 *  Author: Clemens Hauser
 * Simple timer interrupt based algorithm to read out a gray code based rotary encoder. See readme!
 */ 

#include "rotary.h"




void rotary_init()
{	
	status = 0;
	cnt = 0;
	
	button_status = 0;
	button_flag = false;
	button_switch = false;
		
	TCCR1A =	(1<<COM1A0);
	TCCR1B =	TIMER_CONTROL;			//F_TIMER = 125 kHz -> for F_INT = 1 kHz count to 125-1 (interrupt fired 1 cycle later)
	OCR1A  =	TIMER_COMPARE;
	
	TIMSK |= (1<<OCIE1A);										//Enable Output compare match interrupt enable (@TIMER_COMPARE)
	
	
	SIGNAL_DDR &= (0<<SIGNAL_A) & (0<<SIGNAL_B) & (0<<SW);		//Set all as input
	SIGNAL_PORT |= (1<<SW);										//Enable internal pullup for SW-Button
	
	status |=	(SIGNAL_PIN & (1<<SIGNAL_A))>>(SIGNAL_A-1) ;		//write new status
	status |=	(SIGNAL_PIN & (1<<SIGNAL_B))>>(SIGNAL_B) ;		//write new status
	
	button_status |=	(SIGNAL_PIN & (1<<SW))>>SW;						//Push button status
	
	sei();														//start global interrupts
	
}


int8_t rotary_getCount()
{
	return cnt;
}



int8_t rotary_getChange()
{
	static int8_t last = 0;
	int8_t temp = cnt - last;
	
	last = cnt;
	return temp;	
}

uint8_t rotary_getButton()
{
	return button_status;
}

bool rotary_getSwitch()
{
	return button_switch;
}

ISR (TIMER1_COMPA_vect)											//executing every 1ms
{	
	//**** ROTARY EVALUATION ****
	status = status << 2;										//shift to make place for new status
	status |=  (SIGNAL_PIN & (1<<SIGNAL_A))>>(SIGNAL_A-1) ;		//write new status
	status |=  (SIGNAL_PIN & (1<<SIGNAL_B))>>(SIGNAL_B) ;		//write new status

	if((status & 0x0F) == 0xB || (status & 0x0F) == 0x4)
			cnt ++;	
				
	if((status & 0x0F) == 0x7 || (status & 0x0F) == 0x8)
			cnt --;  
			
	//**** BUTTON EVALUATION ****
	
	button_status = button_status << 1;
	button_status |=	(SIGNAL_PIN & (1<<SW))>>SW;					//write new push button status
	
	if(!(button_status&0x1)  &&  ((button_status&0x2)>>1))			//falling edge of button
	{
		button_flag = true;											//button was pushed
	}
	
	if(button_flag && (button_status&0x1))							// button released and pushed before
	{	
		// ****switch toggled****
		button_flag = false;										// reset flag
		button_switch = !button_switch;								// toggle switch			
	}
	
}