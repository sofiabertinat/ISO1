/*
 * os_task.h
 *
 *  Created on: Jul 17, 2022
 *      Author: Sofia Bertinat
 */

#ifndef ISO1_INC_OS_TASK_H_
#define ISO1_INC_OS_TASK_H_

#include "../inc/types.h"

#define STACK_SIZE 			256

#define TASK_NAME_SIZE		10

/*  */
typedef enum _taskState
{
	TASK_RUNNING = 0,
	TASK_READY,
	TASK_BLOCKED
} taskState_t;

typedef uint8_t priority_t;

/* */
typedef struct _task
{
	uint32_t 		stack[STACK_SIZE/4];
	uint32_t 		stack_pointer;
	void 			*entry_point;
	taskState_t 	state;
	priority_t		priority;
	uint32_t 		id;
	uint32_t 		ticks_block;
	char 			name[TASK_NAME_SIZE];
} task_t;

/* */
bool os_task_init(task_t * pTask, void * entryPoint, const char * const pcName, void * const pvParameters, priority_t priority, uint32_t id);

/* */
void os_task_create(task_t * pTask, void * entryPoint, const char * const pcName, void * const pvParameters, priority_t priority);

#endif /* ISO1_INC_OS_TASK_H_ */
