/*
 * os_irq.h
 *
 *  Created on: Aug 13, 2022
 *      Author: Sofia Bertinat
 */

#ifndef ISO1_INC_OS_IRQ_H_
#define ISO1_INC_OS_IRQ_H_

#include "../inc/types.h"
#include "../inc/os_core.h"

/* */
void os_isr_register(LPC43XX_IRQn_Type irq, void* pIsr, uint8_t param);
/* */
void os_isr_unregister( LPC43XX_IRQn_Type irq);
/**/
void os_my_IRQ_Init (void);


#endif /* ISO1_INC_OS_IRQ_H_ */
