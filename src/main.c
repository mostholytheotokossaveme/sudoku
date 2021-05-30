#include <stdio.h>
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
#include <sudoku3/sudoku_gi.h>

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

static int _inp(sudoku_t *sudoku) {
	char* line = 0;
	long llen = 0;
	ushort_t r=0,c=0,n=0,idx=-1;
	printf("Set at [x,y=(1-%d)]: ", sudoku->sz);
	if(getline(&line, &llen, stdin)){
		if (sscanf(line, "%d,%d=%d", &r,&c,&n) == 3) {
		   printf("\t%d,%d=%d\n",r,c,n);
		   if (n == 123) return 1;
                   idx = sudoku->sz * r + c;
		   if (n != 0) {
			if((sudoku->ar + idx)->cv)
				av_unseti(sudoku->ar, idx);
			av_set(sudoku->ar,n,r,c);
		   }
		   else
			av_unseti(sudoku->ar, idx);
		   if (!sudoku_is_correct(sudoku))
			printf("\tchk (%d,%d)=%d\n",r,c,n);
		   if (sudoku_is_complete(sudoku))
			printf("\tcompleted with (%d,%d)=%d\n",r,c,n);
		   if (sudoku_is_solution(sudoku))
			printf("\tsolved with (%d,%d)=%d\n",r,c,n);
		} else {
		   printf("recv:\t%s", line);
		   printf("\texample: 0,0=4\n");
		}
		free(line); line = 0; llen = 0;
	}
	return 0;
}


int main() {
	char* line = 0;
	long llen = 0;
	ushort_t sz = 0;
	sudoku_t* sudoku;
	input_t* inp;
	ushort_t* sltn = 0;
	gi_t gi;
	while(sudoku_size_chk(sz) == 0) {
	 printf("Size [4,9]: ");
	 if(getline(&line, &llen, stdin) &&
	    sscanf(line, "%d", &sz) &&
	    sudoku_size_chk(sz) ) {
		 gi = sudoku_gi_new(sz);
		 printf("\t%d\n", sz);
		 free(line); line = 0; llen = 0;
		 ushort_t as = 0;
		 gi_stat_t* st;
		 while(!(st=sudoku_gi_stat(gi))->complete) {
			free(st); st=0;
			 printf("\n\t---\n");
			 if(as || _inp(gi.sudoku)) {
			   as = 1;
			 }
			 if (as) {
			    sltn = sudoku_gi_solve(gi);
			 }
		 }
		if(st) free(st); st=0;
         }
	}
	sudoku_gi_free(gi);
	if (sltn) {
	   ushort_t ct = sz*sz;
	   ushort_t idx = ct;
	   ushort_t r = sz;
  	   printf("\n");
	   while(r--) {
		ushort_t c = sz;
		while(c--)
			printf("%d  ", sltn[ct - idx--]);
		printf("\n");
	  }
	}
}
