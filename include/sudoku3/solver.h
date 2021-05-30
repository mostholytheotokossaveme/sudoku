#ifndef SOLVER_H
#define SOLVER_H
#include <sudoku3/ftype.h>
#include <sudoku3/av.h>
#include <sudoku3/sudoku.h>
#include <sudoku3/solver_input.h>

struct solver {
 sudoku_t* sudoku;
 input_t* input;
} solver;
typedef struct solver solver_t;

ushort_t solve(solver_t* slvr);
ushort_t* solve_input(sudoku_t*, input_t*);
#endif
