#include <assert.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sudoku3/sgio.h>

FILE *open4() {
 return fopen("io4.b", "wb+");
}
FILE *open9() {
 return fopen("io9.b", "wb+");
}

sgio_t test_append(int belsz) {
	unsigned short a4[belsz];
	unsigned short b4[belsz];
	int k;
	for (k=0;k<belsz;k++)
		a4[k] = k+1;
	assert(a4[0]==1);
	assert(a4[belsz-1]==belsz);
	for (k=0;k<belsz;k++)
		b4[k] = k+1+10;
	assert(b4[0]==11);
	assert(b4[belsz-1]==belsz+10);

	sgio_t sg = { 0,0,0,0, NULL };
	sg.belsz = belsz;
	char *d1 = 0;
	char *d2 = 0;
	d1 = sgio_buf(a4, belsz);
	d2 = sgio_buf(b4, belsz);
	sgio_append(&sg, d1);
	sgio_append(&sg, d2);
	free((void*)d2);
	free((void*)d1);
	assert(sg.ct == 2);
	assert(sg.sz >= 2);

	assert(sg.buf[0] == 1);
	assert(sg.buf[belsz-1] == belsz);
	assert(sg.buf[0 + belsz] == 11);
	assert(sg.buf[belsz + belsz-1] == belsz+10);
	for(k=0;k<belsz;k++) {
	  assert(sg.buf[k] == (char)a4[k]);
	  assert(sg.buf[k+belsz] == (char)b4[k]);
	}
	return sg;
}

void test_to(sgio_t sg, int belsz) {
	FILE* fr;
        if(belsz==16) {
	  fr = open4();
	} else{
	  fr = open9();
	}
        sgio_t t2 = sg;
	t2.buf = malloc(sizeof(char) * sg.ct * belsz);
	memcpy(t2.buf, sg.buf, sizeof(char) * sg.ct * belsz);
	t2.sz = t2.ct;

	sgio_to(fr, sg);
	fflush(fr);
	fclose(fr);
        if(belsz==16) {
	  fr = open4();
	} else{
	  fr = open9();
	}

	sgio_t c4 = {0,0,0,0, NULL};
	sgio_from(fr, &c4);
	fclose(fr);
	assert(c4.ct == sg.ct);
	assert(c4.sz == sg.sz);
	int i;
	for(i=0;i< c4.ct * belsz;i++) {
	  assert(c4.buf[i] == sg.buf[i]);
	}
}

void test_bf() {
	unsigned short t1[] = { 1,2,3,4 };
	unsigned short t2[] = { 5,6,7,8,
				9,10,11,12,
				13,14,15};
	char* b = sgio_buf(t1,4);
	char* c = sgio_buf(t2,11);
	int idx;
	for (idx =0; idx < 4; idx++)
	   assert(b[idx] == (char)t1[idx]);
	free(b);
	b = 0;
	for (idx =0; idx < 10; idx++)
	   assert(c[idx] == (char)t2[idx]);
	free(c);
	c = 0;
}

int main() {
  test_bf();
  test_bf();
  test_bf();
  int belsz;

  belsz =16;
  sgio_t sg = test_append(belsz);
  test_to(sg,belsz);

  belsz = 81;
  sg = test_append(belsz);
  test_to(sg,belsz);
}
