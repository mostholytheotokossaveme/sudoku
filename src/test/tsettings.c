#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sudoku3/av.h>
#include <sudoku3/settings.h>
#include <sudoku3/ftype.h>

int main() {
	settings_t i = { .sz = 4, .sz_sqrt = 2 };
	settings_set(&i);
	settings_t* s = settings_get();
	assert(s->sz == i.sz);
	assert(s->sz_sqrt == i.sz_sqrt);
	assert(s->sz == 4);
}
