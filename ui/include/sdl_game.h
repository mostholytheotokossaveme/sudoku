#ifndef SDL_GAME_H
#define SDL_GAME_H
#include "SDL.h"
#include "tbl.h"
enum RECTS {
  RECTS_O, RECTS_I,
  RECTS_OH, RECTS_IH,
  RECTS_OS, RECTS_IS,
  RECTS_OAV, RECTS_IAV,
  RECTS_S
};

typedef struct {
	void* gi;
	tbl_t* tbl;
} game_t;

void sdl_game(game_t *p, SDL_Renderer* r, unsigned int *prsd, float vf);
void sdl_game_highlight_av1();
void sdl_game_selected_num(ushort num);
/*SDL_Texture* sdl_game_numtx(ushort n);*/

SDL_Rect* sdl_game_get_rects(enum RECTS rcts);

void sdl_game_surface(SDL_Surface *surface);

void sdl_game_draw_rects(SDL_Renderer * renderer);
#endif
