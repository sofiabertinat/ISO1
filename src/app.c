/*
 * app.c
 *
 *  Created on: Jul 17, 2022
 *      Author: Sofia Bertinat
 */

#include "../inc/app.h"
#include "../inc/os_api.h"
#include "sapi.h"

#include "../inc/os_queue.h"

queue_t* displayQueue;

void init_queue(void)
{
	displayQueue = os_queue_create();
}

/* */
void led_task(void )
{
	uint8_t cont = 0;
	uint8_t * item;

	while (1) {
		gpioWrite(LED2,TRUE);
		if(cont == 255)
			cont = 0;
		else
			cont++;
		item = (uint8_t*) malloc(sizeof(uint8_t));
		*item = cont;
		os_queue_write( displayQueue, item );
		os_delay(60000);

		gpioWrite(LED2,FALSE);
		os_delay(60000);
	}
}

/* */
void display_task(void )
{
	uint8_t* item;

	while (1) {
		item = (uint8_t*) os_queue_read( displayQueue);
		uartWriteByte(UART_USB, *item);
		free(item);
	}
}
/* */
void test_hook_rtn_task(void )
{
	gpioWrite(LED1,TRUE);
}
