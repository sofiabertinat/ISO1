/*
 * os_hook.h
 *
 *  Created on: Jul 30, 2022
 *      Author: Sofia Bertinat
 */

#ifndef ISO1_INC_OS_HOOK_H_
#define ISO1_INC_OS_HOOK_H_

#include "../inc/types.h"

/* */
void __attribute__((weak)) tickHook(void);
/* */
void __attribute__((weak)) errorHook(void *caller, uint16_t error);
/**/
void __attribute__((weak)) returnHook(void);

#endif /* ISO1_INC_OS_HOOK_H_ */
