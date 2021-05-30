SQFONT := '"./res/FreeSerifBold.ttf"'
SQFONTSZ := 96

LIBSDL := `sdl2-config --cflags --libs` -lSDL2_ttf -lsudoku
CFLAGS := -I ./include -I ../include -g -lGL -lGLU -lglut -ljpeg -DSQFONT=$(SQFONT) -DSQFONTSZ=$(SQFONTSZ)
OBJ := colors.o j.o rect.o tbl.o gi.o texture.o game.o
DEP := include/colors.h include/rect.h include/j.h include/tbl.h include/gi.h include/texture.h
all :
	make sdlui -B


colors.o : $(DEP)
j.o : $(DEP)
rect.o : $(DEP)
gi.o : $(DEP)
texture.o : $(DEPS)
game.o : game.c
	cc -o game.o -c game.c $(CFLAGS) $(LIBSDL)
sdl_game.o : $(OBJ)
	cc -o sdl_game.o -c sdl_game.c $(CFLAGS) $(LIBSDL)

sdlui : $(OBJ) sdl_game.o
	cc -o sdlui sdlui.c $(OBJ) sdl_game.o $(CFLAGS) $(LIBSDL)

clean :
	rm *.o sdlui
