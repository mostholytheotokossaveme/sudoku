#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <sudoku3/ftype.h>
#include <sudoku3/av.h>
#include <sudoku3/settings.h>
#include <sudoku3/sudoku.h>
#include <sudoku3/solver.h>
#include <sudoku3/ichg.h>
#include <sudoku3/sudoku_gi.h>

static unsigned long _sgi = 0;
static unsigned long gi_num() {
	return ++_sgi;
}

gi_t sudoku_gi_new(enum SUDOKU_SIZE sz) {
   settings_t settings = { sz, (ushort_t)sqrt((int)sz) };
   settings_set(&settings);
   gi_t gi;

   input_ctx_t ictx;
   ichg_ctx_new(&ictx);
   ichg_ctx_set(&ictx);

   gi.num = gi_num();
   gi.sudoku = sudoku_new();
   gi.input = input_new(gi.sudoku);
   gi.ctx = ictx;
   gi.settings = malloc(sizeof(settings));
   *gi.settings = settings;

   return gi;
}

struct sudoku_gi sudoku_gi_from(enum SUDOKU_SIZE sz, gi_ar_t* initar) {
	gi_t sgi = sudoku_gi_new(sz);
	if(initar) {
	   assert(initar->num);
	   assert(initar->idx);
  	   ushort_t idx, n, i,ct;
	   av_t* ar = sgi.sudoku->ar;

	   ct = sgi.sudoku->ct;
	   for(idx = 0; idx < initar->num_ct; idx++) {
		n = *(initar->num + idx);
		i = *(initar->idx + idx);
		if (n > 0 && i >= 0 && i < ct)
		   av_seti(ar, n, i);
	   }
	}
	return sgi;
}

void sudoku_gi_free(gi_t gi) {
	if (gi.input) {
	  input_free(gi.input);
	  gi.input = 0;
	}

	if (gi.sudoku) {
	  sudoku_free(gi.sudoku);
	  gi.sudoku = 0;
	}

	ichg_ctx_free(&gi.ctx);

	if (gi.settings) {
	  free(gi.settings);
	  gi.settings = 0;
	}
}

ushort_t sudoku_gi_get(gi_t gi, ushort_t r, ushort_t c) {
	return sudoku_gi_geti(gi, r*gi.settings->sz + c);
}

ushort_t sudoku_gi_geti(gi_t gi, ushort_t idx) {
	av_t* av = gi.input->a + idx;
	return av->cv;
}

void sudoku_gi_set(gi_t gi, ushort_t r, ushort_t c, ushort_t v) {
	av_seti(gi.input->a, v, r*gi.settings->sz +  c);
}


void sudoku_gi_seti(gi_t gi, ushort_t idx, ushort_t v) {
	av_seti(gi.input->a, v, idx);
}

ushort_t*
		sudoku_gi_solve	(gi_t gi) {
	settings_set(gi.settings);
	return solve_input(gi.sudoku, gi.input);
}

gi_stat_t*
		sudoku_gi_stat		(gi_t gi) {
	ushort_t ct, cct, ict;
	gi_stat_t s;
	cct=ict=0;
	ct = gi.sudoku->sz; ct*=ct;
	while(ct--)
		if ((gi.sudoku->ar + ct)->cv)
			cct++;
		else	ict++;
	s.complete_ct = cct;
	s.incomplete_ct = ict;
	s.correct = sudoku_is_correct(gi.sudoku);
	s.solution = sudoku_is_solution(gi.sudoku);
	s.complete = sudoku_is_complete(gi.sudoku);
	gi_stat_t* stat = malloc(sizeof(s));
	*stat = s;
	return stat;
}

