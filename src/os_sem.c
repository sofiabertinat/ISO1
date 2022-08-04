/*
 * os_sem.c
 *
 *  Created on: Aug 1, 2022
 *      Author: Sofia Bertinat
 */

#include "../inc/os_sem.h"
#include "../inc/os_core.h"

/* */
void os_sem_init(sem_t* sem)
{
	sem->taked = true;
	sem->task = NULL;
}

/* */
void os_sem_take(sem_t* sem)
{
	if(sem->taked)
	{
		sem->task = os_get_current_task();
		os_block_current_task();
	}
	else
	{
		sem->taked = true;
	}
}

/* */
void os_sem_give(sem_t* sem)
{
	if(sem->taked)
	{
		sem->taked = false;
		if(sem->task != NULL)
		{
			sem->task->state = TASK_READY;
		}
	}
}

