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

const static ushort_t NAR_SIZE_INIT = 100;
const static ushort_t NAR_SIZE_STEP = 100;
void input_chg(input_t *i, ushort_t ch) {
   av_set(i->a - i->idx, ch, i->r, i->c);
   i->ct = i->ct + 1;
}

ushort_t input_avail(input_t *i, ushort_t** a_ch) {
   assert(a_ch);
   ushort_t sz = * i->sudoku_sz;
   ushort_t avail = ft_list_set_ct(&(i->a->av), sz);
   if (avail) {
	ushort_t av_ct;
	av_ct = ft_list_set(&(i->a->av), a_ch, sz);
	assert(av_ct == avail);
   }
   return avail;
}

void input_free(input_t* ir) {
   ushort_t ct = *(ir->sudoku_sz);
   ct *= ct;
   ushort_t c = ct;
   /*
   while(ct--) {
    input_t* i = ir + ct;
    free(i->nar);
   }*/
   memset(ir, 0, sizeof(input_t) * c);
   free(ir);
}

ushort_t* input_copy(input_t* s){
	ushort_t ct = *s->sudoku_sz;
	ct*=ct;
	ushort_t* cpy = malloc(sizeof(ushort_t) * ct);
	while(ct--)
		*(cpy + ct) = (s + ct)->a->cv;
	return cpy;
}

input_t* input_new(sudoku_t* s) {
	assert(s);
	ushort_t ct = s->sz;
	ct *= ct;
	long i_mems = sizeof(input_t) * ct;
	input_t *i = malloc( i_mems );
	memset(i,0,i_mems);
	ushort_t idx = ct;
	while(idx--) {
 	 input_t * in = i+idx;
	 in->a = s->ar + idx;
	 in->idx = idx;
	 in->r = idx / s->sz;
	 in->c = idx % s->sz;
	 assert(_idx(in->r, in->c) == in->idx);
	 /*in->sz = NAR_SIZE_INIT;*/
	 in->ct = 0;
	 in->sudoku_sz = &(s->sz);
	 /*
	  long nar_ms = sizeof(ushort_t) * in->sz;
	  in->nar = malloc(nar_ms);
	  memset(in->nar, 0, nar_ms);
	 */
	}
	return i;
}

void inputs(ushort_t* vs, input_t* s) {
	ushort_t ct = *s->sudoku_sz;
	ct *= ct;
	while(ct--) {
	  input_t* i = s + ct;
	  ushort_t v = *(vs + ct);
	  input_chg(i, v);
	}
}

ushort_t input(input_t* i) {
   if (!i->a->cv) {
	   ushort_t* avar;
	   ushort_t avs = input_avail(i, &avar);
	   if (avs) {
	     ushort_t ch = ichg(i, avs, avar);
	     if (ch) {
		input_chg(i, ch);
		return 1;
	     }
	   }
   }
   return 0;
}

