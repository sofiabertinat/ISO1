/*
 * os_core.c
 *
 *  Created on: Jul 17, 2022
 *      Author: Sofia Bertinat
 */

#include "../inc/os_core.h"
#include "../inc/os_hook.h"

#define MAX_TASK_LIST		8


typedef struct _osControl
{
	osState_t 	sys_state;
	osState_t 	sys_previous_state;
	task_t 		*current_task;
	task_t 		*next_task;
	uint16_t 	error;
	task_t *    list[MAX_TASK_LIST];
	int 		cant;
	uint32_t 	os_tick_count;
	uint16_t 	os_critical_count;
} osControl_t;

osControl_t control_OS;

task_t TaskIdle;

/* */
static void os_set_pendSV(void)
{
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
static bool os_scheduler(void)
{
	uint32_t i_task;
	task_t *aux_p_task;
	int i, j;
	uint8_t  aux_p = 0;
	uint32_t current_ticks;

	if (control_OS.sys_state == OS_STARTING)
	{
		i_task = 1;
		/* Set current task as first configure task */
		control_OS.current_task = control_OS.list[i_task-1];
	}
	else
	{
		/*The scheduler can be called from an exception given by Systick in Handler mode,
		 * or by the function os_cpu_yield in Thread mode */
		if (control_OS.sys_state == OS_SCHEDULING)
		{
			return RETURN_FAIL;
		}
		else
		{
			os_set_state(OS_SCHEDULING);
		}

		aux_p = 255;

		if(control_OS.current_task->id < control_OS.cant)
			j = control_OS.current_task->id;
		else
			j = 0;

		for(i=0;i<control_OS.cant;i++)
		{
			if(control_OS.list[j]->state == TASK_BLOCKED)
			{
				if(control_OS.list[j]->ticks_block != 0)
				{
					current_ticks = os_get_tick_count();
					if(current_ticks >= control_OS.list[j]->ticks_block)
					{
						control_OS.list[j]->state = TASK_READY;
						control_OS.list[j]->ticks_block = 0;
					}
				}
			}

			if(control_OS.list[j]->state == TASK_READY)
			{
				if(aux_p > control_OS.list[j]->priority)
				{
					aux_p = control_OS.list[j]->priority;
					i_task = j;
				}
			}

			j++;
			if(j >= control_OS.cant)
				j = 0;

		}

		if(aux_p == 255)
		{
			if(control_OS.current_task->state == TASK_RUNNING)
				control_OS.next_task = control_OS.current_task;
			else
				control_OS.next_task = &TaskIdle;
		}
		else
		{
			control_OS.next_task = control_OS.list[i_task];
		}

		if(control_OS.current_task->state == TASK_BLOCKED)
		{
			os_set_state(OS_RUN);
			os_set_pendSV();
		}

		os_set_state(OS_RUN);
	}

	return RETURN_OK;
}

/* SysTick: Exception generated from system timer. Used as a time base in OS.*/
void SysTick_Handler(void)
{
	bool rc;

	control_OS.os_tick_count++;

	rc = os_scheduler();

	if(rc == RETURN_OK)
		os_set_pendSV();

}

/* */
void __attribute__((weak)) os_idle_task(void)  {
	while(1)  {
		__WFI();
	}
}

/**/
void os_init_control_struct(void)
{
	int i;

	control_OS.sys_previous_state = OS_STARTING;
	control_OS.sys_state = OS_STARTING;
	control_OS.current_task = NULL;
	control_OS.next_task = NULL;
	control_OS.error = 0;
	control_OS.cant = 0;

	for (i = control_OS.cant; i < MAX_TASK_LIST; i++)
	{
		control_OS.list[i] = NULL;
	}
}


/* */
void os_init(void)
{
	/*
	 * Todas las interrupciones tienen prioridad 0 (la maxima) al iniciar la ejecucion. Para que
	 * no se de la condicion de fault mencionada en la teoria, debemos bajar su prioridad en el
	 * NVIC (Nested Vector Interrupt Controller ). La cuenta matematica que se observa da la probabilidad mas baja posible.
	 */
	NVIC_SetPriority(PendSV_IRQn, (1 << __NVIC_PRIO_BITS)-1);


	/* Init Idle Task, lower priority*/
	os_task_init(&TaskIdle, os_idle_task, (const char *)"IdleTask", NULL, 255, 255);
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
		os_set_state(OS_RUN);

		/* Return next line to run*/
		sp_next = control_OS.current_task->stack_pointer;
	}
	else
	{
		if(control_OS.next_task != control_OS.current_task)
		{
			/* Change current task state to READY if it is not BLOCKED */
			if(control_OS.current_task->state == TASK_RUNNING)
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
		else
		{
			/* Save current MSP in task struct, before update of control struct */
			control_OS.current_task->stack_pointer = sp_current;
			sp_next = control_OS.current_task->stack_pointer;
		}
	}
	/* Return to PendSv handler*/
	return sp_next;
}

/* */
void os_set_error(void *caller, uint16_t error)  {
	control_OS.error = error;
	errorHook(caller, error) ;
}

/* */
uint16_t os_get_error(void)  {
	return control_OS.error;
}

/**/
void os_enter_critial(void)
{
	__disable_irq();
	control_OS.os_critical_count++;
}

/**/
void os_exit_critical(void)
{
	if(control_OS.os_critical_count > 0)
		control_OS.os_critical_count--;
	if(control_OS.os_critical_count == 0 )
	{
		__enable_irq();
	}
}

/**/
uint32_t os_get_tick_count( void )
{
	uint32_t aux_ticks;

	/* Critical section required if running on a 16 bit processor. */
	os_enter_critial();
	{
		aux_ticks = control_OS.os_tick_count;
	}
	os_exit_critical();

	return aux_ticks;
}

/**/
void os_block_current_task(void)
{
	control_OS.current_task->state = TASK_BLOCKED;
	os_exit_critical();
	os_cpu_yield();
}

/**/
void os_unblock_task(task_t* task)
{
	task->state = TASK_READY;
}

/**/
task_t* os_get_current_task(void)
{
	return control_OS.current_task;
}

/**/
void os_set_current_task_ticks( uint32_t ticks_block)
{
	control_OS.current_task->ticks_block = ticks_block;
}

/**/
void os_cpu_yield(void)
{
	os_scheduler();
}

/* */
void os_task_create(task_t * pTask, void * entryPoint, const char * const pcName, void * const pvParameters, uint8_t priority)
{
	bool taskCreated = 0;
	u_int32_t id;

	if(control_OS.cant < MAX_TASK_LIST)
	{
		control_OS.cant = control_OS.cant +1;

		taskCreated = os_task_init(pTask, entryPoint, pcName, pvParameters, priority, control_OS.cant);

		if(taskCreated)
		{
			control_OS.list[control_OS.cant-1] = pTask;
		}
	}
	else
	{
		os_set_error(NULL, MAX_TASK_ERROR);
	}
}

/**/
void os_task_remove(task_t * pTask)
{
	int i, j;
	for(i=0;i<control_OS.cant;i++)
	{
		if(control_OS.list[i] == pTask)
		{
			if(i == (control_OS.cant -1 ))
			{
				control_OS.list[i] = NULL;
			}
			else
			{
				for(j=i;j<(control_OS.cant-1);j++)
					control_OS.list[j] = control_OS.list[j+1];
				control_OS.list[j] = NULL;
			}

			control_OS.cant = control_OS.cant -1;
			break;
		}
	}
	os_cpu_yield();

}

void os_set_state(osState_t state)
{
	control_OS.sys_previous_state = control_OS.sys_state;
	control_OS.sys_state = state;
}

void os_set_previous_state(void)
{
	osState_t state;
	state = control_OS.sys_state;
	control_OS.sys_state = control_OS.sys_previous_state;
	control_OS.sys_previous_state =  state;
}


