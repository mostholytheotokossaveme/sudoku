#ifndef TBL_H
#define TBL_H
#include "rect.h"

struct tbl {
   rect_t* osq;
   rect_t* isq;
   ushort ct;
   ushort sz;
   ushort selected;
   ushort hover;
   ushort mrgn;
   ushort* esidx;
   ushort  esct;
};
typedef struct tbl tbl_t;

tbl_t tbl_create(rect_t* r, ushort sz, ushort mrgn);
rect_t* tbl_srange(tbl_t *t, coord_size_t x, coord_size_t y);
#endif
