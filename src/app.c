/*
 * app.c
 *
 *  Created on: Jul 17, 2022
 *      Author: Sofia Bertinat
 */

#include "../inc/app.h"
#include "sapi.h"

/* */
void led_task(void)
{
	bool stateLed = FALSE;

	while (1) {
		if(stateLed == FALSE)
		{
			gpioWrite( LED2, ON );
			stateLed = TRUE;
		}
		else
		{
			gpioWrite( LED2, OFF );
			stateLed = FALSE;
		}

	}
}

/* */
void cont_task(void)
{
	uint32_t cont_1 = 0;
	uint32_t cont_2 = 0;

	while (1) {
		cont_1++;
		cont_2++;
	}
}
