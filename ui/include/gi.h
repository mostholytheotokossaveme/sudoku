#ifndef GI_H
#define GI_H
#include "tbl.h"
enum GI_MV {
	GI_UP, GI_DOWN, GI_RIGHT, GI_LEFT
};

void* gi_open(int x, int y, int w, int h, int sz, int mrgn);
tbl_t* gi_tbl(void*);
void gi_free(void*);

void gi_on_confirm(void* g, unsigned int k);
void gi_on_kbd(void*, unsigned int k);
void gi_on_mv (void*, enum GI_MV mv);
void gi_on_mo (void*, int x, int y);
void gi_on_md (void*, int x, int y);
void gi_on_mu (void*, int x, int y);
void gi_on_mw (void*, int x, int y);

#endif
