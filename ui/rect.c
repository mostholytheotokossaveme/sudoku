#include <assert.h>
#include <stdlib.h>
#include "rect.h"

rect_t rect_create                      (coord_size_t x,coord_size_t y,coord_size_t w,coord_size_t h) {
  return  (rect_t) {.x=x, .y=y, .w = w, .h=h };
}

rect_t rect_square                      (coord_size_t x,coord_size_t y,coord_size_t sz) {
  return rect_create(x,y,sz,sz);
}

rect_t* rect_split			(rect_t* sq, coord_size_t w, coord_size_t h, ushort *sct) {
   assert(sq);
   assert(sct);
   int ri,ci, rc, cc, ct, idx;
   cc = sq->w / w;
   rc = sq->h / h;
   ct = rc*cc;
   rect_t* s = calloc(ct, sizeof(rect_t));
   for(ri=0, idx=0; ri<rc; ri++) {
     for(ci=0; ci<cc; ci++, idx++) {
	s[idx].x = sq->x + ci * w;
	s[idx].y = sq->y + ri * h;
	s[idx].w = w;
	s[idx].h = h;
     }
   }
   *sct = ct;
   return s;
}

short  rect_point                       (rect_t* r, point_t* p) {
	if (p->x < r->x) 	  return 0;
	if (p->y < r->y)   	  return 0;
	if (p->x > (r->x+r->w))   return 0;
	if (p->y > (r->y+r->h))   return 0;
	return 1;
}
