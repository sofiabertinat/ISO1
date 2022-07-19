/*
 * table.h
 *
 *  Created on: Jul 18, 2022
 *      Author: Sofia Bertinat
 */

#ifndef ISO1_INC_TABLE_H_
#define ISO1_INC_TABLE_H_

#include "../inc/types.h"

/* */
typedef struct _table
{
	void ** pItem;
	uint32_t tableInitSize;
	uint32_t itemAmount;
} table_t;

/* */
bool table_init(table_t * table, void * item, uint32_t item_size);
/* */
bool table_add_item(table_t * table, void * item);
/* */
bool table_del_item(table_t * table, void * item);
/* */
u_int32_t table_max_item_number( table_t * table);
/* */
bool table_get_item( table_t * table, u_int32_t i, void * item);

#endif /* ISO1_INC_TABLE_H_ */
