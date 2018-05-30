#pragma once
#include <stdio.h>
#include "structs.h"
#include "db_api.h"

void back_up(struct table* tab) {
	struct table* temp = tab;
	int i = 0;
	while (temp)
	{
		while (i < temp->number_of_cols)
		{
			char * name = malloc(strlen(temp->name) + 10);
			strcpy(name, temp->name);
			name = strcat(name, "_");
			name = strcat(name, temp->column_names[i]);
			name = strcat(name, ".txt");
			FILE * backup = fopen(name, "a");
			if (!backup)
			{
				perror("IO Error");
				system("pause");
				exit(1);
			}

			struct row_node* row = temp->head_row;
			while (row)
			{
				struct column_node* col = row->row;
				while (col && strcmp(temp->column_names[i], col->name))
				{
					col = col->next_column;
				}
				if (temp->types[i] == 0)
				{
					fprintf(backup, "%d,%d,%d\n", row->id, col->version, *((int*)col->value));
				}
				else
				{
					fprintf(backup, "%d,%d,%s\n", row->id, col->version, ((char*)col->value));
				}
				row = row->next_row;
			}
			fclose(backup);
			i++;
		}
		temp = temp->next_table;
	}
}

void restore(struct table* tab) {
	struct table* temp = tab;
	int i = 0;
	while (temp)
	{
		while (i < temp->number_of_cols)
		{
			char * name = malloc(strlen(temp->name) + 10);
			strcpy(name, temp->name);
			name = strcat(name, "_");
			name = strcat(name, temp->column_names[i]);
			name = strcat(name, ".txt");
			FILE * restore = fopen(name, "r");
			if (!restore)
			{
				perror("IO Error");
				system("pause");
				exit(1);
			}

			struct row_node** row = &(temp->head_row);
			(*row) = create_row_file(temp, restore);
			while ((*row))
			{
				/*
				struct column_node* col = (*row)->row;
				while (col && strcmp(temp->column_names[i], col->name))
				{
					col = col->next_column;
				}
				if (temp->types[i] == 0)
				{
					fscanf(restore, "%d,%d,%d\n", (*row)->id, col->version, *((int*)col->value));
				}
				else
				{
					fscanf(restore, "%d,%d,%s\n", (*row)->id, col->version, ((char*)col->value));
				}
				*/
				(*row)->next_row = create_row_file(temp, restore);
				(*row) = (*row)->next_row;
			}
			
			fclose(restore);
			i++;
		}
		temp = temp->next_table;
	}
}
