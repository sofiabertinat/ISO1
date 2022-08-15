/*
 * app.h
 *
 *  Created on: Jul 17, 2022
 *      Author: Sofia Bertinat
 */

#ifndef ISO1_INC_APP_H_
#define ISO1_INC_APP_H_

#include "../inc/types.h"

typedef enum button_state
{
	UP,
	FALLING,
	DOWN,
	RISING
}button_state_t;

/* indices de teclas para el vector de estructuras */
typedef enum buttons
{
	Tecla1,
	Tecla2,
	Tecla3,
	Tecla4
}buttons_t;

/* estructura de control de datos capturados por la irq */
typedef struct button_control
{
	button_state_t   Flanco;
	buttons_t        Tecla;
} button_control_t;

/* estructura de control de la maquina de estados de cada boton */
typedef struct buttons_SM
{
	buttons_t 		button;
	button_state_t 	state;
}buttons_SM_t;


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
void init_sems(void);

/**/
void botton_task(void);

/**/
bool button_rise_IRQHandler(void* param);

/**/
bool button_fall_IRQHandler(void* param);

#endif /* ISO1_INC_APP_H_ */
