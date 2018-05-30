#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include "stack.h"
#include "structs.h"
#include "convert.h"
#include "execute.h"
#include "utility.h"

int main() {
	symbol_table = (struct symbol**)malloc((sizeof(struct symbol*) * 100));
	cnst_table = (struct cnst**)malloc((sizeof(struct cnst*) * 100));
	label_table = (struct label**)malloc((sizeof(struct label*) * 100));
	intermediate_table = (int**)malloc((sizeof(int*) * 100));
	for (int i = 0; i < 100; i++)
		intermediate_table[i] = (int*)malloc((sizeof(int) * 100));

	instrs = (char**)malloc((sizeof(char*) * 100));
	int i = 0;
	for (; i < 100; i++)
		instrs[i] = (char*)malloc((sizeof(char) * 100));

	convert();
	execute();

	getchar();
	return 0;
}
