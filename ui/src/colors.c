#include "colors.h"

static const unsigned char COLOR_WHITE[4] = {0xFF, 0xFF, 0xFF, 0xFF};
static const unsigned char COLOR_GRAY[4] = {0xAA, 0xAA, 0xAA, 0xAA};
static const unsigned char COLOR_GREEN[4] = {0x0, 0x0, 0xFF, 0xFF};
static const unsigned char COLOR_BLUE[4] = {0, 0xFF, 0, 0xFF};
static const unsigned char COLOR_RED[4] = {0xFF, 0, 0, 0xFF};
static const unsigned char COLOR_PURPLE[4] = {0xFF, 0, 0xFF, 0xFF};
static const unsigned char COLOR_YELLOW[4] = {0xFF, 0xFF, 0, 0xFF};
static const unsigned char COLOR_CYAN[4] = {0, 0xFF, 0xFF, 0xFF};
static const unsigned char CE1[4] = {0xf3, 0xd5, 0xdd, 0xff};
static const unsigned char CE2[4] = {0xf3, 0xd5, 0xec, 0xff};
static const unsigned char CE3[4] = {0xd5, 0xf3, 0xeb, 0xff};
static const unsigned char CE4[4] = {0xd5, 0xdd, 0xf3, 0xff};
static const unsigned char CE5[4] = {0,0,0,0};
static const unsigned char* CCH[5] = { CE5, COLOR_GRAY, CE3, CE4, COLOR_GRAY };

unsigned char* colors_get(enum CLRT c) {
	return CCH[c];
}
