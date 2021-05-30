#ifndef ICHG_H
#define ICHG_H
#include <sudoku3/ftype.h>
#include <sudoku3/av.h>
#include <sudoku3/solver_input.h>

struct ichg {
  av_t *a;
  ushort_t v;
  ushort_t av_ct;
  ushort_t *av_ar;

  struct ichg* parent;
  struct ichg** children;
  ushort_t children_ct;
  ushort_t children_sz;
  ushort_t *result;
};

typedef struct ichg input_chg_t;

struct ichg_ctx {
	input_chg_t *pr;
	input_chg_t *cur;
	input_chg_t *last;
};

typedef struct ichg_ctx input_ctx_t;

static const int CHILDREN_INIT_SIZE = 9;
static const int CHILDREN_STEP_SIZE = 9;

void
	ichg_ctx_free   (input_ctx_t*);
void
	ichg_ctx_new    (input_ctx_t*);

void ichg_ctx_get(input_ctx_t*);
void ichg_ctx_set(input_ctx_t*);

void
	ichg_result_no	();
void
	ichg_result_yes	();

const ushort_t* const
	ichg_result	();

ushort_t
 	ichg		(input_t *i, ushort_t av_ct, ushort_t *av_ar);

input_chg_t*
 	ichg_child	(input_chg_t *parent,
		 	 av_t* a, ushort_t v,
			 ushort_t av_ct, ushort_t *av_ar);
#endif
