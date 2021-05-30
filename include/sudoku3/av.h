#ifndef AV_H
#define AV_H 1
#include <sudoku3/ftype.h>

typedef unsigned long ulong_t;

struct availability {
  f_t *v1, *v2, *v3;
  ushort_t cv;  f_t av;  f_t nav;
  ushort_t v;
} availability;

typedef struct availability av_t;

ushort_t av_chg_v(av_t* a, ushort_t cv);
void av_upd(av_t* a);

ushort_t av_set(av_t* ar, ushort_t cv, ushort_t ri, ushort_t ci);
ushort_t av_seti(av_t* ar, ushort_t cv, ushort_t i);
ushort_t av_unseti(av_t* ar, ushort_t i);
ushort_t av_set_sing(av_t* ar);
int av_sing_idx(av_t* ar);
void av_upd_r(av_t* ar, ushort_t r);
void av_upd_c(av_t* ar, ushort_t c);
void av_upd_s(av_t* ar, ushort_t r, ushort_t c);

ushort_t _idx(ushort_t,ushort_t);
ushort_t _sq_idx(ushort_t r, ushort_t c);
void _sq_n(ushort_t sidx, ushort_t**n, size_t* ns);
#endif
