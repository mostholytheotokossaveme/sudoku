#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <alloca.h>
#include <math.h>
#include <sudoku3/ftype.h>
#include <sudoku3/av.h>
#include <sudoku3/settings.h>
#include <sudoku3/sudoku.h>
#include <sudoku3/solver.h>
#include <sudoku3/ichg.h>

ushort_t solve(solver_t* slvr) {
	ushort_t chgd = 0;
	assert(slvr);
	sudoku_t* s = slvr->sudoku;
	input_t* ir = slvr->input;
	assert(s);
	assert(ir);
	assert(s->ar);

	/*
	int sidx;

	while((sidx = av_sing_idx(s->ar)) >= 0 && input(ir + (ushort_t)sidx))
	 chgd=1;
	*/

	ushort_t ct = s->ct;
	while(ct--) {
	  input_t* i = ir + ct;
	  if(input(i) && !chgd) chgd=1;
	  if (i->a->cv == 0)
		return chgd;
	}
	return chgd;
}

ushort_t* solve_input(sudoku_t* s, input_t* ir) {
	solver_t slv = {s,ir};
	long ctr = 0;
	long sltct = 0;
	ushort_t ta = 0;
	ushort_t *inp = input_copy(ir);
        do {
           sudoku_clear(s);
	   inputs(inp, ir);
	   solve(&slv);
	   if (!sudoku_is_complete(s) && !sudoku_is_solution(s)) {
	 	ichg_result_no();
		ta = 1;
	   } else ta = 0;
	   ctr++;
	} while (ta);
        free(inp);
	inp = NULL;

	if (sudoku_is_solution(s)) {
	   sltct ++;
           ichg_result_yes();
	   inp = input_copy(ir);
	} else {
	   ichg_result_no();
	}
	return inp;
}

