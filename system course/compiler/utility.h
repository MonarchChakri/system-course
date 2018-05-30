#pragma once
#include "structs.h"

char* getline(FILE * fp, char delim)
{
	if (fp == NULL) {
		return "\0";
	}
	int linesize = 50;
	char * res = (char*)malloc(linesize * sizeof(char));
	char c;
	int i = 0;
	for (; 1; i++)
	{
		c = fgetc(fp);
		if (c == EOF || c == delim || c == '\0')
			break;
		if (i == linesize - 1)
		{
			linesize *= 2;

			char* temp = (char*)malloc(linesize * sizeof(char));
			strncpy(temp, res, linesize / 2);
			free(res);
			res = temp;
		}
		res[i] = c;
	}
	res[i] = '\0';
	return res;
}

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

int reg_in_registers(char* token) {
	if (token == NULL)
	{
		return -1;
	}
	int i = 0;
	for (; i < 8; i++)
	{
		if (!strcmp(registers[i], token))
		{
			return i;
		}
	}
	return -1;
}

int symbol_in_symboltable(char* token) {
	if (token == NULL)
	{
		return -1;
	}
	int i = 0;
	for (; i < nos; i++)
	{
		if (symbol_table[i]->name == token[0])
		{
			return i;
		}
	}
	return -1;
}

int lab_in_labels(char* token) {
	if (token == NULL)
	{
		return -1;
	}
	int i = 0;
	for (; i < nol; i++)
	{
		if (!strcmp(label_table[i]->title, token))
		{
			return label_table[i]->pc;
		}
	}
	return -1;
}
