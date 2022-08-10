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
	sem->taked = TRUE;
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
		sem->taked = TRUE;
	}
}

/* */
void os_sem_give(sem_t* sem)
{
	if(sem->taked)
	{
		sem->taked = FALSE;
		if(sem->task != NULL)
		{
			os_unblock_task(sem->task);
		}
	}
}

