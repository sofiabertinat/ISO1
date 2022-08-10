/*
 * os_queue.c
 *
 *  Created on: Aug 1, 2022
 *      Author: Sofia Bertinat
 */

#include "../inc/os_queue.h"
#include "../inc/os_core.h"

/**/
queueItem_t* os_queue_item_create(void* item)
{
	queueItem_t* new_item;

	new_item = (queueItem_t*) malloc(sizeof(queueItem_t));

	if(new_item == NULL)
	{
		os_set_error(NULL, MEMORY_ERROR);
		return NULL;
	}

	new_item->item =  item;
	new_item->next_item = NULL;

	return new_item;
}

/**/
queue_t* os_queue_create(void)
{
	queue_t* new_queue;

	new_queue = (queue_t*) malloc(sizeof(queue_t));

	if(new_queue == NULL)
	{
		os_set_error(NULL, MEMORY_ERROR);
		return NULL;
	}

	new_queue->last_item  =  NULL;
	new_queue->first_item = NULL;
	new_queue->count = 0;
	new_queue->task_blocked = NULL;

	return new_queue;
}

/**/
void os_queue_item_delete(queue_t* queue)
{
	queueItem_t* aux_item;

	if(queue->first_item != NULL)
	{
		aux_item = queue->first_item;
		queue->first_item = queue->first_item->next_item;

		queue->count--;

		free(aux_item);

		if(queue->first_item == NULL)
			queue->last_item = NULL;
	}
}

/**/
void os_queue_delete(queue_t* queue)
{
	while(queue->first_item != NULL)
	{
		os_queue_item_delete(queue);
	}
	free(queue);
}

/**/
void* os_queue_read( queue_t* queue)
{
	void* item;

	if(queue->first_item == NULL)
	{
		item =  NULL;
		queue->task_blocked = os_get_current_task();
		os_block_current_task();
	}

	if(queue->first_item != NULL)
	{
		item = queue->first_item->item;
		os_queue_item_delete(queue);
		if(queue->task_blocked != NULL)
		{
			os_unblock_task(queue->task_blocked);
			queue->task_blocked = NULL;
		}
	}
	return item;
}

/**/
void os_queue_write( queue_t* queue, void* item )
{
	queueItem_t* new_item = os_queue_item_create(item);

	if(new_item)
	{
		new_item->item = item;

		if(queue->first_item == NULL)
		{
			queue->first_item = new_item;
			queue->last_item = new_item;
			queue->count++;
			if(queue->task_blocked != NULL)
			{
				os_unblock_task(queue->task_blocked);
				queue->task_blocked = NULL;
			}
		}
		else
		{
			if(queue->count == MAX_QUEUE_ITEM)
			{
				queue->task_blocked = os_get_current_task();
				os_block_current_task();
			}
			else
			{
				queue->last_item->next_item = new_item;
				queue->last_item = new_item;
				queue->count++;
				if(queue->task_blocked != NULL)
				{
					os_unblock_task(queue->task_blocked);
					queue->task_blocked = NULL;
				}
			}
		}
	}

}
