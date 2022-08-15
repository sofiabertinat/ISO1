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
#include "../inc/os_sem.h"

queue_t* displayQueue;
queue_t* controlTecQueue;
sem_t semLed;
sem_t semWrite;

/**/
void init_queue(void)
{
	displayQueue = os_queue_create();
	controlTecQueue = os_queue_create();
}

/**/
void init_sems(void)
{
	os_sem_init(&semLed);
	os_sem_init(&semWrite);
}

/* */
void write_task(void )
{
	uint8_t cont = 0;
	uint8_t * item;

	while (1)
	{
		gpioWrite(LED2,TRUE);
		if(cont == 200)
			cont = 0;
		else
			cont++;
		item = (uint8_t*) malloc(sizeof(uint8_t));
		*item = cont;
		os_queue_write( displayQueue, item, FALSE);
		os_delay(20000);

		gpioWrite(LED2,FALSE);
		os_delay(20000);
	}
}

/* */
void display_task(void )
{
	uint8_t* item;

	while (1)
	{
		item = (uint8_t*) os_queue_read( displayQueue);
		uartWriteByte(UART_USB, *item);
		free(item);
	}
}

/* */
void led_on_task(void)
{
	while (1)
	{
		os_sem_take(&semLed);
		gpioWrite(LED1,TRUE);
	}
}

/* */
void led_off_task(void)
{
	while (1)
	{
		gpioWrite(LED1,FALSE);
		os_delay(1000);
		os_sem_give(&semLed);
		os_delay(1000);
	}
}

void botton_task( void )
{
	static buttons_SM_t Tecla_SM[CANT_BUTTONS];

	button_control_t* control;

	int8_t i;

	uint8_t* item;

	for (i = 0;i<CANT_BUTTONS;i++)
	{
		Tecla_SM[i].button = i;
	}

	while(1)
	{
		control = (button_control_t*) os_queue_read( controlTecQueue);

		switch (Tecla_SM[control->Tecla].state)
		{
				case UP:
					if(control->Flanco == FALLING)
					{
						Tecla_SM[control->Tecla].state = DOWN;
					}
					free(control);
					break;

				case DOWN:
					if(control->Flanco == RISING)
					{
						Tecla_SM[control->Tecla].state = UP;

						item = (uint8_t*) malloc(sizeof(uint8_t));
						*item = control->Tecla + 201;
						os_queue_write( displayQueue, item, TRUE);
					}
					free(control);
					break;

				default:
					Tecla_SM[control->Tecla].state = UP;
					free(control);
					break;
		 }
	}
}

/**/
buttons_t get_button(uint8_t n_irq)
{
	if( n_irq == 0 ||  n_irq == 1 )
		return Tecla1;
	else if( n_irq == 2 ||  n_irq == 3 )
		return Tecla2;
	else if( n_irq == 4 ||  n_irq == 5 )
		return Tecla3;
	else
		return Tecla4;
}

/**/
bool button_rise_IRQHandler(void* param)
{
	uint8_t  n_irq;
	button_control_t* item;

	n_irq = *((uint8_t *) param);

	if (Chip_PININT_GetRiseStates(LPC_GPIO_PIN_INT) & PININTCH(n_irq))
	{
		item = (button_control_t*) malloc(sizeof(button_control_t));
		item->Flanco = RISING;
		item->Tecla = get_button(n_irq);

		os_queue_write( controlTecQueue, item, TRUE );

		Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(n_irq));

		return TRUE;
	}
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(n_irq));
	return FALSE;
}

/**/
bool button_fall_IRQHandler(void* param)
{
	uint8_t n_irq;
	button_control_t* item;

	n_irq = *((uint8_t*) param);

	if (Chip_PININT_GetFallStates(LPC_GPIO_PIN_INT) & PININTCH(n_irq))
	{

		item = (button_control_t*) malloc(sizeof(button_control_t));
		item->Flanco = FALLING;
		item->Tecla = get_button(n_irq);

		os_queue_write( controlTecQueue, item, TRUE );

		Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(n_irq));

		return TRUE;
	}
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(n_irq));
	return FALSE;
}



