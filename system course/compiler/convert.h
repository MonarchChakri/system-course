#pragma once
#include "structs.h"
#include "utility.h"
#include <ctype.h>

int opcode_in_opcodes(char** token) {
	int i = 0;
	int reg, var, lab, pc, mov_type = 0, reg_type = 0, var_type = 0;
	for (; i < 15; i++)
	{
		if (!(*token))
		{
			return -1;
		}
		if (!strcmp(opcodes[i], *token))
		{
			intermediate_table[nooc][0] = i + 1;
			*token = strtok(NULL, " ,=");
			switch (i + 1)
			{
			case 1:
				reg = reg_in_registers(*token);
				if (reg == -1)
				{
					// mem
					int size = 0;
					if (token[0][strlen(*token) - 1] == ']')
					{
						reg = *token[0];
						size = token[0][2] - '0';
					}
					reg = symbol_in_symboltable(*token);
					reg += size;
					reg_type = 0;
				}
				else
				{
					// reg
					reg_type = 1;
				}
				*token = strtok(NULL, " ,=");

				var = reg_in_registers(*token);

				if (var == -1)
				{
					//mem
					int size = 0;
					if (token[0][strlen(*token) - 1] == ']')
					{
						var = *token[0];
						// size = atoi(&(token[0][1]));
						size = token[0][2] - '0';
					}
					var = symbol_in_symboltable(*token);
					var += size;
					var_type = 1;
				}
				else
				{
					// reg
					var_type = 0;
				}

				mov_type = reg_type * 2 + var_type;

				intermediate_table[nooc][0] = mov_type;
				intermediate_table[nooc][1] = reg;
				intermediate_table[nooc][2] = var;

				nooc++;
				goto out;
				break;
			case 2:
				// add
				intermediate_table[nooc][0] = 4;

				reg = reg_in_registers(*token);
				intermediate_table[nooc][1] = reg;

				*token = strtok(NULL, " ,=");
				reg = reg_in_registers(*token);
				intermediate_table[nooc][2] = reg;

				*token = strtok(NULL, " ,=");
				reg = reg_in_registers(*token);
				intermediate_table[nooc][3] = reg;
				nooc++;
				goto out;
				break;
			case 3:
				// sub
				reg = reg_in_registers(*token);
				intermediate_table[nooc][1] = reg;

				*token = strtok(NULL, " ,=");
				reg = reg_in_registers(*token);
				intermediate_table[nooc][2] = reg;

				*token = strtok(NULL, " ,=");
				reg = reg_in_registers(*token);
				intermediate_table[nooc][3] = reg;
				intermediate_table[nooc][0] = 5;
				nooc++;
				goto out;
				break;
			case 4:
				// mul
				intermediate_table[nooc][0] = 6;
				reg = reg_in_registers(*token);
				intermediate_table[nooc][1] = reg;

				*token = strtok(NULL, " ,=");
				reg = reg_in_registers(*token);
				intermediate_table[nooc][2] = reg;

				*token = strtok(NULL, " ,=");
				reg = reg_in_registers(*token);
				intermediate_table[nooc][3] = reg;
				nooc++;
				goto out;
				break;
			case 5:
				// jmp
				lab = lab_in_labels(*token);
				intermediate_table[nooc][1] = lab;
				intermediate_table[nooc][0] = 7;
				nooc++;
				goto out;
				break;
			case 6:
				// if
				//if ax gt bx then i++ else goto  -1
				push(nooc);
				intermediate_table[nooc][1] = reg_in_registers(*token);
				*token = strtok(NULL, " ,=");
				intermediate_table[nooc][0] = 8;
				intermediate_table[nooc][2] = opcode_in_opcodes(token);
				intermediate_table[nooc][4] = -1;
				nooc++;
				goto out;
				break;
			case 7:
				// eq
				intermediate_table[nooc][0] = 6;
				intermediate_table[nooc][3] = reg_in_registers(*token);
				return 7;
				break;
			case 8:
				// lt
				intermediate_table[nooc][0] = 6;
				intermediate_table[nooc][3] = reg_in_registers(*token);
				return 8;
				break;
			case 9:
				// gt
				intermediate_table[nooc][0] = 6;
				intermediate_table[nooc][3] = reg_in_registers(*token);
				return 9;
				break;
			case 10:
				// lteq
				intermediate_table[nooc][0] = 6;
				intermediate_table[nooc][3] = reg_in_registers(*token);
				return 10;
				break;
			case 11:
				// gteq
				intermediate_table[nooc][0] = 6;
				intermediate_table[nooc][3] = reg_in_registers(*token);
				return 11;
				break;
			case 12:
				// print
				reg = reg_in_registers(*token);
				intermediate_table[nooc][1] = reg;
				intermediate_table[nooc][0] = 14;
				nooc++;
				goto out;
				break;
			case 13:
				// read
				reg = reg_in_registers(*token);
				intermediate_table[nooc][1] = reg;
				intermediate_table[nooc][0] = 15;
				nooc++;
				goto out;
				break;
			case 14:
				// else
				pc = pop();
				push(nooc);
				intermediate_table[pc][4] = nooc + 1;
				intermediate_table[nooc][0] = 5;
				intermediate_table[nooc][1] = -1;
				nooc++;
				goto out;
				break;
			case 15:
				// end if
				pc = pop();
				intermediate_table[pc][1] = nooc + 1;
				break;
			default:
				break;
			}
		}
	}
out:;
	*token = strtok(NULL, " ,=");
	return 1;
}

void convert() {
	FILE* input_file = fopen("a.txt", "r");
	if (input_file == NULL)
	{
		perror("IO Error");
		system("pause");
		exit(1);
	}

	char* line = getline(input_file, '\n');
	int i = 0;
	int noi = 0;
	while (line[0] != '\0')
	{
		strcpy(instrs[i], line);
		free(line);
		i++;
		noi++;
		line = getline(input_file, '\n');
	}

	i = 0;
	for (; i < noi; i++)
	{
		char* line = (char*)malloc(sizeof(char) * (strlen(instrs[i]) + 5));
		strcpy(line, instrs[i]);
		char* token = strtok(line, " ,=\t");
		while (token != NULL)
		{
			if (!strcmp(token, "data"))
			{
				token = strtok(NULL, " ,=");
				struct symbol* temp = (struct symbol*)malloc(sizeof(struct symbol));
				temp->name = token[0];
				temp->offset = offset;
				if (token[1] != '[')
					temp->size = 1;
				else
					temp->size = atoi(&token[2]);
				offset += temp->size;
				symbol_table[nos] = temp;
				nos++;
				token = strtok(NULL, " ,=");
			}
			else if (!strcmp(token, "const"))
			{
				token = strtok(NULL, " ,=");
				if (token[1] != '[')
				{
					struct cnst* temp = (struct cnst*)malloc(sizeof(struct cnst));
					temp->sym.name = token[0];
					temp->sym.offset = offset;
					temp->sym.size = 1;
					token = strtok(NULL, " ,=");
					temp->val = atoi(token);
					offset += 1;
					cnst_table[noc] = temp;
					noc++;
				}
			}
			else if (token[strlen(token) - 1] == ':')
			{
				struct label* temp = (struct label*)malloc(sizeof(struct label*));
				temp->pc = i + 1;
				temp->title = (char*)malloc(sizeof(char) * (strlen(token) + 1));
				strcpy(temp->title, token);
				label_table[nol] = temp;
				nol++;
				token = strtok(NULL, " ,=");
			}
			else
			{
				opcode_in_opcodes(&token);
			}
		}
		free(line);
	}
}
