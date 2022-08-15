/*
 * hook.c
 *
 *  Created on: Jul 30, 2022
 *      Author: Sofia Bertinat
 */

#include "../inc/os_hook.h"
#include "sapi.h"
#include "../inc/os_core.h"

/* Caution, function executed within interrupt context */
void __attribute__((weak)) tickHook(void)
{
	__asm( "nop" );
}

/* */
void __attribute__((weak)) errorHook(void *caller, uint16_t error)
{
	char maxTaskErr[] = "No more task allowed";
	char memoryErr[] = "Memory allocation fail";
	char isrErr[] = "No isr defined";
	char *msg;
	if(error == MEMORY_ERROR)
	{
		msg = memoryErr;
		uartWriteString(UART_USB,msg);
		while(1);
	}
	else if(error == MAX_TASK_ERROR)
	{
		msg = maxTaskErr;
		uartWriteString(UART_USB,msg);
	}
	else if(error == ISR_ERROR)
	{
		msg = isrErr;
		uartWriteString(UART_USB,msg);
	}
	else
	{
		while(1);
	}
}

/* */
void __attribute__((weak)) returnHook(void)
{
	/* In the implemented OS, no task should return, if it does it is because an error occurred */
	/* When returning this function, in the link register it must be a pointer to the error hook */
	while(1);
}
