#pragma once
#define NUMBER_OF_RECORDS 48
#define SIZE_OF_STUDENT_DATA_PAGE sizeof(struct data_page_student)
#define SIZE_OF_MARKS_DATA_PAGE sizeof(struct data_page_marks)
#define SIZE_OF_INDEX_PAGE sizeof(struct index_page)
#define SIZE_OF_TABLE_PAGE sizeof(struct table_page)
#define RECORDS_PER_DATA_PAGE_STUDENT 3
#define RECORDS_PER_DATA_PAGE_MARKS 6
#define DATA_PAGES_PER_INDEX_PAGE 16
#define INDEX_PAGES_PER_TABLE_PAGE 10
#define LEVEL_OF_TREE 1

struct student
{
	int id;
	char name[32];
};

struct marks
{
	int s_id;
	int marks[4];
};

struct data_page_student
{
	char page_type;// = '1';
	char table_id;// = '0';
	char unused[18];
	struct student data[3];
};

struct data_page_marks
{
	char page_type;// = '1';
	char table_id;// = '1';
	char unused[6];
	struct marks data[6];
};

struct index_page
{
	char page_type;// = '2';
	char table_id;// = '0' or '1';
	char unused[2];
	int child_page_id[16];
	int key[15];
};

struct table_page
{
	char page_type;// = '3';
	char table_id;// = '-1'; unused;
	char unused[6];
	int root_page_offset[10];
	char table_name[10][8];
};
