#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <sudoku3/ftype.h>
#include <sudoku3/av.h>
#include <sudoku3/settings.h>
#include <sudoku3/sudoku.h>
#include <sudoku3/solver.h>
#include <sudoku3/ichg.h>

static ushort_t RESULT_NO = 0;
static ushort_t RESULT_YES = 1;

static input_ctx_t ichg_ctx;

static input_chg_t* get_current() {
	return ichg_ctx.cur;
}

static void set_current(input_chg_t* c) {
	ichg_ctx.cur=c;
}

static void _free(input_chg_t *p) {
 while (p->children_ct) {
   p->children_ct = p->children_ct - 1;
   _free(p->children[p->children_ct]);
 }
 free(p->children);
 free(p);
}

void ichg_ctx_set(input_ctx_t *pctx) {
	ichg_ctx = *pctx;
}
void ichg_ctx_get(input_ctx_t *pctx) {
	*pctx = ichg_ctx;
}

void ichg_ctx_new(input_ctx_t *pctx) {
   input_ctx_t c = (input_ctx_t) { 0,0,0 };
   input_chg_t r = { 0, 0, 0, NULL, NULL, NULL, 0, 0, NULL };
   r.children_sz = CHILDREN_INIT_SIZE;
   r.children = malloc(sizeof(input_chg_t*) * r.children_sz);
   r.children_ct = 0;
   c.pr = malloc(sizeof(r));
   *c.pr = r;
   c.cur = c.pr;
   *pctx=c;
}

void ichg_ctx_free(input_ctx_t *pctx) {
    _free(pctx->pr);
    pctx->pr = 0;
}

void ichg_result_yes() {
   ushort_t *r = &RESULT_YES;
   input_chg_t *i = get_current();
   while(i) { i->result = r; i = i->parent; }
   set_current( ichg_ctx.pr );
}

const ushort_t* const ichg_result() {
   return ichg_ctx.pr->result;
}

void set_rno(input_chg_t *i) {
   i->result = &RESULT_NO;
   input_chg_t *p = i->parent;
   if (p) {
	ushort_t cct = p->children_ct;
	assert(cct);
	if(i->av_ct - cct <= 0) {
        	while(cct--)	if (p->children[cct]->result != &RESULT_NO)
			   return;
	  set_rno(p);
	}
   }
}

void ichg_result_no() {
   ichg_ctx.last = ichg_ctx.cur;
   set_rno(ichg_ctx.cur);
   set_current( ichg_ctx.pr );
}

static input_chg_t* potential_child(input_chg_t ** children, ushort_t ct, ushort_t av_ct, ushort_t* av_ar) {
	input_chg_t *pch = 0;

	while(av_ct--) {
	  ushort_t cct = ct;
	  ushort_t av = av_ar[av_ct];
	  while(cct--) {
	    input_chg_t *ch = children[cct];
	    ushort_t chct, pchct;
	    chct = ch->children_ct;
	    pchct = pch ? pch->children_ct : 0;
	    if (ch->v == av)
		if(ch->result != &RESULT_NO) {
		  if (!pch || chct < pchct) {
		    pch = ch;
	 	    assert(pch->result == 0 || * pch->result != RESULT_NO);
		  }
	        } else cct = 0;
	  }
	}
	return pch;
}

static ushort_t has_potential2(input_chg_t ** children, ushort_t ct, ushort_t v, input_chg_t **f) {
   *f = NULL;
   while(ct--) {
	input_chg_t *child = children[ct];
	if (child->v == v) {
	   *f = child;
	   if(child->result && (*child->result == RESULT_NO))
		return 0;
	}
   }
   return 1;
}

static ushort_t has_potential1(input_chg_t ** children, ushort_t ct, ushort_t v) {
    input_chg_t *ch;
    if (has_potential2(children,ct,v,&ch))
	return ch == NULL;
    return 0;
}

ushort_t ichg(input_t* i, ushort_t av_ct, ushort_t *av_ar) {
   ushort_t v, cct, av_idx;

   input_chg_t ** children;
   input_chg_t *child = 0;
   ushort_t nr = 0;

   children = get_current()->children,
	v = 0,
	cct = get_current()->children_ct,
	av_idx = 0;

   while(av_idx < av_ct && !has_potential1(children,cct,av_ar[av_idx])) {
	  av_idx++;
   }

   if (av_idx < av_ct) {
       get_current()->result = NULL;
       v = av_ar[av_idx];
       set_current(
		ichg_child(get_current(), i->a, v, av_ct, av_ar)
       );
   } else {
	child = potential_child(children,cct,av_ct,av_ar);
	if (child) {
	  v = child->v;
	  set_current( child );
	}
   }

   return v;
}

input_chg_t* ichg_child(
	input_chg_t *parent,
	av_t* a, ushort_t v,
	ushort_t av_ct, ushort_t *av_ar) {
	input_chg_t c = {
	  .a=a,
	  .v=v,
	  .av_ct=av_ct,
	  .av_ar=av_ar,
	  .parent = parent,
	  .children=NULL,
	  .children_ct = 0,
	  .children_sz = 0,
	  .result = NULL
	};
	input_chg_t *pc = malloc(sizeof(c));
	*pc = c;
	if (parent != NULL) {
	 if (parent->children_ct >= parent->children_sz) {
	   parent->children_sz = parent->children_sz + CHILDREN_STEP_SIZE;
	   long ms = sizeof(input_chg_t*) * parent->children_sz;
	   parent->children = realloc(parent->children, ms);
	 }
	 pc->parent = parent;
         parent->children[parent->children_ct] = pc;
	 parent->children_ct = parent->children_ct + 1;
	}
	return pc;
}
