/*
 * os_core.c
 *
 *  Created on: Jul 17, 2022
 *      Author: Sofia Bertinat
 */

#include "../inc/os_core.h"
#include "../inc/os_task.h"
#include "../inc/table.h"
#include "chip_lpc43xx.h"


typedef enum _osState
{
	OS_STARTING = 0,
	OS_THREAD,
	OS_HANDLER,
} osState_t;

typedef struct _osControl
{
	osState_t 	sys_state;
	task_t 		*current_task;
	task_t 		*next_task;
	int8_t 	    error;
} osControl_t;

static osControl_t control_OS;

static table_t * task_table = NULL;

/* */
void os_task_create(task_t * pTask, void * entryPoint, const char * const pcName, void * const pvParameters, uint8_t priority)
{
	bool taskCreated = 0;
	u_int32_t id;

	if(task_table == NULL)
		id = 0;
	else
		id = table_max_item_number( task_table);

	taskCreated = os_task_init(pTask, entryPoint, pcName, pvParameters, priority, id);

	if(taskCreated)
	{
		if(task_table == NULL)
			table_init(task_table, pTask, sizeof(task_t *));
		else
			table_add_item( task_table, pTask);
	}
}


/* */
static void os_scheduler(void)
{
	uint32_t i_task;
	task_t *aux_p_task;

	if (control_OS.sys_state == OS_STARTING)
	{
		i_task = 1;
	}
	else
	{
		i_task = control_OS.current_task->id +1;
	}

	if(table_get_item( task_table, i_task, aux_p_task) == RETURN_FAIL)
	{
		table_get_item( task_table, 1, aux_p_task);
	}
	/* Set current task as first configure task */
	control_OS.next_task = aux_p_task;
}

/* SysTick: Exception generated from system timer. Used as a time base in OS.*/
static void SysTick_Handler(void)
{
	os_scheduler();

	/* Excepcion PendSV is launched by setting the corresponding pending exception bit in the ICSR registry.  */
	SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;

	/**
	 * Instruction Synchronization Barrier; flushes the pipeline and ensures that
	 * all previous instructions are completed before executing new instructions
	 */
	__ISB();

	/**
	 * Data Synchronization Barrier; ensures that all memory accesses are
	 * completed before next instruction is executed
	 */
	__DSB();

}

/* */
void os_init(void)
{
	/* Init control struct*/
	control_OS.sys_state = OS_STARTING;
	control_OS.current_task = NULL;
	control_OS.next_task = NULL;
	control_OS.error = 0;

	/*
	 * Todas las interrupciones tienen prioridad 0 (la maxima) al iniciar la ejecucion. Para que
	 * no se de la condicion de fault mencionada en la teoria, debemos bajar su prioridad en el
	 * NVIC (Nested Vector Interrupt Controller ). La cuenta matematica que se observa da la probabilidad mas baja posible.
	 */
	NVIC_SetPriority(PendSV_IRQn, (1 << __NVIC_PRIO_BITS)-1);
}

/* */
uint32_t getcontextSwitch(uint32_t sp_current)
{
	uint32_t sp_next;

	if (control_OS.sys_state == OS_STARTING)
	{
		/* Change task state to RUNNING */
		control_OS.current_task->state = TASK_RUNNING;
		/* Change system state to RUN*/
		control_OS.sys_state = OS_THREAD;

		/* Return next line to run*/
		sp_next = control_OS.current_task->stack_pointer;
	}
	else
	{
		/* Change current task state to READY */
		control_OS.current_task->state = TASK_READY;
		/* Save current MSP in task struct, before update of control struct */
		control_OS.current_task->stack_pointer = sp_current;

		/* Update current task in control struct*/
		control_OS.current_task = control_OS.next_task;
		/* Change task state to RUNNING */
		control_OS.current_task->state = TASK_RUNNING;

		/* Return next line to run*/
		sp_next = control_OS.next_task->stack_pointer;
	}
	/* Return to PendSv handler*/
	return sp_next;
}
