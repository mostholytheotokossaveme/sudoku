#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sudoku3/sgio.h>

int sgio_to(FILE *f, sgio_t io) {
   fwrite(&io, sizeof(io.ver), 4, f);
   fwrite(io.buf, sizeof(char), io.belsz * io.ct, f);
   fflush(f);
}

int sgio_from(FILE *f, sgio_t *io) {
   sgio_t n;
   fread(&n, sizeof(n.ver), 4, f);
   assert(n.belsz);
   assert(n.ct);
   assert(n.sz);
   assert(n.ct <= n.sz);
   char buf[n.belsz * n.sz];
   fread(buf, sizeof(char), n.belsz * n.ct, f);
   char *bp = (char*) malloc(n.belsz * n.sz);
   memcpy(bp, buf, n.belsz * n.sz);
   n.buf = bp;
   *io = n;
}

void sgio_maintain_sz(sgio_t *sg) {
  if (sg->ct >= sg->sz) {
   sg->sz += 10;
   if (sg->buf == NULL)
	sg->buf = malloc(sizeof(sg->sz * sg->belsz));
   else
	sg->buf = realloc(sg->buf, sizeof(sg->sz * sg->belsz));
  }
}

void sgio_append(sgio_t* sg, char* bel) {
   sgio_maintain_sz(sg);
   char* p = sg->buf + (sg->ct * sg->belsz);
   sg->ct = sg->ct + 1;
   memcpy(p, bel, sg->belsz);
}

char* sgio_buf(unsigned short* p, unsigned short psz) {
	char * r = malloc(psz * sizeof(char));
	while(psz--) r[psz] = (char)p[psz];
	return r;
}

