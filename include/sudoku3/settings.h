#ifndef SETTINGS_H
#define SETTINGS_H 1
#include <sudoku3/ftype.h>
struct settings {
  ushort_t sz;
  ushort_t sz_sqrt;
} settings;

typedef struct settings settings_t;

void settings_set(settings_t*);
settings_t* settings_get();
#endif
