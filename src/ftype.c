#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sudoku3/ftype.h>

static ushort_t _s;
static f_t* _nums;
static f_t* _complements;

__attribute__((constructor))
static void _init() {
     long s = sizeof(f_t) * 8;
     _nums = malloc(sizeof(f_t) * s);
     _complements = malloc(sizeof(f_t) * s);
     _s = (ushort_t) s;
     long idx, v;
     for(idx=0,v=1; idx<s; idx++,v++) {
	_nums[idx] = 1 << idx;
	_complements[idx] = (f_t) ~ _nums[idx];
     }
};
__attribute__((destructor))
static void _deinit() {
	free(_nums);
	_nums = 0;
	free(_complements);
	_complements = 0;
}

__attribute__((hot))
ushort_t ft_isset(f_t* s, ushort_t pos) {
  assert(pos <= _s);
  return *s & _nums[pos-1];
}

ushort_t ft_is_listunset(f_t* s, ushort_t n, ushort_t pos[]) {
   ushort_t idx;
   f_t t;
   t = *s;
   for (idx = 0; idx < n; idx++) {
	ft_unset(&t, pos[idx]);
   }
   ushort_t r = t == *s;
   return r;
}

ushort_t ft_is_listset(f_t* s, ushort_t n, ushort_t pos[]) {
   ushort_t idx;
   f_t t;
   t = 0;
   for (idx = 0; idx < n; idx++) {
	ft_set(&t, pos[idx]);
   }
   ushort_t r = (t & *s) == t;
   return r;
}

ushort_t ft_is_listsetn(f_t* s, ushort_t n, ...) {
   va_list sa;
   va_start(sa, n);
   ushort_t idx;
   f_t t;
   t = 0;
   for (idx = 0; idx < n; idx++) {
	int pos = va_arg(sa, int);
	ft_set(&t, (ushort_t)pos);
   }
   va_end(sa);
   ushort_t r = (t & *s) == t;
   return r;
}

ushort_t ft_is_listunsetn(f_t* s, ushort_t n, ...) {
   va_list sa;
   va_start(sa, n);
   ushort_t idx;
   f_t t;
   t = *s;
   for (idx = 0; idx < n; idx++) {
	int pos = va_arg(sa, int);
	ft_unset(&t, (ushort_t)pos);
   }
   va_end(sa);
   ushort_t r = t == *s;
   return r;
}

void ft_set(f_t* s,ushort_t pos) {
  assert(pos <= _s);
  *s = *s | _nums[pos-1];
}

void ft_unset(f_t* s, ushort_t pos) {
  assert(pos <= _s);
  *s = *s & _complements[pos-1];
}

ushort_t ft_allset(f_t* s) {
  return 0 == (f_t) ~ (*s);
}

ushort_t ft_noneset(f_t* s) {
	return 0 == *s;
}

ushort_t ft_list_set_ct(f_t* s, ushort_t par_s) {
  ushort_t ps = par_s < _s ? par_s : _s;
  ushort_t idx, pos, i;
  for(idx = 0, i = 0, pos = 1; idx < ps; idx++, pos++) {
     if (ft_isset(s, pos))
	i++;
  }
  return i;
}

ushort_t ft_list_set(f_t* s, ushort_t** par, ushort_t par_s) {
  assert(par);
  ushort_t ps = par_s < _s ? par_s : _s;
  ushort_t ploc[ps];
  memset(ploc, 0, sizeof(ploc));
  ushort_t idx, pos, i;
  for(idx = 0, i = 0, pos = 1; idx < ps; idx++, pos++) {
     if (ft_isset(s, pos))
	ploc[i++] = pos;
  }
  long ploc_s = sizeof(ushort_t) * i;
  *par = malloc(ploc_s);
  memcpy(*par, ploc, ploc_s);
  return i;
}

ushort_t ft_list_unset(f_t* s, ushort_t** par, ushort_t par_s) {
  assert(par);
  ushort_t ploc[_s];
  memset(ploc, 0, sizeof(ploc));
  ushort_t idx, pos, i;
  for(idx = 0, i = 0, pos = 1; idx < _s && idx < par_s; idx++, pos++) {
     if (ft_isset(s, pos) == 0)
	ploc[i++] = pos;
  }
  long ploc_s = sizeof(ushort_t) * i;
  *par = malloc(ploc_s);
  memcpy(*par, ploc, ploc_s);
  return i;
}
