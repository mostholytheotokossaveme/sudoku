#include <assert.h>
#include <alloca.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sudoku3/av.h>
#include <sudoku3/settings.h>
#include <sudoku3/ftype.h>

void test_ft_isset(){
  f_t* ap; ushort_t num;
  f_t a = 0;

  ap=&a;
  num = 10;
  ft_set(ap, num);
  assert(ft_isset(ap, num));
  assert(0 == ft_isset(ap, ++num));
}
void test_ft_unset() {
 f_t a = 7;
 ft_unset(&a, 3);
 assert(a == 3);
 ft_unset(&a, 2);
 assert(a == 1);
 ft_unset(&a, 1);
 assert(a == 0);
}
void test_ft_set() {
 f_t a = 0;
 ft_set(&a, 1);
 assert(a == 1);
 ft_set(&a, 2);
 assert(a == 3);
 ft_set(&a, 3);
 assert(a == 7);
 a=0;
 ft_set(&a, 3);
 assert(a == 4);
}

void test_ft_allset() {
 f_t a = (f_t) ~ (f_t) 0;
 assert(ft_allset(&a));
 while(a--) assert(0 == ft_allset(&a));
}
void test_ft_noneset() {
 f_t a = (f_t) ~ (f_t) 0;
 do assert(0 == ft_noneset(&a));
 while(--a);
 assert(ft_noneset(&a));
}


void test_ft_list_unset() {
  ushort_t sz = sizeof(f_t) * 8;
  f_t a = (f_t) ~ (f_t) 0;
  ft_unset(&a, 1);
  ft_unset(&a, 2);
  ft_unset(&a, 3);
  ushort_t ps[] = {1,2,3};
  ushort_t *p = alloca(sizeof(ushort_t) * sz);
  ushort_t s = ft_list_unset(&a, &p, sz);
  assert(s == 3);
  while(s--)
	assert(ps[s] == p[s]);
}
void test_ft_list_set() {
  ushort_t sz = sizeof(f_t) * 8;
  f_t a = 7;
  ushort_t ps[] = {1,2,3};
  ushort_t *p = alloca(sizeof(ushort_t) * sz);
  ushort_t s = ft_list_set(&a, &p, sz);
  assert(s == 3);
  while(s--)
	assert(ps[s] == p[s]);
}

void test_ft_is_listset() {
  f_t a = 7;
  assert(ft_is_listsetn(&a, 3, 1,2,3));
  ushort_t pos[] = {1,2,3};
  assert(ft_is_listset(&a, 3, pos));
  a = 4;
  assert(0 == ft_is_listset(&a, 3, pos));
  assert(ft_is_listsetn(&a, 1, 3));
}

void test_ft_is_listunset() {
  f_t a = ~0;
  ft_unset(&a, 1);
  ft_unset(&a, 2);
  ft_unset(&a, 3);
  assert(ft_is_listunsetn(&a, 3, 1,2,3));
  ushort_t pos[] = {1,2,3};
  assert(ft_is_listunset(&a, 3, pos));
  ft_set(&a, 1);
  ft_set(&a, 2);
  assert(0 == ft_is_listunset(&a, 3, pos));
  assert(ft_is_listunsetn(&a, 1, 3));
}

int main() {
	test_ft_set();
	test_ft_unset();
	test_ft_isset();
	test_ft_allset();
	test_ft_noneset();
 	test_ft_list_set();
 	test_ft_list_unset();
 	test_ft_is_listset();
 	test_ft_is_listunset();
	return 0;
}
