/*
 * rotary.c
 *
 * Created: 24.07.2022 16:42:44
 * Author : Clem
 *	
 */ 

#include <avr/io.h>
#include "lcd-routines.h"
#include <util/delay.h>
#include "rotary.h"
#include <stdlib.h>

int main(void)
{		
	char buffer[16];
	
	rotary_init();
	
	lcd_init();

	

    while (1) 
    {	
		lcd_clear();
		lcd_home();
		lcd_string(itoa(status,buffer,2));
		lcd_setcursor(0,2);
		lcd_string(itoa(rotary_getCount(),buffer,10));
		lcd_setcursor(5,2);
		lcd_string(itoa(rotary_getChange(),buffer,10));
		lcd_setcursor(8,2);
		lcd_string(itoa(rotary_getButton(),buffer,10));
		_delay_ms(500);
		
		
    }
}

