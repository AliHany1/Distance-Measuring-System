/*********************************************
 *  File Name: Mini_project_4.c             *
 *                                          *
 *  Created on: Oct 13, 2021                 *
 *  Author: Ali Hany                        *
 *********************************************/
#include <avr/io.h>
#include "lcd.h"
#include "icu.h"
#include "Ultrasonic.h"

uint16 dist = 0;

int main (void)
{
	SREG |= (1<<7);

	LCD_init();
	Ultrasonic_init();
	LCD_displayStringRowColumn(0, 0,"Distance =    cm");
	while (1)
	{
		LCD_moveCursor(0,11);
		dist = Ultrasonic_readDistance();
		if(dist >= 100)
			{
				LCD_intgerToString(dist);
			}
			else
			{
				LCD_intgerToString(dist);
				LCD_displayCharacter(' ');
			}
	}
}
