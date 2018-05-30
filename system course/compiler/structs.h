#pragma once
#include <stdio.h> 
struct symbol
{
	char name;
	int size;
	int offset;
}** symbol_table;

struct cnst
{
	struct symbol sym;
	int val;
}** cnst_table;

int** intermediate_table;

char** instrs;

struct label {
	int pc;
	char* title;
}** label_table;

char* registers[] = { "ax", "bx", "cx", "dx", "ex" , "fx" , "gx" , "hx" };

char* opcodes[] = { "mov", "add", "sub", "mul", "jmp",

"if",

"eq", "lt", "gt", "lteq", "gteq",

"print", "read",

"else", "endif" };

int nos = 0;
int noc = 0;
int nol = 0;
int nooc = 0;
int offset = 0;
