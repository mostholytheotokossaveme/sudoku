#include <assert.h>
#include <search.h>
#include <stdlib.h>
#include <string.h>
#include <sudoku3/sudoku_gi.h>
#include "game.h"

static int cmpus(const void* a, const void *b) {
	if (a == b) return 0;
	if (a == 0 && b != 0) return -1;
	if (a != 0 && b == 0) return 1;

	ushort_t* pa;
	ushort_t* pb;
	*(void**)&pa = a;
	*(void**)&pb = b;

	ushort_t x = *pa;
	ushort_t y= *pb;
	if (x > y) return 1;
	if (x < y) return -1;
	if (x == y) return 0;
}

ushort_t* game_init_sol(enum SUDOKU_SIZE sz, size_t fillct, size_t visct, unsigned int rsd, gi_ar_t *piar) {
	if (fillct <= 0) {
		return NULL;
	}
	srand(rsd);
	assert(fillct <= (int)((sz*sz)/1.3));
	ushort_t* sltn = 0;
	unsigned int lmt = 5000000;
	gi_ar_t iar = {0,0,0};
	iar.num_ct = fillct;
	iar.num = calloc(fillct, sizeof(ushort_t));
	iar.idx = calloc(fillct, sizeof(ushort_t));
	ushort_t idx,tc;
	size_t i;
	tc = sz*sz;
	ushort_t *vidx = calloc(visct, sizeof(ushort_t));
	for (i=0;i<visct;i++){
		do idx = (ushort_t) (abs(rand()) % tc);
		while (lfind(&idx, vidx, &i, sizeof(ushort_t), &cmpus));
		vidx[i] = idx;
        }

	gi_t sudoku;
	while(!sltn && lmt--) {
		ushort_t n;

		for(i=0; i<fillct; i++) {
			do
			idx = (ushort_t) (abs(rand()) % tc);
			while (lfind(&idx, iar.idx, &i, sizeof(ushort_t), &cmpus));
			do
			n = (ushort_t) (abs(rand()) % sz) + 1;
			while(n == 0);
			iar.idx[i] = idx;
			iar.num[i] = n;
		}
		for (i = 0; i < fillct - 1; i++) {
			assert(iar.idx[i] != iar.idx[i+1]);
		}
		sudoku = sudoku_gi_from(sz, &iar);
		if (sudoku_is_correct(sudoku.sudoku))
			sltn = sudoku_gi_solve(sudoku);
	}

	      free(iar.num);
	      iar.num = 0;
	      free(iar.idx);
	      iar.idx =0;

	if (sltn) {
	  iar.num_ct = visct;
	  iar.idx = vidx;
	  iar.num = calloc(visct, sizeof(ushort_t));
	  for (i=0;i<visct;i++) {
		idx = iar.idx[i];
		iar.num[i] = (sudoku.sudoku->ar + idx)->cv;
	  }
	 *piar = iar;
	} else free(vidx);
	return sltn;
}
