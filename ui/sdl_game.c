#include <assert.h>
#include <stdio.h>
#include <sudoku3/ftype.h>
#include <sudoku3/av.h>
#include <sudoku3/settings.h>
#include <sudoku3/sudoku.h>
#include <sudoku3/solver.h>
#include <sudoku3/ichg.h>
#include <sudoku3/sudoku_gi.h>

#include "gi.h"
#include "tbl.h"
#include "rect.h"
#include "colors.h"
#include "SDL_ttf.h"
#include "sdl_game.h"
#include "game.h"


#define FILL_FACTOR .03
#define VIS_FACTOR .75

static TTF_Font * sdl_game_load_font();
static void sdl_game_render_sudoku();

static gi_t sudoku_gi;

static SDL_Rect* s_rects1 = 0;
static SDL_Rect* s_rects2 = 0;
static SDL_Rect** s_rav1 = 0;
static SDL_Rect* s_rav1d = 0;
static ushort s_rav1ct = 0;

static game_t *game;
static SDL_Texture* *numtx = 0;
static SDL_Renderer* renderer = 0;
static TTF_Font* font = 0;

SDL_Texture* sdl_game_numtx(ushort n) {
 if (game && game->tbl->sz >= n && n > 0 && numtx != NULL) {
	return numtx[n-1];
 }
 return NULL;
}

static void sdl_game_prepare_numtx() {
	numtx = calloc(game->tbl->sz, sizeof(SDL_Texture*));
	unsigned char* numclr = colors_get(NUM);
	SDL_Color clr = { *numclr++, *numclr++, *numclr++, *numclr++ };
	ushort sz = game->tbl->sz;
	while(sz--){
	 char no[3];
	 int s = snprintf(no, 3, "%hd", sz+1);

	 assert(s);
	 SDL_Surface *kds = TTF_RenderUTF8_Blended(font, no, clr);
	 assert(kds);
	 SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, kds);
	 assert(t);
	 *(numtx + sz) = t;
	 SDL_FreeSurface(kds);
	}
}

static void sdl_game_free_numtx() {
	ushort sz = game->tbl->sz;
	if (numtx) {
		while(sz--) SDL_DestroyTexture(numtx[sz]);
		free(numtx);
	}
	numtx = 0;
}

void sdl_game(game_t *p, SDL_Renderer* r, unsigned int *prsd, float vf) {
	renderer = r;
	if (game) {
		sdl_game_free_numtx();
		if (s_rav1) free(s_rav1); s_rav1 = 0;
		if (s_rav1d) free(s_rav1d); s_rav1d = 0;
		s_rav1ct = 0;
	}
	game = p;
	if (!font && game) {
		font = sdl_game_load_font();
		assert(font);
	}
	if (game)
		sdl_game_prepare_numtx();

	if (game) {
	   unsigned int rsd = 1;
	   if (prsd) rsd = *prsd;
	   gi_ar_t iar;
	   if(game_init_sol (game->tbl->sz,
			     (int)(game->tbl->ct * FILL_FACTOR),
			     (int)(game->tbl->ct * vf),
			     rsd, &iar))
		sudoku_gi = sudoku_gi_from(game->tbl->sz, &iar);
       }
}

SDL_Rect* sdl_game_get_rects(enum RECTS rcts) {
	if (rcts > RECTS_I) {
		int idx;
		enum RECTS rc;
		if (rcts < RECTS_OS)
			idx = game->tbl->hover;
		else if (rcts < RECTS_OAV)
			 idx = game->tbl->selected;
		SDL_Rect* nrects;
		rc = rcts % 2;
		if (rcts < RECTS_OAV) {
 		  if (idx >=0 && idx < game->tbl->ct) {
		    nrects = sdl_game_get_rects(rc) + idx;
		    return nrects;
		  }
		} else {
		    nrects = s_rav1d;
		    return nrects;
		}
		return NULL;
	}
	SDL_Rect *rects;
	rect_t   *tblr;
	SDL_Rect** s2;
	if (rcts == RECTS_O) {
		rects = s_rects1;
		s2 = & s_rects1;
		tblr = game->tbl->osq;
	} else {
		rects = s_rects2;
		s2 = & s_rects2;
		tblr = game->tbl->isq;
	}
	if (!rects) {
		rects = calloc(game->tbl->ct, sizeof(SDL_Rect));
		ushort ct = game->tbl->ct;
		while(ct--) {
			SDL_Rect *sr = rects + ct;
			rect_t *tr = tblr + ct;
			sr->x = tr->x;
			sr->y = tr->y;
			sr->w = tr->w;
			sr->h = tr->h;
		}
		*s2 = rects;
	}
	return rects;
}

void sdl_game_draw_rects(SDL_Renderer * renderer) {
	unsigned char* clr;
	SDL_Rect* r;

	clr = colors_get(CLEAR_BG);
	SDL_SetRenderDrawColor(renderer,
			*clr++, *clr++, *clr++, *clr);
	SDL_RenderClear(renderer);

	clr = colors_get(BG);
	SDL_SetRenderDrawColor(renderer,
			*clr++, *clr++, *clr++, *clr);
	r = sdl_game_get_rects(RECTS_O);
	SDL_RenderFillRects(renderer, r, game->tbl->ct);

	if (s_rav1ct) {
	clr = colors_get(AV1);
	SDL_SetRenderDrawColor(renderer,
			*clr++, *clr++, *clr++, *clr);
	r = sdl_game_get_rects(RECTS_OAV);
	SDL_RenderFillRects(renderer, r, s_rav1ct);
        }

	clr = colors_get(SQ);
	SDL_SetRenderDrawColor(renderer,
			*clr++, *clr++, *clr++, *clr);
	r = sdl_game_get_rects(RECTS_I);
	SDL_RenderFillRects(renderer, r, game->tbl->ct);


	clr = colors_get(HOVER);
	SDL_SetRenderDrawColor(renderer,
			*clr++, *clr++, *clr++, *clr);
	r = sdl_game_get_rects(RECTS_OH);
	if (r)
		SDL_RenderFillRect(renderer, r);

	clr = colors_get(SQ);
	SDL_SetRenderDrawColor(renderer,
			*clr++, *clr++, *clr++, *clr);
	r = sdl_game_get_rects(RECTS_IH);
	if (r)
		SDL_RenderFillRect(renderer, r);

	clr = colors_get(SELECTED);
	SDL_SetRenderDrawColor(renderer,
			*clr++, *clr++, *clr++, *clr);
	r = sdl_game_get_rects(RECTS_OS);
	if (r)
		SDL_RenderFillRect(renderer, r);

	clr = colors_get(SQ);
	SDL_SetRenderDrawColor(renderer,
			*clr++, *clr++, *clr++, *clr);
	r = sdl_game_get_rects(RECTS_IS);
	if (r)
		SDL_RenderFillRect(renderer, r);

	sdl_game_render_sudoku();
}

typedef int (*draw_rects)
	(SDL_Surface*, const SDL_Rect*, int, unsigned int);
typedef int (*draw_rect)
	(SDL_Surface*, const SDL_Rect*, unsigned int);

void sdl_game_surface(SDL_Surface *surface) {
		unsigned char* clr;
		unsigned int color;
		ushort ct;
		SDL_Rect* r;
		draw_rect dr;
		draw_rects drs;

		dr = SDL_FillRect;
		drs = SDL_FillRects;
		ct = game->tbl->ct;

		clr = colors_get(CLEAR_BG);
		color = SDL_MapRGB(surface->format, *clr++, *clr++, *clr++);
		dr(surface, NULL, color);

		clr = colors_get(BG);
		color = SDL_MapRGB(surface->format, *clr++, *clr++, *clr++);
		r = sdl_game_get_rects(RECTS_O);
		drs(surface, r, ct, color);

		clr = colors_get(SQ);
		color = SDL_MapRGB(surface->format, *clr++, *clr++, *clr++);
		r = sdl_game_get_rects(RECTS_I);
		drs(surface, r, ct, color);

		clr = colors_get(HOVER);
		color = SDL_MapRGB(surface->format, *clr++, *clr++, *clr++);
		r = sdl_game_get_rects(RECTS_OH);
		if (r)   dr(surface, r, color);

		clr = colors_get(SQ);
		color = SDL_MapRGB(surface->format, *clr++, *clr++, *clr++);
		r = sdl_game_get_rects(RECTS_IH);
		if (r)   dr(surface, r, color);

		clr = colors_get(SELECTED);
		color = SDL_MapRGB(surface->format, *clr++, *clr++, *clr++);
		r = sdl_game_get_rects(RECTS_OS);
		if (r)   dr(surface, r, color);

		clr = colors_get(SQ);
		color = SDL_MapRGB(surface->format, *clr++, *clr++, *clr++);
		r = sdl_game_get_rects(RECTS_IS);
		if (r)   dr(surface, r, color);
}

static TTF_Font * sdl_game_load_font() {
	char *p, *fname;

	fname = SQFONT;
	const size_t size = SQFONTSZ;
	TTF_Font *font;
	/* start SDL_ttf */
	if(TTF_Init()==-1)
	{
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(2);
	}
	font=TTF_OpenFont(fname, size);
	if(!font)
	{
		printf("TTF_OpenFont: %s\n", TTF_GetError());
		exit(3);
	}
/*
	printf("size                    : %ld\n",size);
	printf("TTF_FontHeight          : %d\n",TTF_FontHeight(font));
	printf("TTF_FontAscent          : %d\n",TTF_FontAscent(font));
	printf("TTF_FontDescent         : %d\n",TTF_FontDescent(font));
	printf("TTF_FontLineSkip        : %d\n",TTF_FontLineSkip(font));
	printf("TTF_FontFaceIsFixedWidth: %d\n",TTF_FontFaceIsFixedWidth(font));
*/
	{
		char *str=TTF_FontFaceFamilyName(font);
		if(!str)
			str="(null)";
		printf("TTF_FontFaceFamilyName  : \"%s\"\n",str);
	}
	{
		char *str=TTF_FontFaceStyleName(font);
		if(!str)
			str="(null)";
		printf("TTF_FontFaceStyleName   : \"%s\"\n",str);
	}
	if(TTF_GlyphIsProvided(font,'g'))
	{
		int minx, maxx, miny, maxy, advance;
		TTF_GlyphMetrics(font,'g', &minx, &maxx, &miny, &maxy, &advance);
		printf("TTF_GlyphMetrics('g'):\n\tminx=%d\n\tmaxx=%d\n\tminy=%d\n\tmaxy=%d\n\tadvance=%d\n",
				minx, maxx, miny, maxy, advance);
	}
	else
		printf("TTF_GlyphMetrics('g'): unavailable in font!\n");

	/* set window title and icon name, using filename and stuff */
	p=strrchr(fname,'/');
	if(!p)
		p=strrchr(fname,'\\');
	if(!p)
		p=strrchr(fname,':');
	if(!p)
		p=fname;
	else
		p++;

	return font;
}

static void sdl_game_render_sudoku() {
	if (!game) return;
	sudoku_t *sudoku = sudoku_gi.sudoku;
	ushort ct = sudoku->ct;
	ushort i,j,t;
	for(i=0,t=0;i<sudoku->sz;i++) {
	 for(j=0;j<sudoku->sz;j++,t++) {
	   ushort num=(sudoku->ar + t)->cv;
	   SDL_Texture *tn = sdl_game_numtx(num);
	   if (tn) {
		int w, h, ac;
		unsigned int pxf;
		SDL_QueryTexture(tn, &pxf, &ac, &w, &h);
		rect_t* s = game->tbl->osq + t;
		SDL_Rect r = {.x=s->x, .y = s->y, .w = s->w, .h = s->h };
		SDL_Rect tgt = r;
		tgt.x += 5;
		tgt.y += 5;
		tgt.w -= 10;
		tgt.h -= 10;
       	if (SDL_RenderCopy(renderer, tn, NULL, &tgt)) {
 		  SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
		 "Couldn't cpy number texture %hd: %s\n", num, SDL_GetError() );
		}
	   }
	 }
       }
}

void sdl_game_highlight_av1() {
	ushort_t *es;
	es = calloc(sudoku_gi.sudoku->ct, sizeof(ushort));
	ushort_t ct, idx;
	idx = 0;
	ct = sudoku_gi.sudoku->ct;
	while(ct--) {
	  av_t * a = sudoku_gi.sudoku->ar + ct;
	  if (a->cv == 0 && 1 == ft_list_set_ct(&(a->av), sudoku_gi.sudoku->sz)) {
		es[idx++] = ct;
	  }
	}

	if (idx != game->tbl->esct || memcmp(es, game->tbl->esidx, sizeof(ushort) * sudoku_gi.sudoku->ct)) {
	   memcpy(game->tbl->esidx, es, sizeof(ushort) * sudoku_gi.sudoku->ct);
	   game->tbl->esct = idx;
	   s_rav1ct = idx;
	   if(s_rav1) free(s_rav1);
	   s_rav1 = calloc(idx, sizeof(SDL_Rect*));
	   while(idx--) {
		s_rav1[idx] = s_rects1 + es[idx];
	   }
	   if (s_rav1d) free(s_rav1d);
	   s_rav1d = calloc(s_rav1ct, sizeof(SDL_Rect));
	   idx = s_rav1ct;
	   while(idx--)
		s_rav1d[idx] = *(s_rav1[idx]);
	}
	free(es);
}

void sdl_game_selected_num(ushort num) {
	if (game && game->tbl->selected >= 0 && game->tbl->selected < game->tbl->ct) {
	    ushort idx = game->tbl->selected;
	    av_t* ar = sudoku_gi.sudoku->ar;
	    av_t* a = ar + idx;
	    if (num == 0 || num != a->cv)
		av_unseti(ar, idx);
	    if (num)
		av_seti(ar, num, idx);
	    if (sudoku_is_solution(sudoku_gi.sudoku))
		colors_bg_solved();
	    else if (sudoku_is_correct(sudoku_gi.sudoku)) {
		colors_bg_correct();
	    }
	    else colors_bg_incorrect();
            sdl_game_highlight_av1();
	}
}
