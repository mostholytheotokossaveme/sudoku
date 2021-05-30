#ifndef SUDOKU_H
#define SUDOKU_H
#include <sudoku3/ftype.h>
#include <sudoku3/av.h>
#include <sudoku3/settings.h>

const static ushort_t SUDOKU_SIZE_MIN = 4;

/* 
  Type for sudoku struct.
  
*/
typedef struct {
 av_t *ar;
 f_t *v1, *v2, *v3;
 ushort_t *seq;

 ushort_t ct;
 ushort_t sz;
 ushort_t sz_sqrt;
 long ar_memsize;
 long v_memsize;
 long seq_memsize;
} sudoku_t;

ushort_t sudoku_size_chk(ushort_t sz);
void sudoku_size(ushort_t sz);
sudoku_t* sudoku_new();
void sudoku_free(sudoku_t* s);
void sudoku_clear(sudoku_t* s);
sudoku_t* sudoku_copy(sudoku_t* s);
void sudoku_copyav(sudoku_t* d, sudoku_t* s);

ushort_t sudoku_is_correct(sudoku_t* s);
ushort_t sudoku_is_complete(sudoku_t* s);
ushort_t sudoku_is_solution(sudoku_t* s);
ushort_t sudoku_has_avail(sudoku_t* s);
#endif
