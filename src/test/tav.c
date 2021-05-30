#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <sudoku3/av.h>
#include <sudoku3/settings.h>
#include <sudoku3/ftype.h>

void _sq_n(ushort_t sidx, ushort_t** n, size_t *ns);

void test_av_chg_v() {
	f_t v1,v2,v3;
	v1=0; v2=1; v3=2;
	av_t a = {&v1, &v2, &v3, 0,0,0,0};
	av_chg_v(&a, 3);
	assert(v1==4 && v2==5 && v3 == 6 && a.nav == 7 && a.cv == 3);
	assert(a.av == (f_t)~a.nav);
}

void test_av_upd() {
	f_t v1,v2,v3;
	v1=0; v2=1; v3=2;
	av_t a = {&v1, &v2, &v3, 0,0,0,0};
	av_upd(&a);
	assert(v1==0 && v2==1 && v3 == 2 && a.nav == 3 && a.cv == 0);
	assert(a.av == (f_t)~a.nav);
}

void setup_ar(ushort_t psz, av_t** par, f_t **pv1, f_t **pv2, f_t **pv3) {
	settings_t s = { psz, (ushort_t)sqrt(psz) };
	ushort_t sz = s.sz;
	settings_set(&s);
	ushort_t ct = sz*sz;
	f_t *v1 = malloc(sizeof(f_t) * ct);
	f_t *v2 = malloc(sizeof(f_t) * ct);
	f_t *v3 = malloc(sizeof(f_t) * ct);
	*pv1 = v1;
	*pv2 = v2;
	*pv3 = v3;
	memset(v1, 0, sizeof(f_t) * ct);
	memset(v2, 0, sizeof(f_t) * ct);
	memset(v3, 0, sizeof(f_t) * ct);
	av_t* ar = malloc(sizeof(av_t) * ct);
	*par = ar;
	memset(ar, 0, sizeof(av_t) * ct);
	while(ct--) {
	  av_t* a = ar + ct;
	  a->v1 = &v1[ct];
	  a->v2 = &v2[ct];
	  a->v3 = &v3[ct];
	  a->cv = 0;
	  av_upd(a);
        };
}

void test_av_set() {
	f_t *v1, *v2, *v3;
	av_t* ar;
	setup_ar(4, &ar, &v1, &v2, &v3);
	assert(av_set(ar, 1, 0, 0));
	assert(av_set(ar, 2, 0, 1));
	assert(av_set(ar, 3, 1, 1));
	assert((ar + 0 + 0)->cv == 1);
	assert((ar + 0 + 0)->nav == 1);
	assert(v1   [0 + 0] == 1);
	assert(v2   [0 + 0] == 1);
	assert(v3   [0 + 0] == 1);

	assert((ar + 0 + 1)->cv == 2);
	assert(v1[   0 + 1] == 2);

	assert((ar + 4 + 1)->cv == 3);
	assert(v1[   4 + 1] == 4);

	assert((ar + 0 + 2)->cv == 0);
	assert(v1[   0 + 2] == 0);

	void* fr[] = {ar,v1,v2,v3};
	int n = 4;
	while(n--)
	  free(fr[n]);
}

void test_av_set_sing() {
	f_t *v1, *v2, *v3;
	av_t* ar;
	setup_ar(4, &ar, &v1, &v2, &v3);

	(ar+1)->av=1;
	(ar+11)->av=1;
	while(av_set_sing(ar));
	assert((ar+0)->cv == 0);
	assert((ar+1)->cv == 1);
	assert((ar+11)->cv == 1);
	assert((ar+10)->cv == 0);

	void* fr[] = {ar,v1,v2,v3};
	int n = 4;
	while(n--)
	  free(fr[n]);
}

void test_sq_n() {
	settings_t s = { 4, 2 };
	ushort_t sz = s.sz;
	settings_set(&s);

	ushort_t* idx;
	size_t ct;
	_sq_n(0, &idx, &ct);
	assert(ct == 4);
	assert(idx[0] == 0);
	assert(idx[1] == 1);
	assert(idx[2] == 4);
	assert(idx[3] == 5);

	free(idx);idx=0;

        s.sz = 9; s.sz_sqrt = 3;
	settings_set(&s);
	_sq_n(3, &idx, &ct);
	assert(ct == 9);
	assert(idx[0] == 27);
	assert(idx[3] == 36);
	assert(idx[6] == 45);
	assert(idx[8] == 47);
}

void test_av_upd_r() {
	f_t *v1, *v2, *v3;
	av_t* ar;
	ar=0, v1=0, v2=0, v3=0;
	setup_ar(4, &ar, &v1, &v2, &v3);

	ushort_t ct = 16;
	while(ct--) v1[ct] = v2[ct] = v3[ct] = 1;
	av_upd_r(ar, 0);
	ct = 4;
	while(ct--) assert((ar + ct)->nav == 1);
	assert((ar+5)->nav == 0);
	av_upd_r(ar, 1);
	assert((ar+4)->nav);
	assert((ar+5)->nav);
	assert((ar+6)->nav);
	assert((ar+7)->nav);
	assert((ar+8)->nav == 0);

	void* fr[] = {ar,v1,v2,v3};
	int n = 4;
	while(n--)
	  free(fr[n]);
	ar=0, v1=0, v2=0, v3=0;
}

void test_av_upd_c() {
	f_t *v1, *v2, *v3;
	av_t* ar;
	int n = 4;
	ar=0, v1=0, v2=0, v3=0;
	setup_ar(n, &ar, &v1, &v2, &v3);

	ushort_t ct = n*n;
	while(ct--) v1[ct] = v2[ct] = v3[ct] = 1;
	av_upd_c(ar, 0);
	ct = 4;
	while(ct--) {
		assert((ar + ct * n + 0)->nav == 1);
		assert((ar + ct * n + 1)->nav == 0);
	}

	void* fr[] = {ar,v1,v2,v3};
	ct = n;
	while(ct--)
	  free(fr[ct]);
	ar=0, v1=0, v2=0, v3=0;
	setup_ar(n, &ar, &v1, &v2, &v3);
	ct = n*n;
	while(ct--) v1[ct] = v2[ct] = v3[ct] = 1;

	av_upd_c(ar, 1);
	ct = n;
	while(ct--) {
		assert((ar + ct * n + 1)->nav == 1);
		assert((ar + ct * n + 2)->nav == 0);
	}

	void *fr2[] = {ar,v1,v2,v3};
	ct = n;
	while(ct--)
	  free(fr2[ct]);
	ar=0, v1=0, v2=0, v3=0;
}

void test_av_upd_s() {
	f_t *v1, *v2, *v3;
	av_t* ar;
	ar=0, v1=0, v2=0, v3=0;
	ushort_t n = 9;
	setup_ar(n, &ar, &v1, &v2, &v3);

	ushort_t ct = n*n;
	while(ct--) v1[ct] = v2[ct] = v3[ct] = 1;


	av_upd_s(ar,4,4);

	assert(ar->nav == 0);
	assert((ar+29)->nav == 0);
	assert((ar+30)->nav);	assert((ar+31)->nav);	assert((ar+32)->nav);
	assert((ar+39)->nav);	assert((ar+40)->nav);	assert((ar+41)->nav);
	assert((ar+48)->nav);	assert((ar+49)->nav);	assert((ar+50)->nav);
	assert((ar+51)->nav == 0);
	assert((ar+80)->nav == 0);

	void* fr[] = {ar,v1,v2,v3};
	ct = 4;
	while(ct--)
	  free(fr[ct]);
	ar=0, v1=0, v2=0, v3=0;
}
/*
void av_upd_r(av_t* ar, ushort_t r);
void av_upd_c(av_t* ar, ushort_t c);
void av_upd_s(av_t* ar, ushort_t r, ushort_t c);
*/
int main() {
	test_sq_n();
	test_av_chg_v();
	test_av_upd();
	test_av_set();
	test_av_set_sing();
	test_av_upd_r();
	test_av_upd_c();
	test_av_upd_s();
	return 0;
}
