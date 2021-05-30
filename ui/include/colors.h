#ifndef COLORS_H
#define COLORS_H 1

enum CLRT {
	CLEAR_BG,
	BG,
	SQ,
	HOVER,
	SELECTED,
	NUM,
	AV1
};
void colors_bg_incorrect();
void colors_bg_correct();
void colors_bg_solved();
unsigned char* colors_get(enum CLRT c);
#endif
