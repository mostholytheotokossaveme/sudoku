#ifndef SGIO_H
#include <stdio.h>
#define SGIO_H
#define SGIO_LEN_4 16
#define SGIO_LEN_9 81

struct sgios {
	unsigned short ver;
	unsigned short ct;
	unsigned short sz;
	unsigned short belsz;
	char* buf;
} sgios;

typedef struct sgios sgio_t;

int sgio_to(FILE *f, sgio_t);
int sgio_from(FILE *f, sgio_t *psg);
void sgio_append(sgio_t* sg, char*);
char* sgio_buf(unsigned short* p, unsigned short psz);
#endif
