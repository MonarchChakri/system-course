#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include "structs.h"
#include "file_api.h"

int main() {

	/*
	FILE* data_file = fopen("mc_studs.csv", "rb");
	FILE* output_file = fopen("result.bin", "wb");

	// convert students.csv file into result.bin file page wise.

	char header[1024];
	fgets(header, 1024, data_file);

	int i = 0;
	void* dp = create_data_page_from_file(data_file, 0);
	while (dp && i < NUMBER_OF_RECORDS)
	{
		fwrite(dp, SIZE_OF_STUDENT_DATA_PAGE, 1, output_file);
		free(dp);
		dp = NULL;
		dp = create_data_page_from_file(data_file, 0);
		i += RECORDS_PER_DATA_PAGE_STUDENT;
	}

	fclose(data_file);
	fclose(output_file);
	*/

	/*

	// load records into memory and print them.

	FILE* output_file = fopen("result.bin", "rb");
	int i = 0;
	struct data_page_student** dp = malloc(sizeof(struct data_page_student*) * (NUMBER_OF_RECORDS / 3));

	for (; i < NUMBER_OF_RECORDS / 3; i++)
	{
		dp[i] = malloc(SIZE_OF_data_page_student);
		fread(dp[i], SIZE_OF_data_page_student, 1, output_file);
	}

	i = 0;
	for (; i < NUMBER_OF_RECORDS / 3; i++)
	{
		int j = 0;
		for (; j < 3; j++)
		{
			printf("%d, %s\n", dp[i]->data[j].id, dp[i]->data[j].name);
		}
	}

	free(dp);
	dp = NULL;

	fclose(output_file);
	*/

	/*
	num_records_read = 1;
	output_file = fopen("result.bin", "rb");
	create_index_pages_from_data_page(&output_file, 3);
	fclose(output_file);
	*/

	/*
	data_file = fopen("mc_marks.csv", "rb");
	output_file = fopen("result.bin", "ab");

	// convert marks.csv file into result.bin file page wise.

	fgets(header, 1024, data_file);

	i = 0;
	dp = create_data_page_from_file(data_file, 1);
	while (dp && i < NUMBER_OF_RECORDS)
	{
		fwrite(dp, SIZE_OF_MARKS_DATA_PAGE, 1, output_file);
		free(dp);
		dp = NULL;
		dp = create_data_page_from_file(data_file, 1);
		i += RECORDS_PER_DATA_PAGE_MARKS;
	}

	fclose(data_file);
	fclose(output_file);
	*/

	/*

	// load records into memory and print them.

	FILE* output_file = fopen("result.bin", "rb");
	int i = 0;
	struct data_page_student** dp = malloc(sizeof(struct data_page_student*) * (NUMBER_OF_RECORDS / 3));

	for (; i < NUMBER_OF_RECORDS / 3; i++)
	{
	dp[i] = malloc(SIZE_OF_data_page_student);
	fread(dp[i], SIZE_OF_data_page_student, 1, output_file);
	}

	i = 0;
	for (; i < NUMBER_OF_RECORDS / 3; i++)
	{
	int j = 0;
	for (; j < 3; j++)
	{
	printf("%d, %s\n", dp[i]->data[j].id, dp[i]->data[j].name);
	}
	}

	free(dp);
	dp = NULL;

	fclose(output_file);
	*/

	/*
	num_records_read = 1;
	output_file = fopen("result.bin", "rb");
	create_index_pages_from_data_page(&output_file, 6);
	fclose(output_file);
	*/

	/*
	output_file = fopen("result.bin", "ab");
	create_table_page_form_offsets(&output_file);
	fclose(output_file);
	*/

	/*
	output_file = fopen("result.bin", "rb");
	create_index_pages_from_index_page(&output_file);
	fclose(output_file);
	*/

	/*
	struct index_page* ip = malloc(sizeof(struct index_page) * data_page_studentS_PER_INDEX_PAGE);
	output_file = fopen("result.bin", "rb");
	fseek(output_file, NUMBER_OF_RECORDS / 3 * sizeof(struct data_page_student), SEEK_CUR);
	fread(ip, SIZE_OF_INDEX_PAGE, 1, output_file);
	fclose(output_file);
	*/

	/*
	FILE* output_file = fopen("result.bin", "rb");
	int id = 0;
	printf("Enter the id: ");
	scanf("%d", &id);
	struct student* s = get_student(id, &output_file);
	printf("%d, %s\n", s->id, s->name);
	fclose(output_file);

	output_file = fopen("result.bin", "rb");
	int s_id = 0, e_id = 0;
	printf("Enter start id: ");
	scanf("%d", &s_id);
	printf("Enter end id: ");
	scanf("%d", &e_id);
	get_student_range(s_id, e_id, &output_file);
	fclose(output_file);
	*/

	FILE* output_file = fopen("result.bin", "rb");
	struct table_page* table = load_table_page_form_file(&output_file);
	fclose(output_file);

	/*
	output_file = fopen("result.bin", "rb");
	int s_id = 0;
	int e_id = 0;
	printf("Enter start id: ");
	scanf("%d", &s_id);
	printf("Enter end id: ");
	scanf("%d", &e_id);
	get_student_range(s_id, e_id, &output_file, tablepage->root_page_offset[0]);
	fclose(output_file);

	output_file = fopen("result.bin", "rb");
	s_id = 0;
	e_id = 0;
	printf("Enter start id: ");
	scanf("%d", &s_id);
	printf("Enter end id: ");
	scanf("%d", &e_id);
	get_marks_range(s_id, e_id, &output_file, tablepage->root_page_offset[1]);
	fclose(output_file);
	*/

	output_file = fopen("result.bin", "rb");
	// join("studs", "marks", table, &output_file);
	fclose(output_file);

	output_file = fopen("result.bin", "rb");
	int i = 1;
	for (; i < 49; i++)
	{
		struct student* s = get_student(i, &output_file, 2048);
		struct marks* m = get_marks(i, &output_file, 3200);
		printf("%d. %s %d\n", s->id, s->name, m->marks[0] + m->marks[1] + m->marks[2] + m->marks[3]);
		// free(s);
		// free(m);
	}
	fclose(output_file);

	getchar();
	return 0;
}
