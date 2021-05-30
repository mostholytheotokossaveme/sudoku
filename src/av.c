#include <assert.h>
#include <stdlib.h>
#include <sudoku3/ftype.h>
#include <sudoku3/av.h>
#include <sudoku3/settings.h>

ushort_t _idx(ushort_t r, ushort_t c) {
	settings_t* s = settings_get();
	return r* s->sz + c;
}

void _ridx(ushort_t idx, ushort_t *r, ushort_t *c) {
	settings_t* s = settings_get();
	*r = idx / s->sz;
	*c = idx % s->sz;
}

ushort_t _sq_idx(ushort_t r, ushort_t c) {
	settings_t* s = settings_get();
	ushort_t sz_sqrt = s->sz_sqrt;
	return (((ushort_t)(r/sz_sqrt)) * sz_sqrt) + (c/sz_sqrt);
}

void _sq_n(ushort_t sidx, ushort_t** n, size_t *ns) {
	assert(n);
	assert(ns);
	settings_t* s = settings_get();
	const ushort_t sz = s->sz, sz_sqrt = s->sz_sqrt;
        const ushort_t sri = (sidx/sz_sqrt) * (sz_sqrt);
	const ushort_t sci = (sidx%sz_sqrt) * (sz_sqrt);
	const ushort_t fn = sri * sz + sci;
	*n = malloc(s->sz * sizeof(ushort_t));
	ushort_t *pn = *n;
	*ns = sz;
	ushort_t sr,sc,i;
	for (sr = 0, i=0; sr < sz_sqrt; sr++) {
	   for (sc = 0; sc < sz_sqrt; sc++,i++) {
		pn[i] = fn + sr * sz + sc;
	   }
	}
}

void av_reset(av_t* a) {
  ushort_t cv = a->cv;
  if (cv != 0) {
   ft_set(a->v1, cv);
   ft_set(a->v2, cv);
   ft_set(a->v3, cv);
   av_upd(a);
  }
}

void av_resets(av_t* ar, ushort_t* pr, ushort_t* pc) {
  assert(ar);
  ushort_t sz,r,c,szc;
  sz = settings_get()->sz;
  szc = sz;

  if (pr) {
	  r = *pr;
	  while(sz--)
		av_reset(ar + _idx(r,sz));
	  sz = szc;
  }
  if (pc) {
	 c = *pc;
	 while(sz--)
	 	 av_reset(ar + _idx(sz,c));
	 sz = szc;
  }
  if (pr && pc) {
	ushort_t s = _sq_idx(r,c);
	ushort_t *n;
	size_t ns;
	_sq_n(s, &n, &ns);
	while(ns--)
		av_reset(ar + n[ns]);
  }
}



ushort_t av_chg_v(av_t* a, ushort_t cv) {
   if (a->cv != cv) {
	if (a->cv == 0) {
	   ft_set(a->v1, cv);
	   ft_set(a->v2, cv);
	   ft_set(a->v3, cv);
	   a->cv = cv;
	   a->v = a->v + 1;
	   av_upd(a);
	   return 1;
	}
	assert(0);
   }
   return 0;
}

void av_upd(av_t* a) {
  f_t av = (f_t)~0;
  av &= (f_t) ~*(a->v1);
  av &= (f_t) ~*(a->v2);
  av &= (f_t) ~*(a->v3);
  f_t nav = (f_t) ~av;
  a->av = av;
  a->nav = nav;
}

ushort_t av_set(av_t* ar, ushort_t cv, ushort_t ri, ushort_t ci) {
  ushort_t chg = av_chg_v(ar + _idx(ri,ci), cv);
  if (chg) {
 	 av_upd_r(ar, ri);
 	 av_upd_c(ar, ci);
 	 av_upd_s(ar, ri, ci);
  }
  return chg;
}

ushort_t av_seti(av_t* ar, ushort_t cv, ushort_t i) {
  ushort_t chg = av_chg_v(ar + i, cv);
  if (chg) {
	 ushort_t r,c;
	 _ridx(i,&r,&c);
	 assert(i == _idx(r,c));
 	 av_upd_r(ar, r);
 	 av_upd_c(ar, c);
 	 av_upd_s(ar, r, c);
  }
  return chg;
}

ushort_t av_unseti(av_t* ar, ushort_t i) {
  av_t *a = ar + i;
  if (a->cv) {
	a->cv = 0;
        a->v = a->v + 1;
	*a->v1 = 0;
	*a->v2 = 0;
	*a->v3 = 0;
  }
	ushort_t r,c;
	_ridx(i,&r,&c);
	assert(i == _idx(r,c));
	av_resets(ar, &r, &c);
	av_upd(a);
 	av_upd_r(ar, r);
 	av_upd_c(ar, c);
 	av_upd_s(ar, r, c);

  return a->cv;
}

int av_sing_idx(av_t* ar) {
 ushort_t sz,idx;
 ushort_t r,c;

 sz = settings_get()->sz;
 for(r=0,idx=0;r<sz;r++) {
  for(c=0;c<sz;c++,idx++) {
    av_t* a = ar + idx;
    if(a->cv == 0 && 1 == ft_list_set_ct(&a->av, sz)) {
	return idx;
    }
  }
 }
 return -1;
}

ushort_t av_set_sing(av_t* ar) {
 settings_t* s = settings_get();
 ushort_t sz = s->sz;
 ushort_t *av = calloc(sizeof(ushort_t), sz);
 ushort_t r,c;
 for(r=0;r<sz;r++) {
  for(c=0;c<sz;c++) {
    av_t* a = ar + _idx(r,c);
    if(1 == ft_list_set(&a->av, &av, sz)) {
 	if(av_chg_v(a, av[0])) {
 	 av_upd_r(ar, r);
 	 av_upd_c(ar, c);
 	 av_upd_s(ar, r, c);
	 free(av); av = 0;
	 return 1;
	}
    }
  }
 }
 free(av);
 av = 0;
 return 0;
}

void av_upd_r(av_t* ar, ushort_t r) {
  ushort_t sz = settings_get()->sz;
  while(sz--)
	av_upd(ar + _idx(r,sz));
}

void av_upd_c(av_t* ar, ushort_t c) {
  ushort_t sz = settings_get()->sz;
  while(sz--)
	av_upd(ar + _idx(sz,c));
}

void av_upd_s(av_t* ar, ushort_t r, ushort_t c) {
	ushort_t s = _sq_idx(r,c);
	ushort_t *n;
	size_t ns;
	_sq_n(s, &n, &ns);
	while(ns--)
		av_upd(ar + n[ns]);
}

