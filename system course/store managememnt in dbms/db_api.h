#pragma once
#include "structs.h"
#include <stdio.h>

void create_table(struct table** tab) {
	struct table* temp = malloc(sizeof(struct table));
	temp->head_row = NULL;
	temp->next_table = *tab;
	*tab = temp;
	temp->column_names = NULL;
	printf("Enter table name: ");
	scanf("%s", temp->name);
	printf("number of columns: ");
	scanf("%d", &temp->number_of_cols);
	temp->column_names = malloc(temp->number_of_cols * sizeof(char*));
	temp->types = malloc(sizeof(int) * temp->number_of_cols);
	int i = 0;
	char *t;
	for (; i < temp->number_of_cols; i++)
	{
		printf("col[%i] name: ", i + 1);
		t = malloc(20);
		scanf("%s", t);
		temp->column_names[i] = t;
		printf("col[%i] type<0, 1>: ", i + 1);
		scanf("%d", &temp->types[i]);
	}
}

struct column_node* create_col_node(char* cloumn_name, void* val) {
	struct column_node* temp = (struct column_node*)malloc(sizeof(struct column_node));
	temp->name = malloc(strlen(cloumn_name));
	strcpy(temp->name, cloumn_name);
	temp->next_column = NULL;
	temp->next_version = NULL;
	temp->value = val;
	temp->version = 0;
	return temp;
}

void print_table(struct table* tab) {
	int i = 0;
	struct row_node* row = tab->head_row;
	while (row != NULL)
	{
		struct column_node* temp = row->row;
		while (temp != NULL)
		{
			struct column_node* next_version = temp;
			printf("%s\n", next_version->name);
			while (next_version != NULL)
			{
				if (tab->types[i] == 0)
				{
					printf("%d ", *((int*)next_version->value));
				}
				else if (tab->types[i] == 1)
				{
					printf("%s ", ((char*)next_version->value));
				}
				next_version = next_version->next_version;
			}
			printf("\n");
			temp = temp->next_column;
			i++;
		}
		printf("\n");
		row = row->next_row;
	}
}

void print_row(struct table* tab, struct row_node* row) {
	int i = 0;
	if (row != NULL && row->row != NULL)
	{
		struct column_node* temp = row->row;
		while (temp != NULL)
		{
			struct column_node* next_version = temp;
			if (tab->types[i] == 0)
			{
				printf("%s: %d\n", next_version->name, *((int*)next_version->value));
			}
			else if (tab->types[i] == 1)
			{
				printf("%s: %s\n", next_version->name, ((char*)next_version->value));
			}
			i++;
			temp = temp->next_column;
		}
		printf("\n");
	}
}

void insert(struct table *tab, int id, struct column_node* start) {
	struct row_node** row = &(tab->head_row);
	if (*row == NULL)
	{
		(*row) = malloc(sizeof(struct row_node));
		(*row)->id = id;
		(*row)->next_row = NULL;
		(*row)->row = start;
		(*row)->commit = 0;
		(*row)->current = 0;
		return;
	}
	while ((*row)->next_row)
		(*row) = (*row)->next_row;
	(*row)->next_row = malloc(sizeof(struct row_node));
	(*row)->next_row->id = id;
	(*row)->next_row->next_row = NULL;
	(*row)->next_row->row = start;
	(*row)->next_row->commit = 0;
	(*row)->next_row->current = 0;
}

void insert_partial(struct table *tab, struct column_node* start, int id, int *cols) {
	struct row_node** row = &(tab->head_row);
	if (*row != NULL)
	{
		struct column_node* next_column = (*row)->row;
		struct column_node** prev_column = &((*row)->row);
		struct column_node* start_next_column = start;
		int i = 0;
		for (; i < tab->number_of_cols; i++)
		{
			if (cols[i])
			{
				if (next_column && start_next_column && !(strcmp(next_column->name, start_next_column->name)))
				{
					struct column_node* start_next_version = start_next_column;
					struct column_node* start_next_temp = start_next_column->next_column;
					struct column_node* next_version = next_column;

					start_next_column->next_column = NULL;

					start_next_version->version = next_version->version + 1;
					start_next_version->next_column = next_version->next_column;
					start_next_version->next_version = next_version;
					next_version->next_column = NULL;


					(*prev_column) = start_next_version;
					prev_column = &((*prev_column)->next_column);

					next_column = start_next_version->next_column;
					start_next_column = start_next_temp;
				}
				else
				{
					struct column_node* start_next_version = start;
					struct column_node* start_next_temp = start;
					struct column_node* next_version = *prev_column;

					start_next_temp = start->next_column;
					start->next_column = next_version->next_column;
					(*prev_column)->next_column = next_column;
					next_version->next_column = start;
					start = start_next_temp;
				}
			}
		}
	}
	else
	{
		insert(tab, id, start);
	}
	(*row)->current++;
	(*row)->commit++;
}

struct row_node* get(struct table* tab, int id) {
	struct row_node* temp = tab->head_row;
	if (!temp)
	{
		return NULL;
	}
	else if (!temp->row)
	{
		return NULL;
	}
	else
	{
		while (temp && temp->id != id)
		{
			temp = temp->next_row;
		}
		return temp;
	}
}

struct column_node* get_column(struct table* tab, struct row_node* row, int column) {
	struct column_node* temp = row->row;
	if (!temp)
	{
		return NULL;
	}
	else
	{
		while (temp && strcmp(temp->name, tab->column_names[column]))
		{
			temp = temp->next_column;
		}
		return temp;
	}
}

struct row_node** scan(struct table* tab, int id_start, int id_end) {
	int count = id_end - id_start + 1;
	struct row_node** result_set = malloc(sizeof(struct row_node*) * count);
	int i = id_start;
	for (; i < id_end + 1; i++)
	{
		result_set[i] = get(tab, i);
	}
	return result_set;
}

void del(struct table* tab, int id) {
	if (tab->head_row)
	{
		if (tab->head_row->id == id)
		{
			struct row_node* row = tab->head_row;
			tab->head_row = tab->head_row->next_row;
			row->next_row = NULL;
			struct column_node* column = row->row;
			while (column)
			{
				struct column_node* version = column;
				column = column->next_column;
				while (version) {
					struct column_node* temp = version;
					version = version->next_version;
					free(temp->value);
					temp->value = NULL;
					temp->next_version = NULL;
					temp->next_column = NULL;
					free(temp);
					temp = NULL;
				}
			}
			free(row);
			row = NULL;
		}
		else
		{
			struct row_node* row = tab->head_row;
			struct row_node** row_addr = &(tab->head_row);
			struct row_node** prev_row = &row;
			while ((row))
			{
				if (id == (row)->id)
				{
					(*prev_row)->next_row = (row)->next_row;
					(row)->next_row = NULL;
					struct column_node* column = (row)->row;
					while (column)
					{
						struct column_node* version = column;
						column = column->next_column;
						while (version) {
							struct column_node* temp = version;
							version = version->next_version;
							free(temp->value);
							temp->value = NULL;
							temp->next_version = NULL;
							temp->next_column = NULL;
							free(temp);
							temp = NULL;
						}
					}
					(*row_addr)->row = NULL;
					free((*row_addr)->next_row);
					(*row_addr)->next_row = NULL;
					(*row_addr) = NULL;
					break;
				}
				else
				{
					(*prev_row) = (row);
					(row) = (row)->next_row;
					row_addr = &row;
				}
			}
		}
	}
}

struct column_node* create_row(struct table* tab) {
	int i = 1;
	void* temp = NULL;
	struct column_node *t, *head = NULL, *iter = NULL;
	iter = head;
	if (head == NULL)
	{
		printf("%s: ", tab->column_names[0]);
		if (tab->types[0] == 0)
		{
			temp = malloc(sizeof(int));
			scanf("%d", (int*)temp);
		}
		else if (tab->types[0] == 1)
		{
			temp = malloc(sizeof(char) * 20);
			scanf("%s", (char*)temp);
		}
		t = create_col_node(tab->column_names[0], temp);
		iter = t;
		head = iter;
	}
	for (; i < tab->number_of_cols; i++)
	{
		printf("%s: ", tab->column_names[i]);
		if (tab->types[i] == 0)
		{
			temp = malloc(sizeof(int));
			scanf("%d", (int*)temp);
		}
		else if (tab->types[i] == 1)
		{
			temp = malloc(sizeof(char) * 20);
			scanf("%s", (char*)temp);
		}
		t = create_col_node(tab->column_names[i], temp);
		iter->next_column = t;
		iter = iter->next_column;
	}
	return head;
}

struct column_node* create_partial_row(struct table* tab, int* cols_to_update) {
	int i = 0;
	void* temp = NULL;
	struct column_node *t, *head = NULL, *iter = NULL;
	iter = head;

	while (head == NULL)
	{
		if (cols_to_update[i])
		{
			printf("%s: ", tab->column_names[i]);
			if (tab->types[i] == 0)
			{
				temp = malloc(sizeof(int));
				scanf("%d", (int*)temp);
			}
			else if (tab->types[i] == 1)
			{
				temp = malloc(sizeof(char) * 20);
				scanf("%s", (char*)temp);
			}
			t = create_col_node(tab->column_names[i], temp);
			iter = t;
			head = iter;
		}
		i++;
	}

	for (; i < tab->number_of_cols; i++)
	{
		if (cols_to_update[i])
		{
			printf("%s: ", tab->column_names[i]);
			if (tab->types[i] == 0)
			{
				temp = malloc(sizeof(int));
				scanf("%d", (int*)temp);
			}
			else if (tab->types[i] == 1)
			{
				temp = malloc(sizeof(char) * 20);
				scanf("%s", (char*)temp);
			}
			t = create_col_node(tab->column_names[i], temp);
			iter->next_column = t;
			iter = iter->next_column;
		}
	}
	return head;
}

struct column_node* create_row_file(struct table* tab, int cols_to_update, FILE* fp) {
	int i = 1;
	void* temp = NULL;
	struct column_node *t, *head = NULL, *iter = NULL;
	iter = head;
	if (head == NULL)
	{
		if (tab->types[cols_to_update] == 0)
		{
			temp = malloc(sizeof(int));
			fscanf(fp, "%d,", (int*)temp);
		}
		else if (tab->types[cols_to_update] == 1)
		{
			temp = malloc(sizeof(char) * 20);
			fscanf(fp, "%s,", (char*)temp);
		}
		t = create_col_node(tab->column_names[0], temp);
		iter = t;
		head = iter;
	}
	for (; i < tab->number_of_cols - 1; i++)
	{
		if (tab->types[i] == 0)
		{
			temp = malloc(sizeof(int));
			fscanf(fp, "%d,", (int*)temp);
		}
		else if (tab->types[i] == 1)
		{
			temp = malloc(sizeof(char) * 20);
			fscanf(fp, "%s,", (char*)temp);
		}
		t = create_col_node(tab->column_names[i], temp);
		iter->next_column = t;
		iter = iter->next_column;
	}

	if (tab->types[i] == 0)
	{
		temp = malloc(sizeof(int));
		fscanf(fp, "%d", (int*)temp);
	}
	else if (tab->types[i] == 1)
	{
		temp = malloc(sizeof(char) * 20);
		fscanf(fp, "%s", (char*)temp);
	}
	t = create_col_node(tab->column_names[i], temp);
	iter->next_column = t;
	iter = iter->next_column;

	return head;
}

struct table* update_temp(char* table, struct table* tab) {
	struct table* temp = tab;
	while (temp)
	{
		if (!strcmp(temp->name, table))
			break;
		temp = temp->next_table;
	}
	if (!temp)
	{
		printf("%s: table not found\n", table);
	}
	return temp;
}
