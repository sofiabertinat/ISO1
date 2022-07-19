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

//task_t * pTaskCont1 = NULL;
//task_t * pTaskCont2 = NULL;
//task_t * pTaskLed = NULL;

task_t TaskCont1, TaskCont2, TaskLed;

/* */
static void sys_init(void)
{
	/* Board setting, already implemented*/
	Board_Init();
	/* Update system clock date, already implemented*/
	SystemCoreClockUpdate();
	/* systick 1ms, already implemented */
	SysTick_Config(SystemCoreClock / MS_IN_S);
}

/* */
int main( void )
{
	sys_init();

	os_task_create(&TaskCont1, cont_task, (const char *)"Tarea1", NULL, 1);
	os_task_create(&TaskCont2, cont_task, (const char *)"Tarea2", NULL, 1);
	os_task_create(&TaskLed, led_task, (const char *)"Tarea3", NULL, 1);

	os_init();

	while (1)
	{

	}
}


