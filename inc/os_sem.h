/*
 * os_sem.h
 *
 *  Created on: Aug 1, 2022
 *      Author: Sofia Bertinat
 */

#ifndef ISO1_INC_OS_SEM_H_
#define ISO1_INC_OS_SEM_H_

#include "../inc/types.h"
#include "../inc/os_task.h"

typedef struct _sem
{
	task_t* 	task;
	bool 	    taked;
} sem_t;

/* */
void os_sem_init(sem_t* sem);

/* */
void os_sem_take(sem_t* sem);

/* */
void os_sem_give(sem_t* sem);


#endif /* ISO1_INC_OS_SEM_H_ */
