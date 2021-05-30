#include <assert.h>
#include "GLES3/gl3.h"
#include "SDL.h"
#include "colors.h"
#include "texture.h"
#include "gi.h"
#include "sdl_game.h"
#include "SDL_ttf.h"

enum {
  TEX_A, TEX_B, TEX_C, TEX_D,
  TEX_E, TEX_F, TEX_G, TEX_H
};

static GLuint *tx_names = 0;
static texture_t** textures;
static int _w = 600, _h=600;
static game_t game;
static unsigned int rsd;
static char gminit = 0;
static float vfactor = 0.7;

static void initgl(void) {
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
}

static texture_t* loadtx(int argc, char *argv[]) {
    texture_t* tex;

    textures = (texture_t**) calloc(argc, sizeof(texture_t*));
    textures[TEX_A] = texture_from_file("ChristPantocrator.jpg");
    tx_names = calloc(argc, sizeof(GLuint));
    int idx;
    for(idx=1;idx<argc;idx++) {
	textures[idx] = texture_from_file(argv[idx]);
    }
    tex = textures[TEX_A];
    return tex;
}

int main(int argc, char *argv[])
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *surface;
    SDL_Surface *sq_surface;
    SDL_Texture *texture;
    SDL_Texture *sq_texture;
    SDL_Event *event;
    SDL_GLContext glcontext;
    SDL_DisplayMode displaymode;
    TTF_Font *font=0;
    displaymode = (SDL_DisplayMode) {
       .format = SDL_PIXELFORMAT_RGBA32,
       .w = _w,
       .h = _h,
       .refresh_rate = 0,
       .driverdata = 0
    };
    event = malloc(sizeof(SDL_Event));
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }

    initgl();

    if (SDL_CreateWindowAndRenderer(_w, _h, SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    }

    if (!window) {
	SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    }

    if (SDL_SetWindowDisplayMode(window, &displaymode)) {
	SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
    }
   /* glcontext = SDL_GL_CreateContext(window); */
   /*
    texture_t* tex = loadtx(argc, argv);
    surface = SDL_CreateRGBSurfaceWithFormatFrom((void*)tex->tx, tex->w, tex->h,
                                                       tex->dpt, tex->rs, SDL_PIXELFORMAT_RGB24
    if (!surface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
        return 3;
    }
   */

    sq_surface = SDL_CreateRGBSurface(0, _w, _h, 32, 0, 0, 0, 0);
    /*
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
        return 3;
    }
    SDL_FreeSurface(surface);
    */

    char c;
    c = 1;
    char hint = 1;

    game.gi = gi_open(3,3,_w, _h,9,4);
    game.tbl = gi_tbl(game.gi);

    sq_texture = SDL_CreateTexture(renderer,
	SDL_PIXELFORMAT_RGBA32,
	SDL_TEXTUREACCESS_TARGET, _w, _h);
    int sz = game.tbl->sz;
    while (c) {
	if (!gminit && game.tbl->selected >=0) rsd += game.tbl->selected;
	SDL_RenderClear(renderer);
	SDL_SetRenderTarget(renderer, sq_texture);
	if (gminit) {
	  sdl_game_draw_rects(renderer);
	  if (hint)    sdl_game_highlight_av1();
	  hint = 0;
	}
	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, sq_texture, NULL, NULL);
	SDL_RenderPresent(renderer);

        if (SDL_WaitEventTimeout(event, 80)) {
			const int type = event->type;
			const int sym = event->key.keysym.sym;
			const int num = event->key.keysym.mod & KMOD_NUM;
			const int ctrl = event->key.keysym.mod & KMOD_CTRL;
			const int alt = event->key.keysym.mod & KMOD_ALT;
            if (type == SDL_QUIT) {c = 0;continue;    }
            if (type == SDL_KEYDOWN && gminit) {
				if (ctrl) {
					if (sym == SDLK_n) {
						rsd++;
						sdl_game(&game, renderer, &rsd, vfactor);
						colors_bg_correct();
					}
					else if (sym == SDLK_m)
						if (vfactor < .91)
							vfactor += 0.05;
						else if (sym == SDLK_k)
							if (vfactor > .14)
								vfactor -= 0.05;
					continue;
				}
				if (alt) {
					if (sym == SDLK_m)
						if (vfactor > .14)
							vfactor -= 0.05;
				}
            if (sym >= SDLK_0 && sym <= SDLK_9) {
		ushort num=0;
		switch(sym){
			case SDLK_9: num++;
			case SDLK_8: num++;
			case SDLK_7: num++;
			case SDLK_6: num++;
			case SDLK_5: num++;
			case SDLK_4: num++;
			case SDLK_3: num++;
			case SDLK_2: num++;
			case SDLK_1: num++;
		}
		sdl_game_selected_num(num);
		gi_on_kbd(game.gi, sym); continue;
	    }
	    else if (sym >= SDLK_KP_1 && sym <= SDLK_KP_0) {
               if (!num) {
                 enum GI_MV mv;
		 enum GI_MV *pmv;
		 pmv = 0;
		      if (sym == SDLK_KP_4) { mv = GI_LEFT;  pmv = &mv; }
		 else if (sym == SDLK_KP_8) { mv = GI_UP;    pmv = &mv; }
		 else if (sym == SDLK_KP_2) { mv = GI_DOWN;  pmv = &mv; }
		 else if (sym == SDLK_KP_6) { mv = GI_RIGHT; pmv = &mv; }
		 if (pmv) gi_on_mv(game.gi, *pmv);
               } else {
	         gi_on_kbd(game.gi, sym);
               }
	       continue;
            }
	    else if (sym == SDLK_RETURN ||  sym == SDLK_SPACE || sym == SDLK_KP_ENTER) {
		     gi_on_confirm(game.gi, sym);
            }
	    switch(sym) {
		case SDLK_q: {
		   c = 0;
		   continue;
		};
                case SDLK_s:
		case SDLK_DOWN:{
		   gi_on_mv(game.gi, GI_DOWN);
		   continue;
		};
                case SDLK_w:
		case SDLK_UP:{
		   gi_on_mv(game.gi, GI_UP);
		   continue;
		};
		case SDLK_a:
		case SDLK_LEFT:{
		   gi_on_mv(game.gi, GI_LEFT);
		   continue;
		};
		case SDLK_d:
		case SDLK_RIGHT: {
		   gi_on_mv(game.gi, GI_RIGHT);
		   continue;
		};
	     };
            } else if (event->type == SDL_MOUSEMOTION) {
		if (!gminit)
			rsd += 13*event->motion.x +
			 11*event->motion.y +
			 ((event->motion.x+event->motion.y) % 17) - (rsd / (event->motion.x*event->motion.y));
		else
		gi_on_mo(game.gi, event->motion.x, event->motion.y);
            } else if (event->type == SDL_MOUSEBUTTONDOWN) {
		if (!gminit)
			rsd += 13*event->motion.x +
			 11*event->motion.y +
			 ((event->motion.x+event->motion.y) % 17) - (rsd / (event->motion.x*event->motion.y));
		else
		gi_on_md(game.gi, event->button.x, event->button.y);
            } else if (event->type == SDL_MOUSEBUTTONUP) {
		if (!gminit)
			rsd += 13*event->motion.x +
			 11*event->motion.y +
			 ((event->motion.x+event->motion.y) % 17) - (rsd / (event->motion.x*event->motion.y));
		else
		gi_on_mu(game.gi, event->button.x, event->button.y);
	    } else if (event->type == SDL_MOUSEWHEEL) {
		if (!gminit)
			rsd += 13*event->motion.x +
			 11*event->motion.y +
			 ((event->motion.x+event->motion.y) % 17) - (rsd / (event->motion.x*event->motion.y));
		else
		gi_on_mw(game.gi, event->button.x, event->button.y);
	    }
	    if (!gminit) {
		gminit=1;
		printf("sdl_game %d ...\n", rsd);
  	        sdl_game(&game, renderer, &rsd, vfactor);
	    }
	}
    }

    SDL_DestroyTexture(sq_texture);
    SDL_DestroyRenderer(renderer);
    /*SDL_GL_DeleteContext(glcontext);*/
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
