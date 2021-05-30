#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sudoku3/av.h>
#include <sudoku3/settings.h>
#include <sudoku3/ftype.h>
#include <sudoku3/sudoku.h>
#include <sudoku3/solver.h>
#include <sudoku3/ichg.h>
#include <sudoku3/sudoku_gi.h>

static enum SUDOKU_SIZE size_four = 4;
static enum SUDOKU_SIZE size_nine = 9;

void test_gi_new() {
	gi_t gi = gi_new(size_four);
	assert(gi.settings->sz == 4);
	assert(gi.settings->sz_sqrt == 2);
	assert(gi.input);
	assert(* gi.input->sudoku_sz == 4);
	assert((gi.input + 1)->idx == 1);
	gi_free(gi);
}

void test_gi_free() {
	gi_t gi4 = gi_new(size_four);
	gi_t gi9 = gi_new(size_nine);
	gi_free(gi4);
	gi_free(gi9);
}

void test_gi_get() {
	gi_t gi = gi_new(size_four);
	(gi.input->a + 1)->cv = 2;
	assert(2 == gi_geti(gi, 1));
	assert(0 == gi_geti(gi, 0));
	gi_free(gi);

}

void test_gi_set() {
	gi_t gi = gi_new(size_four);
	gi_seti(gi, 2, 3);
	assert(3 == gi_geti(gi, 2));
	assert(0 == gi_geti(gi, 1));
	gi_free(gi);
}
void test_gi_stat() {
	gi_t gi = gi_new(size_nine);
	assert(gi_solve(gi));
	gi_stat_t *s = gi_stat(gi);
	assert(s->solution);
	assert(s->complete);
	assert(s->correct);
	assert(s->complete_ct == 81);
	assert(s->incomplete_ct == 0);

}

void test_gi_solve() {
	gi_t gi = gi_new(size_nine);
	gi_seti(gi, 2, 3);
	assert(3 == gi_geti(gi, 2));
	assert(0 == gi_geti(gi, 1));
	ushort_t * slt = gi_solve(gi);
	assert(slt);
	assert( *(slt + 2) == 3 );
	ushort_t ct = 81;
	while(ct--)
		assert(*(slt+ct));
	gi_free(gi);
}

int main() {
	test_gi_new();
	test_gi_free();
	test_gi_get();
	test_gi_set();
	test_gi_stat();
	test_gi_solve();
	return 0;
}
