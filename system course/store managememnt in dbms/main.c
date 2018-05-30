#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include "structs.h"
#include "db_api.h"
#include "file_storage.h"

void display_menu() {
	system("cls");
	printf("1. create table\n");
	printf("2. put\n");
	printf("3. update\n");
	printf("4. get\n");
	printf("5. scan\n");
	printf("6. delete\n");
	printf("7. dump\n");
	printf("8. re-create\n");
	printf("9. exit\n");
}

int main() {
	int i = 0, id_start, id_end;
	char *t;
	int* cols;
	int row_num;

	struct table* tab = NULL;
	struct table* temp;
	struct column_node* row = NULL;

	while (1)
	{
		display_menu();
		char c;
		scanf("%c", &c);
		system("cls");
		switch (c - 1)
		{
		case '0':
			create_table(&tab);
			break;
		case '1':
			printf("table name: ");
			t = malloc(20);
			scanf("%s", t);
			temp = update_temp(t, tab);
			free(t);
			if (temp)
			{
				printf("row: ");
				scanf("%d", &row_num);
				struct column_node* row = create_row(temp);
				insert(temp, row_num, row);
			}
			break;
		case '2':
			printf("table name: ");
			t = malloc(20);
			scanf("%s", t);
			temp = update_temp(t, tab);
			free(t);
			if (temp)
			{
				cols = malloc(temp->number_of_cols * sizeof(int));
				i = 0;
				for (; i < temp->number_of_cols; i++)
				{
					printf("want to update %s?<0, 1>: ", temp->column_names[i]);
					scanf("%d", &cols[i]);
				}
				printf("row: ");
				scanf("%d", &row_num);
				row = create_partial_row(temp, cols);
				insert_partial(temp, row, row_num, cols);
				free(cols);
			}
			break;
		case '3':
			printf("table name: ");
			t = malloc(20);
			scanf("%s", t);
			temp = update_temp(t, tab);
			free(t);
			if (temp)
			{
				printf("Enter row number to get: ");
				scanf("%d", &row_num);
				print_row(temp, get(temp, row_num));
			}
			system("pause");
			break;
		case '4':
			printf("table name: ");
			t = malloc(20);
			scanf("%s", t);
			temp = update_temp(t, tab);
			free(t);
			if (temp)
			{
				printf("Enter starting id to get: ");
				scanf("%d", &id_start);
				printf("Enter ending id to get: ");
				scanf("%d", &id_end);
				struct row_node** result_set = scan(temp, id_start, id_end);
				i = 0;
				for (; i < id_end - id_start + 1; i++)
					print_row(temp, result_set[i]);
			}
			system("pause");
			break;
		case '5':
			printf("table name: ");
			t = malloc(20);
			scanf("%s", t);
			temp = update_temp(t, tab);
			free(t);
			if (temp)
			{
				printf("Enter row number to delete: ");
				scanf("%d", &row_num);
				del(temp, row_num);
			}
			break;
		case '6':
			// back_up
			back_up(tab);
			break;
		case '7':
			restore(tab);
			break;
		case '8':
			return 0;
		default:
			break;
		}
	}
}
