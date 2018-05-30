#pragma once
struct column_node
{
	char* name;
	void* value;
	int version;
	struct column_node* next_column;
	struct column_node* next_version;
	//
	struct column_node* next_row;
	struct column_node* prev_row;
	//
};

struct row_node
{
	int commit;
	int current;
	struct column_node* row;
	struct row_node* next_row;
	int id;
};

struct table
{
	struct row_node* head_row;
	char name[20];
	char** column_names;
	int number_of_cols;
	int* types; // 0 int
				// 1 string
	int * sizes;
	struct table* next_table;
};
