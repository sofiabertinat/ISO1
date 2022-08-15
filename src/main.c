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
#include "../inc/os_irq.h"


task_t TaskWrite, TaskDisplay, TaskLedOn, TaskLedOff, TaskBotton;

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

	/* Init led*/
	gpioWrite(LED1,FALSE);
	gpioWrite(LED2,FALSE);

	/**/
	os_my_IRQ_Init();
}

/* */
int main( void )
{
	sys_init();

	init_queue();
	init_sems();

	os_init_control_struct();

	os_task_create(&TaskWrite, write_task, (const char *)"Tarea00001", NULL, 2);
	os_task_create(&TaskDisplay, display_task, (const char *)"Tarea00002", NULL, 3);
	os_task_create(&TaskLedOn, led_on_task, (const char *)"Tarea00003", NULL, 4);
	os_task_create(&TaskLedOff, led_off_task, (const char *)"Tarea00004", NULL, 4);
	os_task_create(&TaskBotton, botton_task, (const char *)"Tarea00005", NULL, 1);

	os_isr_register(PIN_INT0_IRQn, button_fall_IRQHandler, 0);
	os_isr_register(PIN_INT1_IRQn, button_rise_IRQHandler, 1);
	os_isr_register(PIN_INT2_IRQn, button_fall_IRQHandler, 2);
	os_isr_register(PIN_INT3_IRQn, button_rise_IRQHandler, 3);
	os_isr_register(PIN_INT4_IRQn, button_fall_IRQHandler, 4);
	os_isr_register(PIN_INT5_IRQn, button_rise_IRQHandler, 5);
	os_isr_register(PIN_INT6_IRQn, button_fall_IRQHandler, 6);
	os_isr_register(PIN_INT7_IRQn, button_rise_IRQHandler, 7);
	os_init();

	while (1)
	{

	}
}


