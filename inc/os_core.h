/*
 * os_core.h
 *
 *  Created on: Jul 17, 2022
 *      Author: Sofia Bertinat
 */

#ifndef ISO1_INC_OS_CORE_H_
#define ISO1_INC_OS_CORE_H_

#include "../inc/types.h"
#include "../inc/os_task.h"

typedef enum _osState
{
	OS_STARTING = 0,
	OS_SCHEDULING,
	OS_RUN,
	OS_IRQ,
} osState_t;

/**/
void os_init_control_struct(void);

/* */
void os_init(void);

/* */
uint16_t os_get_error(void);

/* */
void os_set_error(void *caller, uint16_t error);

/**/
void os_enter_critial(void);

/**/
void os_exit_critical(void);

/**/
uint32_t os_get_tick_count( void );

/**/
void os_block_current_task(void);

/**/
void os_unblock_task(task_t* task);

/**/
task_t* os_get_current_task(void);

/**/
void os_set_current_task_ticks( uint32_t ticks_block);

/**/
void os_cpu_yield(void);

/**/
void os_task_remove(task_t * pTask);

/**/
void os_set_state(osState_t state);

/**/
void os_set_previous_state(void);

#endif /* ISO1_INC_OS_CORE_H_ */
