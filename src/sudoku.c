#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <alloca.h>
#include <math.h>
#include <sudoku3/ftype.h>
#include <sudoku3/av.h>
#include <sudoku3/settings.h>
#include <sudoku3/sudoku.h>

void sudoku_size(ushort_t sz) {
	settings_t s = {sz, (ushort_t)sqrt(sz)};
	settings_set(&s);
}

void sudoku_copyav(sudoku_t* d, sudoku_t *s) {
   ushort_t sz = s->sz;
   ushort_t r,c;
   for(r=0; r < sz; r++)
	for(c=0;c < sz; c++){
	   ushort_t idx = _idx(r,c);
	   av_t* da = d->ar + idx;
	   av_t* sa = s->ar + idx;
	   da->cv = sa->cv;
	}
   for(r=0; r < sz; r++)
	for(c=0;c < sz; c++){
	   ushort_t idx = _idx(r,c);
	   av_t* da = d->ar + idx;
	   av_upd(da);
	}

}

sudoku_t* sudoku_copy(sudoku_t* s) {
   ushort_t sz = s->sz;
   sudoku_t* t = sudoku_new();
   assert(t->sz == sz);
   memcpy(t->v1, s->v1, s->v_memsize);
   memcpy(t->v2, s->v2, s->v_memsize);
   memcpy(t->v3, s->v3, s->v_memsize);
   memcpy(t->seq, s->seq, s->seq_memsize);
   memcpy(t->ar, s->ar, s->ar_memsize);
   return t;
}

void sudoku_clear(sudoku_t* s) {
   memset(s->v1, 0, s->v_memsize);
   memset(s->v2, 0, s->v_memsize);
   memset(s->v3, 0, s->v_memsize);
   ushort_t ct = s->ct;
   while(ct--) {
	av_t* a = s->ar + ct;
	a->cv = 0;
	a->v = 0;
	av_upd(a);
   }
}

sudoku_t* sudoku_new() {
   sudoku_t s = {NULL,NULL,NULL,NULL,NULL, 0,0,0,0,0,0};
   s.sz = settings_get()->sz;
   s.ct = s.sz * s.sz;
   s.sz_sqrt = settings_get()->sz_sqrt;
   s.ar_memsize = sizeof(av_t) * s.ct;
   s.v_memsize = sizeof(f_t) * s.sz;
   s.seq_memsize = sizeof(ushort_t) * s.sz;
   s.ar = malloc(s.ar_memsize);
   memset(s.ar, 0, s.ar_memsize);
   s.v1 = malloc(s.v_memsize);
   s.v2 = malloc(s.v_memsize);
   s.v3 = malloc(s.v_memsize);
   memset(s.v1, 0, s.v_memsize);
   memset(s.v2, 0, s.v_memsize);
   memset(s.v3, 0, s.v_memsize);
   s.seq = malloc(s.seq_memsize);
   memset(((ushort_t*)s.seq), 0, s.seq_memsize);
   ushort_t i;
   for(i=0;i<s.sz;i++) {
	ushort_t* e = ((ushort_t*)s.seq) + i;
	*e = (ushort_t) 1 + i;
   }
   ushort_t r,c;
   for(r=0; r < s.sz; r++)
	for(c=0;c < s.sz; c++){
	   ushort_t idx = _idx(r,c);
	   av_t* a = s.ar + idx;
	   a->v1 = &s.v1[r];
	   a->v2 = &s.v2[c];
	   a->v3 = &s.v3[_sq_idx(r,c)];
	   av_upd(a);
	}
   sudoku_t* sudoku = malloc(sizeof(s));
   *sudoku = s;
   return sudoku;
}

void sudoku_free(sudoku_t* s) {
	free(s->v1);
	free(s->v2);
	free(s->v3);
	free(s->ar);
	free(s->seq);

	s->v1 = 0;
	s->v2 = 0;
	s->v3 = 0;
	s->ar = 0;
	s->seq = 0;

	s->ct = 0;
	s->sz = 0;
	s->sz_sqrt = 0;
	s->v_memsize = 0;
	s->ar_memsize = 0;
	free(s);
}

ushort_t sudoku_size_chk(ushort_t sz) {
	return sz == 4 || sz == 9;
}

ushort_t sudoku_is_rcorrect(sudoku_t *s) {
 /* correct rows */
 ushort_t sz = s->sz;
 while(sz--) {
	f_t v1 = s->v1[sz];
	ushort_t rc = ft_list_set_ct(&v1, s->sz);
	ushort_t fc = 0;
	av_t *a = s->ar + sz * s->sz;
	ushort_t rs = s->sz;
	while(rs--)
	  if((a+rs)->cv) fc++;
	if (rc != fc) return 0;
 }
 return 1;
}

ushort_t sudoku_is_ccorrect(sudoku_t *s) {
 /* correct cols */
 ushort_t sz = s->sz;
 while(sz--) {
	f_t v2 = s->v2[sz];
	ushort_t cc = ft_list_set_ct(&v2, s->sz);
	ushort_t fc = 0;
	av_t *a = s->ar + sz;
	ushort_t rs = s->sz;
	while(rs--)
	  if ((a+(rs*(s->sz)))->cv) fc++;
	if (cc != fc) return 0;
 }
 return 1;
}

ushort_t sudoku_is_scorrect(sudoku_t *s) {
 /* correct cols */
 ushort_t sz = s->sz;
 while(sz--) {
	f_t v3 = s->v3[sz];
	ushort_t sc = ft_list_set_ct(&v3, s->sz);
	ushort_t fc = 0;

	ushort_t* nidx; size_t ns;
	_sq_n(sz, &nidx, &ns);
	av_t *a = s->ar;
	while(ns--)
	   if ((a + nidx[ns])->cv)
		fc++;
	if (sc != fc) return 0;
 }
 return 1;
}


ushort_t sudoku_is_correct(sudoku_t* s) {
 if(!sudoku_is_rcorrect(s)) return 0;
 if(!sudoku_is_ccorrect(s)) return 0;
 if(!sudoku_is_scorrect(s)) return 0;
 return 1;
}

ushort_t sudoku_is_complete(sudoku_t* s) {
  ushort_t ct = s->ct;
  ushort_t fc = 0;
  while(ct--)
	if ((s->ar + ct)->cv)
		fc++;
	else
		return 0;
  return s->ct == fc;
}

ushort_t sudoku_is_solution(sudoku_t* s) {
 ushort_t sz = s->sz;
 ushort_t vset = 1;
 while(sz-- && vset) {
	f_t v1 = s->v1[sz];
	f_t v2 = s->v2[sz];
	f_t v3 = s->v3[sz];

	vset =
 	 ft_is_listset(&v1, s->sz, s->seq) &&
	 ft_is_listset(&v2, s->sz, s->seq) &&
	 ft_is_listset(&v3, s->sz, s->seq);
 }
 return vset;
}

ushort_t sudoku_has_avail(sudoku_t* s) {
  ushort_t ct = s->ct;
  while(ct--) {
	av_t* a = s->ar + ct;
	if(a->cv == 0 && ft_list_set_ct(&(a->av), s->sz))
		return 1;
  }
  return 0;
}

void jesus();
void thankyou();
