/*
 * app.h
 *
 *  Created on: Jul 17, 2022
 *      Author: Sofia Bertinat
 */

#ifndef ISO1_INC_APP_H_
#define ISO1_INC_APP_H_

#include "../inc/types.h"

/* */
void led_task(void );

/* */
void display_task(void );

/* */
void test_hook_rtn_task(void );

/**/
void init_queue(void);

#endif /* ISO1_INC_APP_H_ */
