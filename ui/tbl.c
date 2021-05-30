#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "tbl.h"

tbl_t tbl_create(rect_t* sq, ushort sz, ushort mrgn) {
  tbl_t t;
  t.sz=sz;
  t.ct = sz*sz;
  t.mrgn = mrgn;
  t.selected = -1;
  t.hover = -1;
  t.esct = 0;
  t.esidx = calloc(t.ct, sizeof(ushort));
  ushort sct;
  rect_t p = *sq;
  p.x += mrgn;
  p.y += mrgn;
  p.w -= mrgn*2;
  p.h -= mrgn*2;
  t.osq = rect_split(&p, p.w/sz, p.h/sz, &sct);
  long ms = sizeof(rect_t) * sct;
  t.isq = malloc(ms);
  memcpy(t.isq, t.osq, ms);
  ushort ct = sct;
  while(ct--){
	t.isq[ct].x += mrgn;
	t.isq[ct].y += mrgn;
	t.isq[ct].w -= (mrgn*2);
	t.isq[ct].h -= (mrgn*2);
  }
  return t;
}

rect_t* tbl_srange(tbl_t *t, coord_size_t x, coord_size_t y) {
assert(0); return (void*)0;
}
