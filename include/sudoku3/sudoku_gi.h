#ifndef SUDOKU_GI_H
#define SUDOKU_GI_H
#include <sudoku3/ftype.h>
#include <sudoku3/av.h>
#include <sudoku3/settings.h>
#include <sudoku3/sudoku.h>
#include <sudoku3/solver.h>
#include <sudoku3/ichg.h>
#include <sudoku3/sudoku_gi.h>

struct sudoku_gi {
	unsigned long num;
	settings_t* settings;
	sudoku_t* sudoku;
	input_t* input;
	input_ctx_t ctx;
};

struct gi_stat {
	ushort_t correct;
	ushort_t complete;
	ushort_t solution;
	ushort_t complete_ct;
	ushort_t incomplete_ct;
};

struct sudoku_gi_ar {
	ushort_t *num;
	ushort_t *idx;
	ushort_t num_ct;
};

typedef struct sudoku_gi gi_t;
typedef struct gi_stat gi_stat_t;
typedef struct sudoku_gi_ar gi_ar_t;

/*
 The number of squares per row and column.
 Total sq. count would be SUDOKU_SIZE * SUDOKU_SIZE.
 */
enum SUDOKU_SIZE {
 FOUR=4,
 NINE=9,
 SIXTEEN=16,
 TWENTYFIVE=25
};

struct sudoku_gi
		sudoku_gi_new		(enum SUDOKU_SIZE sz);
void
		sudoku_gi_free		(struct sudoku_gi gi);

struct sudoku_gi
		sudoku_gi_from		(enum SUDOKU_SIZE sz, gi_ar_t* ar);

ushort_t*
		sudoku_gi_solve	(struct sudoku_gi);

void
		sudoku_gi_set		(struct sudoku_gi, ushort_t r, ushort_t c, ushort_t v);
void
		sudoku_gi_seti		(struct sudoku_gi, ushort_t idx, ushort_t v);

ushort_t
		sudoku_gi_get		(struct sudoku_gi, ushort_t r, ushort_t c);
ushort_t
		sudoku_gi_geti		(struct sudoku_gi, ushort_t idx);

struct gi_stat*
		sudoku_gi_stat		(struct sudoku_gi);

#endif
