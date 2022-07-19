/*
 * table.c
 *
 *  Created on: Jul 18, 2022
 *      Author: Sofia Bertinat
 */

#include "../inc/table.h"

/* */
bool table_init(table_t * table, void * item, uint32_t item_size)
{
	table_t * aux_table;
	void ** p_item;

	aux_table = (table_t*) malloc( sizeof(table_t));
	p_item = (void*) malloc( item_size );

	if ((aux_table == NULL) || (p_item == NULL))
	{
		return RETURN_FAIL;
	}

	*p_item = item;
	aux_table->itemAmount = 1;
	aux_table->tableInitSize = item_size;
	aux_table->pItem = p_item;

	table = aux_table;

	return RETURN_OK;
}

/* */
bool table_add_item(table_t * table, void * item)
{
	void ** p_item;
	uint32_t i;

	p_item = (void*)malloc((table->itemAmount+1)*sizeof(table->tableInitSize));

	// Check if the memory has been successfully allocated by malloc
	if (p_item == NULL)
	{
		return RETURN_FAIL;
	}

	for(i=0;i<table->itemAmount;i++)
	{
		p_item[i]  = table->pItem[i];
	}

	p_item[i]  = item;
	table->itemAmount++;

	free(table->pItem);

	table->pItem = p_item;

	return RETURN_OK;
}

/* */
bool table_del_item(table_t * table, void * item)
{
	uint32_t i, j;

	if(table->itemAmount == 1)
		free(table);

	for(i=0;i<table->itemAmount;i++)
	{
		if (item == table->pItem[i])
		{
			for(j=i;j<(table->itemAmount-1);j++)
			{
				table->pItem[j] = table->pItem[j+1];
			}
			free(table->pItem[j]);
			table->itemAmount--;
		}
	}

	return RETURN_OK;
}

/* */
u_int32_t table_max_item_number( table_t * table)
{

	return table->itemAmount;
}

/* */
bool table_get_item( table_t * table, u_int32_t i, void * item)
{
	if((i > table->itemAmount) || (i == 0))
		return RETURN_FAIL;

	item = table->pItem[i-1];

	return RETURN_OK;
}





