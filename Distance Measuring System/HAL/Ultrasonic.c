/*
 * Ultrasonic.c
 *
 *  Created on: Oct 13, 2021
 *      Author: Ali Hany
 */
#include "Ultrasonic.h"
#include "icu.h"
#include "common_macros.h"
#include "gpio.h"
#include <util/delay.h>
#include <avr/io.h>

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

uint8 g_edgeCount = 0;
uint16 g_timeHigh = 0;
uint32 distance = 0;
uint32 timer = 0;


Icu_ConfigType ICU_Config = {F_CPU_8,RISING};

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void Ultrasonic_init(void)
{
	Icu_init(&ICU_Config);
	Icu_setCallBack(Ultrasonic_edgeProcessing);
	GPIO_setupPinDirection(PORTB_ID,PIN5_ID,PIN_OUTPUT);
	GPIO_writePin(PORTB_ID,PIN5_ID,LOGIC_LOW);
}

void Ultrasonic_Trigger(void)
{
	GPIO_writePin(PORTB_ID,PIN5_ID,LOGIC_HIGH);
	_delay_us(10);
	GPIO_writePin(PORTB_ID,PIN5_ID,LOGIC_LOW);
}

void Ultrasonic_edgeProcessing(void)
{
	g_edgeCount++;
	if(g_edgeCount == 1)
	{
		/*
		 * Clear the timer counter register to start measurements from the
		 * first detected rising edge
		 */
		Icu_clearTimerValue();
		/* Detect falling edge */
		Icu_setEdgeDetectionType(FALLING);
	}
	else if(g_edgeCount == 2)
	{
		/* Store the High time value */
		g_timeHigh = Icu_getInputCaptureValue();
		/* Detect rising edge */
		Icu_setEdgeDetectionType(RISING);
	}

}
uint16 Ultrasonic_readDistance(void)
{
	Ultrasonic_Trigger();
	if(g_edgeCount == 2)
	{
		distance = (float)(g_timeHigh / 58.8);
		if (distance >= 2 && distance < 60)
		{
			distance = distance + 1;
		}
		else if (distance >= 60 && distance < 133)
		{
			distance = distance + 2;
		}
		else if (distance >= 133 && distance < 207)
		{
			distance = distance + 3;
		}
		else if (distance >= 207 && distance < 280)
		{
			distance = distance + 4;
		}
		else if (distance >= 280 && distance < 353)
		{
			distance = distance + 5;
		}
		else if (distance >= 353 && distance <= 400)
		{
			distance = distance + 6;
		}

		g_edgeCount = 0;
	}


	return distance;
}
