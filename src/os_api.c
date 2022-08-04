/*
 * os_api.c
 *
 *  Created on: Aug 1, 2022
 *      Author: Sofia Bertinat
 */

#include "../inc/os_api.h"
#include "../inc/os_core.h"

#define TickRateMS 1

/**/
void os_delay(uint32_t time_ms)
{
	uint32_t aux_ticks;
	uint32_t current_ticks;

	os_enter_critial();

	current_ticks = os_get_tick_count();

	aux_ticks = current_ticks + time_ms /TickRateMS;

	os_block_current_task();

	os_set_current_task_ticks(aux_ticks);

	os_exit_critical();

}
