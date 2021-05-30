#ifndef SOLVER_INPUT_H
#define SOLVER_INPUT_H
#include <sudoku3/ftype.h>
#include <sudoku3/av.h>

struct solver_input {
   /*ushort_t* nar;*/
   av_t* a;
   /*ushort_t sz;*/
   unsigned long ct;
   ushort_t idx;
   ushort_t r;
   ushort_t c;
   ushort_t *sudoku_sz;
} solver_input;

typedef struct solver_input input_t;

input_t* 	input_new	(sudoku_t *sudoku);
void 		input_free	(input_t* i);
ushort_t*	input_copy	(input_t* i);
ushort_t 	input_avail	(input_t *i, ushort_t** a_ch);
void 		input_chg	(input_t *i, ushort_t ch);
ushort_t	input		(input_t* i);
void		inputs		(ushort_t* vs, input_t* i);
#endif
