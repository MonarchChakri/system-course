#pragma once
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include "structs.h"

int offset = 0;
int num_records_read = 0;
int num_index_pages_read = 0;

char* gettoken(char** sp, char delim)
{
	if (*sp == NULL) {
		return "\0";
	}
	int tokensize = 50;
	char * res = (char*)malloc(tokensize * sizeof(char));
	char c;
	int i = 0;
	for (; 1; i++)
	{
		c = **sp;
		if (c == EOF || c == delim || c == '\0')
			break;
		if (i == tokensize - 1)
		{
			tokensize *= 2;
			char* temp = (char*)malloc(tokensize * sizeof(char));
			strncpy(temp, res, tokensize / 2);
			free(res);
			res = temp;
		}
		res[i] = c;
		(*sp)++;
	}
	res[i] = '\0';
	(*sp)++;
	return res;
}

void create_index_pages_from_index_page(FILE** output_file) {
	int i = 0;
	struct index_page* ip_prev_page_type;
	struct index_page* ip_current_page_type;
	ip_current_page_type = malloc(sizeof(struct index_page));
	memset(ip_current_page_type, 0, sizeof(struct index_page));
	ip_current_page_type->child_page_id[0] = offset - 128;
	while (num_index_pages_read < ((NUMBER_OF_RECORDS / 3) / 16) - 1)
	{
		fseek(*output_file, offset, SEEK_SET);
		ip_prev_page_type = malloc(sizeof(struct index_page));
		fread(ip_prev_page_type, SIZE_OF_INDEX_PAGE, 1, *output_file);

		ip_current_page_type->key[i] = ip_prev_page_type->key[0];
		ip_current_page_type->child_page_id[i + 1] = offset;

		offset += DATA_PAGES_PER_INDEX_PAGE * 8;
		free(ip_prev_page_type);
		ip_prev_page_type = NULL;
		num_index_pages_read++;
		i++;
	}
	fclose(*output_file);
	*output_file = fopen("result.bin", "ab");
	fwrite(ip_current_page_type, SIZE_OF_INDEX_PAGE, 1, *output_file);
	free(ip_current_page_type);
	ip_current_page_type = NULL;
	fclose(*output_file);
	*output_file = fopen("result.bin", "rb");

}

void create_index_pages_from_data_page(FILE** output_file, int size) {
	offset += DATA_PAGES_PER_INDEX_PAGE * 8;
	int i = 0;
	void** dps;
	int data_page_type, table_page_type;
	struct index_page* ip;
	while (num_records_read < (NUMBER_OF_RECORDS / size))
	{
		fseek(*output_file, offset, SEEK_CUR);
		dps = malloc(sizeof(void*) * DATA_PAGES_PER_INDEX_PAGE);

		for (; i < DATA_PAGES_PER_INDEX_PAGE - 1; i++)
		{
			void* buff = malloc(128);
			fread(buff, 128, 1, *output_file);
			data_page_type = ((char*)buff)[1] - '0';
			table_page_type = ((char*)buff)[0] - '0';
			dps[i] = malloc(SIZE_OF_STUDENT_DATA_PAGE);
			dps[i] = buff;
			//fseek(*output_file, -128, SEEK_CUR);
			//fread(dps[i], 128, 1, *output_file);
			num_records_read += 1;
		}

		fclose(*output_file);
		*output_file = fopen("result.bin", "ab");
		ip = calloc(DATA_PAGES_PER_INDEX_PAGE, sizeof(struct index_page));
		memset(ip, 0, sizeof(struct index_page));

		int k = 0;
		i = 0;
		ip->child_page_id[k] = offset - 128;
		k++;
		for (; k < DATA_PAGES_PER_INDEX_PAGE; k++) {
			ip->child_page_id[k] = offset;
			if (data_page_type == 0)
			{
				ip->key[k - 1] = ((struct data_page_student*)dps[k + i - 1])->data[0].id;
			}
			else
			{
				ip->key[k - 1] = ((struct data_page_marks*)dps[k + i - 1])->data[0].s_id;
			}
			offset += DATA_PAGES_PER_INDEX_PAGE * 8;
		}

		offset += DATA_PAGES_PER_INDEX_PAGE * 8;
		if (data_page_type == 0)
		{
			ip->table_id = ((struct data_page_student*)dps[0])->table_id;
		}
		else
		{
			ip->table_id = ((struct data_page_marks*)dps[0])->table_id;
		}
		ip->page_type = '2';
		free(dps);
		dps = NULL;
		fwrite(ip, SIZE_OF_INDEX_PAGE, 1, *output_file);
		free(ip);
		ip = NULL;
		fclose(*output_file);
		*output_file = fopen("result.bin", "rb");
	}
	fclose(*output_file);
	*output_file = fopen("result.bin", "rb");
}

struct student* create_student_from_line(char* line) {
	struct student* t = malloc(sizeof(struct student));
	memset(t, 0, sizeof(struct student));

	char* buff = malloc(strlen(line) + 5);
	char* temp = buff;
	strcpy(buff, line);

	t->id = atoi(gettoken(&buff, ','));
	strcpy(t->name, gettoken(&buff, ','));

	free(temp);
	return t;
}

struct marks* create_marks_from_line(char* line) {
	struct marks* t = malloc(sizeof(struct marks));
	memset(t, 0, sizeof(struct marks));

	char* buff = malloc(strlen(line) + 5);
	char* temp = buff;
	strcpy(buff, line);

	t->s_id = atoi(gettoken(&buff, ','));
	t->marks[0] = atoi(gettoken(&buff, ','));
	t->marks[1] = atoi(gettoken(&buff, ','));
	t->marks[2] = atoi(gettoken(&buff, ','));
	t->marks[3] = atoi(gettoken(&buff, ','));

	free(temp);
	return t;
}

void* create_data_page_from_file(FILE* data_file, int data_page_type) {
	num_records_read = 0;
	void* dp;

	if (!data_page_type)
	{
		dp = malloc(sizeof(struct data_page_student));
		memset(dp, 0, sizeof(struct data_page_student));
	}
	else
	{
		dp = malloc(sizeof(struct data_page_marks));
		memset(dp, 0, sizeof(struct data_page_marks));
	}

	int k = 0;
	char* buff = NULL;
	char* temp = buff;

	for (; ; ) {
		buff = malloc(BUFSIZ);
		temp = buff;
		if (!data_page_type)
		{
			if (k < RECORDS_PER_DATA_PAGE_STUDENT) {
				if (fgets(buff, BUFSIZ, data_file) != NULL)
				{
					((struct data_page_student*)dp)->data[k] = *create_student_from_line(buff);
					num_records_read += 1;
					k++;
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
		else
		{
			if (k < RECORDS_PER_DATA_PAGE_MARKS) {
				if (fgets(buff, BUFSIZ, data_file) != NULL)
				{
					((struct data_page_marks*)dp)->data[k] = *create_marks_from_line(buff);
					num_records_read += 1;
					k++;
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
		free(temp);
		temp = NULL;
	}

	if (k == 0)
	{
		return NULL;
	}

	if (!data_page_type)
	{
		((struct data_page_student*)dp)->page_type = '1';
		((struct data_page_student*)dp)->table_id = '0';
	}
	else
	{
		((struct data_page_marks*)dp)->page_type = '1';
		((struct data_page_marks*)dp)->table_id = '1';
	}
	return dp;
}

void create_table_page_form_offsets(FILE** output_file) {
	struct table_page tablepage;
	memset(&tablepage, 0, sizeof(struct table_page));
	tablepage.page_type = '3';
	tablepage.table_id = '\0';
	tablepage.root_page_offset[0] = 16 * 128;
	tablepage.root_page_offset[1] = 25 * 128;
	strcpy(tablepage.table_name[0], "studs");
	strcpy(tablepage.table_name[1], "marks");
	fwrite(&tablepage, 128, 1, *output_file);
}

struct table_page* load_table_page_form_file(FILE** output_file) {
	struct table_page* tablepage = malloc(sizeof(struct table_page));
	fseek(*output_file, -128, SEEK_END);
	fread(tablepage, sizeof(struct table_page), 1, *output_file);
	return tablepage;
}

void get_student_range_print(int s_id, int e_id, FILE** data_file, int ofst) {
	struct index_page* index_page;

	int seek_offset_start = ofst;
	int i = 0;
	for (; i < LEVEL_OF_TREE; i++)
	{
		index_page = malloc(SIZE_OF_INDEX_PAGE);
		fseek(*data_file, seek_offset_start, SEEK_SET);
		fread(index_page, SIZE_OF_INDEX_PAGE, 1, *data_file);

		int j = 0;
		for (; j < 15; j++)
		{
			if (s_id < index_page->key[j])
			{
				seek_offset_start = index_page->child_page_id[j];
				free(index_page);
				index_page = NULL;
				goto out_start;
			}
		}
		if (s_id > index_page->key[14])
		{
			seek_offset_start = index_page->child_page_id[14];
			free(index_page);
			index_page = NULL;
			break;
		}
	out_start:;
	}

	int seek_offset_end = ofst;
	i = 0;
	for (; i < LEVEL_OF_TREE; i++)
	{
		index_page = malloc(SIZE_OF_INDEX_PAGE);
		fseek(*data_file, seek_offset_end, SEEK_SET);
		fread(index_page, SIZE_OF_INDEX_PAGE, 1, *data_file);

		int j = 0;
		for (; j < 15; j++)
		{
			if (e_id < index_page->key[j])
			{
				seek_offset_end = index_page->child_page_id[j];
				free(index_page);
				index_page = NULL;
				goto out_end;
			}
		}
		if (e_id > index_page->key[14])
		{
			seek_offset_end = index_page->child_page_id[14];
			free(index_page);
			index_page = NULL;
			break;
		}
	out_end:;
	}

	struct data_page_student* data_page_student_start = NULL;
	while (seek_offset_start < seek_offset_end) {
		data_page_student_start = malloc(SIZE_OF_STUDENT_DATA_PAGE);
		fseek(*data_file, seek_offset_start, SEEK_SET);
		fread(data_page_student_start, SIZE_OF_STUDENT_DATA_PAGE, 1, *data_file);

		i = 0;
		for (; i < RECORDS_PER_DATA_PAGE_STUDENT; i++)
		{
			if (s_id <= data_page_student_start->data[i].id)
			{
				printf("%d, %s\n", data_page_student_start->data[i].id, data_page_student_start->data[i].name);
			}
		}

		seek_offset_start += 128;
	}

	struct data_page_student* data_page_student_end = malloc(SIZE_OF_STUDENT_DATA_PAGE);
	fseek(*data_file, seek_offset_end, SEEK_SET);
	fread(data_page_student_end, SIZE_OF_STUDENT_DATA_PAGE, 1, *data_file);

	i = 0;
	for (; i < RECORDS_PER_DATA_PAGE_STUDENT; i++)
	{
		if (e_id >= data_page_student_end->data[i].id)
		{
			printf("%d, %s\n", data_page_student_end->data[i].id, data_page_student_end->data[i].name);
		}
	}

	free(data_page_student_start);
	free(data_page_student_end);
	free(index_page);
}

void get_marks_range_print(int s_id, int e_id, FILE** data_file, int ofst) {
	struct index_page* index_page;

	int seek_offset_start = ofst;
	int i = 0;
	for (; i < LEVEL_OF_TREE; i++)
	{
		index_page = malloc(SIZE_OF_INDEX_PAGE);
		fseek(*data_file, seek_offset_start, SEEK_SET);
		fread(index_page, SIZE_OF_INDEX_PAGE, 1, *data_file);

		int j = 0;
		for (; j < 15; j++)
		{
			if (s_id < index_page->key[j])
			{
				seek_offset_start = index_page->child_page_id[j];
				free(index_page);
				index_page = NULL;
				goto out_start;
			}
		}
		if (s_id > index_page->key[14])
		{
			seek_offset_start = index_page->child_page_id[14];
			free(index_page);
			index_page = NULL;
			break;
		}
	out_start:;
	}

	int seek_offset_end = ofst;
	i = 0;
	for (; i < LEVEL_OF_TREE; i++)
	{
		index_page = malloc(SIZE_OF_INDEX_PAGE);
		fseek(*data_file, seek_offset_end, SEEK_SET);
		fread(index_page, SIZE_OF_INDEX_PAGE, 1, *data_file);

		int j = 0;
		for (; j < 15; j++)
		{
			if (e_id < index_page->key[j])
			{
				seek_offset_end = index_page->child_page_id[j];
				free(index_page);
				index_page = NULL;
				goto out_end;
			}
		}
		if (e_id > index_page->key[14])
		{
			seek_offset_end = index_page->child_page_id[14];
			free(index_page);
			index_page = NULL;
			break;
		}
	out_end:;
	}

	struct data_page_marks* data_page_marks_start = NULL;
	while (seek_offset_start < seek_offset_end) {
		data_page_marks_start = malloc(SIZE_OF_MARKS_DATA_PAGE);
		fseek(*data_file, seek_offset_start, SEEK_SET);
		fread(data_page_marks_start, SIZE_OF_MARKS_DATA_PAGE, 1, *data_file);

		i = 0;
		for (; i < RECORDS_PER_DATA_PAGE_MARKS; i++)
		{
			if (s_id <= data_page_marks_start->data[i].s_id)
			{
				printf("%d, %d, %d, %d, %d\n", data_page_marks_start->data[i].s_id,
					data_page_marks_start->data[i].marks[0], data_page_marks_start->data[i].marks[1],
					data_page_marks_start->data[i].marks[2], data_page_marks_start->data[i].marks[3]);
			}
		}

		seek_offset_start += 128;
	}

	struct data_page_marks* data_page_marks_end = malloc(SIZE_OF_MARKS_DATA_PAGE);
	fseek(*data_file, seek_offset_end, SEEK_SET);
	fread(data_page_marks_end, SIZE_OF_MARKS_DATA_PAGE, 1, *data_file);

	i = 0;
	for (; i < RECORDS_PER_DATA_PAGE_MARKS; i++)
	{
		if (e_id >= data_page_marks_end->data[i].s_id)
		{
			printf("%d, %d, %d, %d, %d\n", data_page_marks_end->data[i].s_id,
				data_page_marks_end->data[i].marks[0], data_page_marks_end->data[i].marks[1],
				data_page_marks_end->data[i].marks[2], data_page_marks_end->data[i].marks[3]);
		}
	}

	free(data_page_marks_start);
	free(data_page_marks_end);
	free(index_page);
}

int get_root_page_offset(char* table_name, struct table_page* table) {
	int i = 0;
	for (; i < 10; i++)
	{
		if (!strcmp(table_name, table->table_name[i]))
		{
			return table->root_page_offset[i];
		}
	}
	return -1;
}

void join() {

}

//// generic code to be implemented here.
struct student* get_student(int id, FILE** data_file, int ofst) {
	struct index_page* index_page;
	int i = 0;
	for (; i < LEVEL_OF_TREE; i++)
	{
		index_page = malloc(SIZE_OF_INDEX_PAGE);
		fseek(*data_file, ofst, SEEK_SET);
		fread(index_page, SIZE_OF_INDEX_PAGE, 1, *data_file);

		int j = 0;
		for (; j < 15; j++)
		{
			if (id < index_page->key[j])
			{
				ofst = index_page->child_page_id[j];
				free(index_page);
				index_page = NULL;
				goto out_start;
			}
		}
		if (id > index_page->key[j])
		{
			ofst = index_page->child_page_id[j];
			free(index_page);
			index_page = NULL;
			break;
		}
	out_start:;
	}

	struct data_page_student* data_page_student = malloc(SIZE_OF_STUDENT_DATA_PAGE);
	fseek(*data_file, ofst, SEEK_SET);
	fread(data_page_student, SIZE_OF_STUDENT_DATA_PAGE, 1, *data_file);

	i = 0;
	for (; i < RECORDS_PER_DATA_PAGE_STUDENT; i++)
	{
		if (id == data_page_student->data[i].id)
		{
			return &data_page_student->data[i];
		}
	}
	free(data_page_student);
	free(index_page);
	return NULL;
}

struct marks* get_marks(int id, FILE** data_file, int ofst) {
	struct index_page* index_page;
	int i = 0;
	for (; i < LEVEL_OF_TREE; i++)
	{
		index_page = malloc(SIZE_OF_INDEX_PAGE);
		fseek(*data_file, ofst, SEEK_SET);
		fread(index_page, SIZE_OF_INDEX_PAGE, 1, *data_file);

		int j = 0;
		for (; j < 15; j++)
		{
			if (id < index_page->key[j])
			{
				ofst = index_page->child_page_id[j];
				free(index_page);
				index_page = NULL;
				goto out_start;
			}
		}
		if (id > index_page->key[j])
		{
			ofst = index_page->child_page_id[j];
			free(index_page);
			index_page = NULL;
			break;
		}
	out_start:;
	}

	struct data_page_marks* data_page_marks = malloc(SIZE_OF_MARKS_DATA_PAGE);
	fseek(*data_file, ofst, SEEK_SET);
	fread(data_page_marks, SIZE_OF_MARKS_DATA_PAGE, 1, *data_file);

	i = 0;
	for (; i < RECORDS_PER_DATA_PAGE_MARKS; i++)
	{
		if (id == data_page_marks->data[i].s_id)
		{
			return &data_page_marks->data[i];
		}
	}

	free(data_page_marks);
	free(index_page);
	return NULL;
}

void get_student_range_name(int s_id, int e_id, char *name, FILE** data_file) {
	struct index_page* index_page;

	int seek_offset_start = offset;
	int i = 0;
	for (; i < LEVEL_OF_TREE; i++)
	{
		index_page = malloc(SIZE_OF_INDEX_PAGE);
		fseek(*data_file, seek_offset_start, SEEK_SET);
		fread(index_page, SIZE_OF_INDEX_PAGE, 1, *data_file);

		int j = 0;
		for (; j < 15; j++)
		{
			if (s_id < index_page->key[j])
			{
				seek_offset_start = index_page->child_page_id[j];
				free(index_page);
				index_page = NULL;
				goto out_start;
			}
		}
		if (s_id > index_page->key[14])
		{
			seek_offset_start = index_page->child_page_id[14];
			free(index_page);
			index_page = NULL;
			break;
		}
	out_start:;
	}

	int seek_offset_end = offset;
	i = 0;
	for (; i < LEVEL_OF_TREE; i++)
	{
		index_page = malloc(SIZE_OF_INDEX_PAGE);
		fseek(*data_file, seek_offset_end, SEEK_SET);
		fread(index_page, SIZE_OF_INDEX_PAGE, 1, *data_file);

		int j = 0;
		for (; j < 15; j++)
		{
			if (e_id < index_page->key[j])
			{
				seek_offset_end = index_page->child_page_id[j];
				free(index_page);
				index_page = NULL;
				goto out_end;
			}
		}
		if (e_id > index_page->key[14])
		{
			seek_offset_end = index_page->child_page_id[14];
			free(index_page);
			index_page = NULL;
			break;
		}
	out_end:;
	}

	struct data_page_student* data_page_student_start = NULL;
	while (seek_offset_start < seek_offset_end) {
		data_page_student_start = malloc(SIZE_OF_STUDENT_DATA_PAGE);
		fseek(*data_file, seek_offset_start, SEEK_SET);
		fread(data_page_student_start, SIZE_OF_STUDENT_DATA_PAGE, 1, *data_file);

		i = 0;
		for (; i < RECORDS_PER_DATA_PAGE_STUDENT; i++)
		{
			if (s_id <= data_page_student_start->data[i].id && strstr(data_page_student_start->data[i].name, name) > 0)
			{
				printf("%d, %s\n", data_page_student_start->data[i].id, data_page_student_start->data[i].name);
			}
		}

		seek_offset_start += 128;
	}

	struct data_page_student* data_page_student_end = malloc(SIZE_OF_STUDENT_DATA_PAGE);
	fseek(*data_file, seek_offset_end, SEEK_SET);
	fread(data_page_student_end, SIZE_OF_STUDENT_DATA_PAGE, 1, *data_file);

	i = 0;
	for (; i < RECORDS_PER_DATA_PAGE_STUDENT; i++)
	{
		if (e_id >= data_page_student_end->data[i].id && strstr(data_page_student_end->data[i].name, name) > 0)
		{
			printf("%d, %s\n", data_page_student_end->data[i].id, data_page_student_end->data[i].name);
		}
	}

	free(data_page_student_start);
	free(data_page_student_end);
	free(index_page);
}
