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
void write_task(void );

/* */
void display_task(void );

/**/
void init_queue(void);

/* */
void led_on_task(void);

/* */
void led_off_task(void);

/**/
void init_sem_led(void);

#endif /* ISO1_INC_APP_H_ */
