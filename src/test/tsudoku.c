#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sudoku3/av.h>
#include <sudoku3/settings.h>
#include <sudoku3/ftype.h>
#include <sudoku3/sudoku.h>
void test_sudoku_is_correct();

void test_sudoku_size_chk() {
  assert(sudoku_size_chk(4));
  assert(sudoku_size_chk(9));
  assert(!sudoku_size_chk(0));
  assert(!sudoku_size_chk(1));
  assert(!sudoku_size_chk(2));
  assert(!sudoku_size_chk(3));
  assert(!sudoku_size_chk(5));
}
void test_sudoku_size() {
	sudoku_size(4);
	assert(settings_get()->sz == 4);
}

void test_sudoku_new() {
	sudoku_size(4);
	sudoku_t* s = sudoku_new();
	assert(s);
	sudoku_t* q = sudoku_new();
	assert(q);
	assert(s!=q);
	assert(s->seq);
	ushort_t i = s->sz;
	while(i--)
		assert(*(s->seq + i) == (i+1));

	assert(s->v1);	assert(s->v2);	assert(s->v3);
	assert(s->v1 != s->v2);
	assert(s->v1 != s->v3);
	assert(s->v3 != s->v2);
	assert(s->v1 != q->v1);
	assert(s->sz == 4);
	assert(s->sz_sqrt == 2);
	assert(s->ar); assert(q->ar != s->ar);
	assert(s->ct == 16);
	assert(s->ar_memsize == 16 * sizeof(av_t));
	assert(s->v_memsize == 4 * sizeof(f_t));

	char *buf = s->v1;
	long ct = s->v_memsize;
	while(ct--) assert(buf[ct] == 0);

	buf = s->v2;
	ct = s->v_memsize;
	while(ct--) assert(buf[ct] == 0);

	buf = s->v3;
	ct = s->v_memsize;
	while(ct--) assert(buf[ct] == 0);
	sudoku_free(s);
	sudoku_free(q);
}

void test_sudoku_free() {
	sudoku_size(4);

	sudoku_t* s = sudoku_new();
	sudoku_free(s);
	assert(s->ct == 0);
	assert(s->sz == 0);
	assert(s->v_memsize == 0);
	assert(s->ar_memsize == 0);
}


void test_sudoku_is_complete() {
	sudoku_size(4);
	sudoku_t *s = sudoku_new();
	assert(sudoku_is_complete(s) == 0);
	ushort_t ct = s->ct;
	ushort_t fct = 0;
	while(ct--)
  	 (s->ar + ct)->cv = 1;
	assert(sudoku_is_complete(s));
	sudoku_free(s);
}

static ushort_t const _solved[] = {
1,2,4,3,
4,3,1,2,
3,4,2,1,
2,1,3,4
};
static ushort_t const _A[] = {
0,2,4,3,
4,3,1,2,
3,4,2,1,
2,1,3,4
};
static ushort_t const _B[] = {
2,2,4,3,
4,3,1,2,
3,4,2,1,
2,1,3,4
};
static ushort_t const _C[] = {
 9,  6,  7,  0,  0,  2,  3,  0,  1,
 2,  0,  0,  9,  7,  5,  8,  6,  4,
 3,  1,  0,  8,  6,  4,  9,  7,  5,
 6,  9,  8,  0,  0,  1,  0,  3,  2,
 0,  0,  1,  6,  9,  8,  5,  4,  7,
 0,  3,  2,  5,  4,  7,  6,  9,  8,
 7,  8,  9,  0,  0,  0,  1,  2,  3,
 0,  0,  0,  7,  8,  9,  4,  5,  6,
 1,  2,  3,  4,  5,  6,  7,  8,  9
};

void test_sudoku_has_avail() {
	sudoku_size(4);
	sudoku_t *s = sudoku_new();
	ushort_t ct = s->ct;
	assert(sudoku_has_avail(s));
	while(ct--)
	  av_seti(s->ar, _solved[ct], ct);
	assert(!sudoku_has_avail(s));
	sudoku_free(s);
	s = sudoku_new();
	ct = s->ct;
	while(ct--)
	  av_seti(s->ar, _A[ct], ct);
	assert(sudoku_has_avail(s));
	sudoku_free(s);
	s = sudoku_new();
	ct = s->ct;
	while(ct--)
	  av_seti(s->ar, _B[ct], ct);
	assert(!sudoku_has_avail(s));

	sudoku_free(s);
	sudoku_size(9);
	s = sudoku_new();
	ct = s->ct;
	while(ct--)
	  av_seti(s->ar, _C[ct], ct);
	assert(!sudoku_has_avail(s));
}

void test_sudoku_is_solution() {
	sudoku_size(4);
	sudoku_t *s = sudoku_new();
	ushort_t ct = s->ct;
	assert(sudoku_is_solution(s) == 0);
	while(ct--)
	  av_seti(s->ar, _solved[ct], ct);
	assert(sudoku_is_solution(s));

	sudoku_free(s);
	s = sudoku_new();
	ct = s->ct;
	while(ct--)
	  av_seti(s->ar, _A[ct], ct);
	assert(sudoku_is_solution(s)==0);

	av_seti(s->ar, 1, 0);
	assert(sudoku_is_solution(s));
	assert(sudoku_is_complete(s));
	assert(sudoku_is_correct(s));

	sudoku_free(s);
	s = sudoku_new();
	ct = s->ct;
	while(ct--)
	  av_seti(s->ar, _B[ct], ct);
	assert(sudoku_is_solution(s)==0);
}

void test_sudoku_is_correct() {
	sudoku_size(4);
	sudoku_t *s = sudoku_new();
	assert(sudoku_is_correct(s));

	ushort_t ct = s->ct;
	while(ct--)
	  av_seti(s->ar, _A[ct], ct);
	assert(sudoku_is_correct(s));
	sudoku_free(s);
	s = sudoku_new();
	ct = s->ct;
	while(ct--)
	  av_seti(s->ar, _B[ct], ct);
	assert(0 == sudoku_is_correct(s));
	sudoku_free(s);

	s = sudoku_new();
	ct = s->ct;
	while(ct--)
	  av_seti(s->ar, _solved[ct], ct);
	assert(sudoku_is_correct(s));
	sudoku_free(s);
	s = sudoku_new();
	ct = s->ct;
	while(ct--)
	  av_seti(s->ar, 1, ct);
	assert(0 == sudoku_is_correct(s));
}

void test_sudoku_clear() {
	sudoku_size(4);
	sudoku_t *s = sudoku_new();
	ushort_t ct = s->ct;
	while(ct--)
	  av_seti(s->ar, _solved[ct], ct);
	assert(sudoku_is_solution(s));
	assert(sudoku_is_complete(s));
	assert(sudoku_is_correct(s));
	assert(!sudoku_has_avail(s));

	sudoku_clear(s);
	assert(!sudoku_is_solution(s));
	assert(!sudoku_is_complete(s));
	assert(sudoku_is_correct(s));
	assert(sudoku_has_avail(s));
}

int main() {
  test_sudoku_size_chk();
  test_sudoku_size();
  test_sudoku_new();
  test_sudoku_free();
  test_sudoku_is_correct();
  test_sudoku_is_complete();
  test_sudoku_is_solution();
  test_sudoku_has_avail();
  test_sudoku_clear();
}

