#include <stdlib.h>
#include <assert.h>
#include "rect.h"
#include "tbl.h"
#include "gi.h"


void* gi_open(int x, int y, int w, int h, int sz, int mrgn) {
   rect_t sq = {x,y,w,h};
   tbl_t t = tbl_create(&sq, sz, mrgn);
   tbl_t* p = malloc(sizeof(t));
   *p = t;
   return p;
}
tbl_t* gi_tbl(void* gi) {
  tbl_t *t;
  *(void**)&t = gi;
  return t;
}
void gi_free(void* g) {
assert(0);
}

void gi_on_kbd(void* g, unsigned char k) {
  if(k == 'q')
	exit(0);
}

static ushort_t gi_tgt_chg(ushort_t ct, ushort_t* tgt, ushort_t nv) {
   if (nv >= 0 && nv < ct) {
     if (*tgt != nv)
  	 *tgt = nv;
   } else if (*tgt < 0 || *tgt > ct){
	*tgt = -1;
   }
   return *tgt;
}

void gi_on_mv (void* g, enum GI_MV mv) {
   tbl_t* t;
   ushort_t delta, *tgt;
   t = gi_tbl(g);
   delta = 0;
   tgt = &(t->selected);
   if (mv == GI_UP)
	delta = -1 * t->sz;
   else if(mv == GI_DOWN)
	delta = t->sz;
   else if (mv == GI_RIGHT)
	delta = 1;
   else if (mv == GI_LEFT)
	delta = -1;

   gi_tgt_chg(t->ct, tgt, delta + *tgt);
}
void gi_on_mo (void* g, int x, int y) {
   tbl_t* t;
   t = gi_tbl(g);
   ushort_t ct;
   point_t p = {x,y};
   ct=t->ct;
   while(ct--)
	if (rect_point(t->osq + ct, &p)) {
	  gi_tgt_chg(t->ct, &(t->hover), ct);
	  ct = 0;
	}
}
void gi_on_md (void* g, int x, int y) {
   tbl_t* t;
   t = gi_tbl(g);
   ushort_t ct;
   point_t p = {x,y};
   ct=t->ct;
   while(ct--)
	if (rect_point(t->osq + ct, &p)) {
	  gi_tgt_chg(t->ct, &(t->selected), ct);
	  ct = 0;
	}
}

void gi_on_mu (void* g, int x, int y) {
}

