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
static unsigned int FREQ = 100;

void test_input_new(sudoku_t* s1, input_t* ir1) {
   sudoku_t *s = sudoku_new();
   input_t *ir = input_new(s);
   assert(ir);
   ushort_t ct = s->ct;
   while(ct--) {
     input_t *i = ir + ct;
     /*assert(i->nar);*/
     assert(i->a);
     assert(i->idx == ct);
     assert(_idx(i->r,i->c) == i->idx);
     assert(i->ct == 0);
     /*assert(i->sz);*/
   }
   input_free(ir);
   sudoku_free(s);
}

void test_input_avail(sudoku_t* s, input_t* ir) {
 ushort_t ac;
 ushort_t *ach;
 ac = input_avail(ir + 0, &ach);
 assert(ac == s->sz);
 printf("iav:\t");
 while(ac--) printf("%d\t", ach[ac]);
 printf("\n");
 free(ach); ach=0;

 /*(ir+0)->nar[0] = 1;*/
 (ir+0)->ct = 1;
 ac = input_avail(ir + 0, &ach);
 /*assert(ac == s->sz - 1);*/
 printf("iav:\t");
 while(ac--) printf("%d\t", ach[ac]);
 printf("\n");
 free(ach); ach=0;

 (ir+1)->a->av = 7;
 ac = input_avail(ir + 1, &ach);
 assert(ac == 3);
 printf("iav:\t");
 while(ac--) printf("%d\t", ach[ac]);
 printf("\n");
}

void test_input(sudoku_t* s, input_t* ir) {
	av_t *a = ir->a;
	assert(0 == a->cv);
	input(ir);
	assert(a->cv);
}

void test_input_chg(sudoku_t* s, input_t* ir) {
	av_t *a = ir->a;
	assert(0 == a->cv);
	input_chg(ir, 5);
	assert(a->cv == 5);
}

static void _render(sudoku_t* sudoku) {
        ushort_t ct = sudoku->ct;
        ushort_t i,j,t;
        for(i=0,t=0;i<sudoku->sz;i++){
            for(j=0;j<sudoku->sz;j++,t++){
                printf("%2d ", (sudoku->ar + t)->cv);
            }
            printf("\n");
        }
}

void test_solve(sudoku_t* s, input_t* ir) {
	solver_t slv = {s,ir};
	long ctr = 0;
	long sltct = 0;
	TA:
        do {
	   sudoku_clear(s);
	   solve(&slv);
	   if (!sudoku_is_complete(s) || !sudoku_is_solution(s)) {
	 	 ichg_result_no();
	   }
	   ctr++;
	   if (0 == (ctr % FREQ)) {
	    printf("\t----- %ld  ----\n", ctr);
	    _render(s);
	   }
	} while (!(sudoku_is_complete(s) &&
		   sudoku_is_solution(s) ) );

	if (sudoku_is_solution(s)) {
	   sltct ++;
	   printf("ok\nSOLUTION: %ld\n", sltct);
	   _render(s);
           ichg_result_no();
	  goto TA;
	}
}

void t() {
 void (*test[])(sudoku_t*, input_t*) =
 { test_input_new, test_input_avail, test_input,
   test_input_chg, test_solve };
 int i = 5;
 while(i--) {
   sudoku_t *s = sudoku_new();
   input_t *ir = input_new(s);

   test[i](s, ir);

   input_free(ir);
   sudoku_free(s);
 }
}

void ts() {
   sudoku_t *s = sudoku_new();
   input_t *ir = input_new(s);
   input_ctx_t ictx;
   ichg_ctx_new(&ictx);
   ichg_ctx_set(&ictx);

   input_ctx_t q;
   ichg_ctx_get(&q);
   test_solve(s, ir);

   input_free(ir);
   sudoku_free(s);
   ichg_ctx_free(&ictx);
}

int main(int argc, char* argv[]) {
  if (argc > 1) {
	assert(sscanf(argv[1], "%d", &FREQ));
  }
  printf("render freq: %d\n", FREQ);
  /*
 settings_set(&s);
 t();*/
 settings_t s = { 4, 2 };
 s = (settings_t) { 9, 3 };
 settings_set(&s);
 ts();

 s = (settings_t) { 16, 4 };
 settings_set(&s);
 ts();

 s = (settings_t) { 25, 5 };
 settings_set(&s);
 ts();
}
