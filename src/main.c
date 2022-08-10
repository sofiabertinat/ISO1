/*
 * main.c
 *
 *  Created on: Jul 17, 2022
 *      Author: Sofia Bertinat
 */

#include "../inc/types.h"
#include "../inc/os_core.h"
#include "../inc/os_task.h"
#include "../inc/app.h"
#include "sapi.h"

task_t TaskWrite, TaskDisplay, TaskLedOn, TaskLedOff;

/* */
static void sys_init(void)
{
	/* Board setting, already implemented*/
	Board_Init();
	/* Update system clock date, already implemented*/
	SystemCoreClockUpdate();
	/* systick 1ms, already implemented */
	SysTick_Config(SystemCoreClock / MS_IN_S);

	/* Init UART_USB a 115200 baudios */
	uartConfig( UART_USB, 115200 );

	gpioWrite(LED1,FALSE);
	gpioWrite(LED2,FALSE);
}

/* */
int main( void )
{
	sys_init();

	init_queue();
	init_sem_led();

	os_task_create(&TaskWrite, write_task, (const char *)"Tarea00001", NULL, 1);
	os_task_create(&TaskDisplay, display_task, (const char *)"Tarea00002", NULL, 2);
	os_task_create(&TaskLedOn, led_on_task, (const char *)"Tarea00001", NULL, 3);
	os_task_create(&TaskLedOff, led_off_task, (const char *)"Tarea00002", NULL, 3);

	os_init();

	while (1)
	{

	}
}


