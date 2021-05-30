#ifndef GAME_H
#define GAME_H
#include <sudoku3/sudoku.h>
#include <sudoku3/sudoku_gi.h>
ushort_t* game_init_sol(enum SUDOKU_SIZE sz, size_t fill_sz, size_t vis_ct, unsigned int rsd, gi_ar_t *piar);
#endif
