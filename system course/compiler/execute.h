#pragma once
#include "structs.h"
#include "utility.h"

char mem[1024];
char reg[8];

void execute() {
	int i = 0;
	int re, var, re2, op;
	for (; i < nooc; i++)
	{
		switch (intermediate_table[i][0])
		{
		case 0:
			// reg to mem
			re = intermediate_table[i][1];
			var = intermediate_table[i][2];
			mem[re] = reg[var];
			break;
		case 1:
			// mem to mem
			re = intermediate_table[i][1];
			var = intermediate_table[i][2];
			mem[re] = mem[var];
			break;
		case 2:
			// reg to reg
			re = intermediate_table[i][1];
			var = intermediate_table[i][2];
			reg[re] = reg[var];
			break;
		case 3:
			// mem to reg
			re = intermediate_table[i][1];
			var = intermediate_table[i][2];
			reg[re] = mem[var];
			break;
		case 4:
			// add
			reg[intermediate_table[i][1]] = reg[intermediate_table[i][2]] + reg[intermediate_table[i][3]];
			break;
		case 5:
			// sub
			reg[intermediate_table[i][1]] = reg[intermediate_table[i][2]] - reg[intermediate_table[i][3]];
			break;
		case 6:
			// mul
			reg[intermediate_table[i][1]] = reg[intermediate_table[i][2]] * reg[intermediate_table[i][3]];
			break;
		case 7:
			// jmp
			i = intermediate_table[i][1];
			break;
		case 8:
			// if
			//if ax gt bx then i++ else goto  -1
			re = intermediate_table[i][1];
			op = intermediate_table[i][2];
			re2 = intermediate_table[i][3];
			switch (op)
			{
			case 9:
				if (reg[re] != reg[re2]) {
					i = intermediate_table[i][4];
					i--;
				}
				break;
			case 10:
				if (reg[re] >= reg[re2])
				{
					i = intermediate_table[i][4];
					i--;
				}
				break;
			case 11:
				if (reg[re] <= reg[re2])
				{
					i = intermediate_table[i][4];
					i--;
				}
				break;
			case 12:
				if (reg[re] > reg[re2])
				{
					i = intermediate_table[i][4];
					i--;
				}
				break;
			case 13:
				if (reg[re] <= reg[re2])
				{
					i = intermediate_table[i][4];
					i--;
				}
				break;
			default:
				break;
			}
			goto out;
			break;
		case 14:
			// print
			printf("%d\n", reg[intermediate_table[i][1]]);
			break;
		case 15:
			// read
			printf("Enter %s: ", registers[intermediate_table[i][1]]);
			scanf("%d", &reg[intermediate_table[i][1]]);
			break;
		default:
			break;
		}
	out:;
	}
}
