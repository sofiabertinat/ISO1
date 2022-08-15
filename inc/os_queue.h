/*
 * os_queue.h
 *
 *  Created on: Aug 1, 2022
 *      Author: Sofia Bertinat
 */

#ifndef ISO1_INC_OS_QUEUE_H_
#define ISO1_INC_OS_QUEUE_H_

#include "../inc/types.h"
#include "../inc/os_task.h"

#define MAX_QUEUE_ITEM 40

typedef struct _queueItem
{
	void*					item;
	struct _queueItem *	next_item;
} queueItem_t;

typedef struct _queue
{
	queueItem_t * 	first_item;
	queueItem_t * 	last_item;
	uint32_t 		count;
	task_t* 		task_blocked;
} queue_t;

/**/
queueItem_t* os_queue_item_create(void* item);

/**/
queue_t* os_queue_create(void);

/**/
void os_queue_item_delete(queue_t* queue);

/**/
void os_queue_delete(queue_t* queue);

/**/
void* os_queue_read( queue_t* queue);

/**/
void os_queue_write( queue_t* queue, void* item , bool from_irq);

#endif /* ISO1_INC_OS_QUEUE_H_ */
