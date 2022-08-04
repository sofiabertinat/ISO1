/*
 * os_task.c
 *
 *  Created on: Jul 17, 2022
 *      Author: Sofia Bertinat
 */

#include "../inc/os_task.h"
#include "../inc/os_hook.h"

#define STACK_FRAME_SIZE			8
#define FULL_STACKING_SIZE 			17	/* 16 core registers + LR previous value*/

#define INIT_XPSR 	1 << 24				/* xPSR.T = 1 ; bit[0] of PC allways load in xPSR[24] (Thumb bit)*/
#define EXEC_RETURN	0xFFFFFFF9			/* retornar a modo thread con MSP, FPU no utilizada */

/* */
bool os_task_init(task_t * pTask, void * entryPoint, const char * const pcName, void * const pvParameters, priority_t priority, uint32_t id)
{
	uint8_t i;
	task_t *pTask_aux = NULL;

	pTask_aux = pTask;

	/* Thumb bit*/
	pTask_aux->stack[STACK_SIZE/4 - XPSR] = INIT_XPSR;
	/* save task init address*/
	pTask_aux->stack[STACK_SIZE/4 - PC_REG] = (uint32_t)entryPoint;
	/* Task return*/
	pTask_aux->stack[STACK_SIZE/4 - LR] = (uint32_t)returnHook;

	/*
	 * El valor previo de LR (que es EXEC_RETURN en este caso) es necesario dado que
	 * en esta implementacion, se llama a una funcion desde dentro del handler de PendSV
	 * con lo que el valor de LR se modifica por la direccion de retorno para cuando
	 * se termina de ejecutar getContextoSiguiente
	 */
	pTask_aux->stack[STACK_SIZE/4 - LR_PREV_VALUE] = EXEC_RETURN;

	pTask_aux->stack_pointer = (uint32_t) (pTask_aux->stack + STACK_SIZE/4 - FULL_STACKING_SIZE);

	pTask_aux->entry_point = entryPoint;

	pTask_aux->state = TASK_READY;

	pTask_aux->id = id;

	pTask_aux->priority = priority;

	pTask_aux->ticks_block = 0;

	for(i=0;i<TASK_NAME_SIZE;i++)
		pTask_aux->name[i] = pcName[i];

	pTask = pTask_aux;

	return RETURN_OK;
}
