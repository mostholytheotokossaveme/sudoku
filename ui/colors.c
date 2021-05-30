#include "colors.h"

static void swap(unsigned char *a, unsigned char* b) {
   unsigned char t = *a;
   *a = *b;
   *b = t;
}

static unsigned char* flip(unsigned char *a) {
   unsigned char *f1 = a;
   unsigned char *f2 = a+1;
   unsigned char *f3 = a+2;
   swap(f1, f2);
   swap(f2, f3);
   return a;
}

static unsigned char COLOR_WHITE[4] = {0xFF, 0xFF, 0xFF, 0xFF};
static unsigned char COLOR_GRAY[4] =  {0xAA, 0xAA, 0xAA, 0xAA};
static unsigned char COLOR_GREEN[4] = {0x0, 0x0, 0xFF, 0xFF};
static unsigned char COLOR_BLUE[4] =  {0x0, 0xFF, 0x0, 0xFF};
static unsigned char COLOR_RED[4] = {0xFF, 0, 0, 0xFF};
static unsigned char COLOR_YELLOW[4] = {0xFF, 0, 0xFF, 0xFF};
static unsigned char COLOR_PURPLE[4] = {0xFF, 0xFF, 0, 0xFF};
static unsigned char COLOR_CYAN[4] = {0, 0xFF, 0xFF, 0xFF};
static unsigned char CE1[4] = {0xf3, 0xd5, 0xdd, 0xff};
static unsigned char CE2[4] = {0xf3, 0xd5, 0xec, 0xff};
static unsigned char CE3[4] = {0xd5, 0xf3, 0xeb, 0xff};
static unsigned char CE4[4] = {0xd5, 0xdd, 0xf3, 0xff};
static unsigned char CE6[4] = {237,248,251, 0xff};
static unsigned char CE7[4] = {178,226,226, 0xff};
static unsigned char CE8[4] = {102,194,164, 0xff};
static unsigned char CE8h[4] = {255,255,212, 0xff};
static unsigned char CE9[4] = {44,162,95,255};
static unsigned char CE10[4] = {0,109,44,255};
static unsigned char CE11[4] = {255,255,204,255};
static unsigned char CE5[4] = {0,0,0,0};
#define CCH_LEN 6
static unsigned char* CCH[CCH_LEN] = { (CE1), (CE2), (CE3), (COLOR_WHITE), (COLOR_YELLOW), CE5, COLOR_GREEN, COLOR_GREEN };

static unsigned char _init = 0;

static unsigned char* _CLEAR_BG;

void colors_bg_incorrect() {
	_CLEAR_BG = COLOR_RED;
}
void colors_bg_correct() {
	_CLEAR_BG = CE11;
}
void colors_bg_solved() {
	_CLEAR_BG = COLOR_GREEN;
}

unsigned char* colors_get(enum CLRT c) {
	if (!_init)
	{
		_init = 1;
		flip(CE4);flip(CE1);flip(CE2);flip(CE3);flip(CE5);flip(CE6);flip(CE7);flip(CE8);flip(CE9);
		flip(COLOR_CYAN);
		//flip(CE8h);
		flip(COLOR_WHITE);flip(COLOR_YELLOW);flip(COLOR_GREEN);flip(COLOR_RED);flip(COLOR_BLUE);
		colors_bg_correct();
	}
   switch(c) {
		case CLEAR_BG: return _CLEAR_BG;
		case BG: return CE6; /*return CE2;*/
		case SQ: return CE7;
		case HOVER: return CE8;
		case SELECTED: return CE9;
		case NUM: return CE11;
		case AV1: return COLOR_GREEN;
   }
   return COLOR_GRAY;
}
