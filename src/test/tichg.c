#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sudoku3/av.h>
#include <sudoku3/settings.h>
#include <sudoku3/ftype.h>
#include <sudoku3/sudoku.h>
#include <sudoku3/ichg.h>
#include <sudoku3/solver_input.h>

void test_ichg_result_no() {
}

void test_ichg_result_yes() {
}

void test_ichg() {
   settings_t t = {4,2};
   settings_set(&t);
   sudoku_t *s = sudoku_new();
   input_t *i = input_new(s);
   ushort_t c1, c2, c3;
   ushort_t *av_ar;
   ushort_t

   av_ct = input_avail(i, &av_ar);
   c1 = ichg(i, av_ct, av_ar);

   av_ct = input_avail(i, &av_ar);
   c2 = ichg(i+1, av_ct, av_ar);

   av_ct = input_avail(i, &av_ar);
   c3 = ichg(i+2, av_ct, av_ar);

   assert(c1 == c2);
   assert(c2 == c3);
   assert(c1 != 0);
   sudoku_clear(s);
   av_ct = input_avail(i, &av_ar);
   assert(c1 == ichg(i, av_ct, av_ar));
   ichg_result_no();
   sudoku_clear(s);
   av_ct = input_avail(i, &av_ar);
   assert(c1 != ichg(i, av_ct, av_ar));

   input_free(i);
   sudoku_free(s);
}

void test_ichg_child() {
}

int main() {
   test_ichg();
   test_ichg_child();
   test_ichg_result_no();
   test_ichg_result_yes();
}
/*
void		ichg_result_no	();

ushort_t 	ichg		(input_t *i, ushort_t av_ct, ushort_t *av_ar);

input_chg_t* 	ichg_child	(input_chg_t *parent,
			 	 av_t* a, ushort_t v,
				 ushort_t av_ct, ushort_t *av_ar);
*/
