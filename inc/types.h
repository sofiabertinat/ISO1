/*
 * types.h
 *
 *  Created on: Jul 17, 2022
 *      Author: Sofia Bertinat
 */

#ifndef ISO1_INC_TYPES_H_
#define ISO1_INC_TYPES_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "board.h"
#include "cmsis_43xx.h"

#define ERROR		-1
#define RETURN_FAIL  0
#define RETURN_OK 	 1

#define MS_IN_S		1000

/* stack register position */
#define XPSR			1	/* PSR: program counter register*/
#define PC_REG			2	/* PC (R15): Program Counter, current program address direction. reset value: 0x00000004.*/
#define LR				3	/* LR (R14): Link Register save return information */
#define R12				4
#define R3				5
#define R2				6
#define R1				7
#define R0				8
#define LR_PREV_VALUE	9
#define R4				10
#define R5				11
#define R6				12
#define R7				13
#define R8				14
#define R9				15
#define R10 			16
#define R11 			17

#endif /* ISO1_INC_TYPES_H_ */
