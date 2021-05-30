LIB_VER_MAJ := 1
LIB_VER_MIN := 0.1
LIB_N := libsudoku.so
LIB_NAME := $(LIB_N).$(LIB_VER_MAJ)

IDIR := ../include
LDIR := ../lib
ODIR := ../obj
SDIR := ../src
TDIR := ../src/test
_DEP := sudoku3/ftype.h sudoku3/settings.h sudoku3/av.h sudoku3/sudoku.h sudoku3/ichg.h sudoku3/solver_input.h sudoku3/solver.h sudoku3/sudoku_gi.h
DEPS := $(addprefix $(IDIR)/,$(_DEP))
_OBJ := ftype.o settings.o av.o sudoku.o ichg.o solver_input.o solver.o sudoku_gi.o
OBJS := $(addprefix $(ODIR)/,$(_OBJ))
TSTS := $(addprefix $(TDIR)/,$(_OBJ) sgio.o)

CC := cc
LLIBS := -lm
CFLAGS := -std=c89 -g -fPIC -I$(IDIR) -L$(LDIR) $(LLIBS)

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -o $@ -c $< $(CFLAGS)
$(OBJS): | $(ODIR)
$(ODIR):
	mkdir $(ODIR)
clean:
	rm $(ODIR)/*.o

$(TDIR)/%.o: $(TDIR)/t%.c $(OBJS)
	$(CC) -o $@ -c $< $(OBJS) sgio.o $(CFLAGS)

$(TDIR)/%: $(TDIR)/%.o $(OBJS)
	$(CC) -o $@ $< $(OBJS) sgio.o $(CFLAGS)

tests: $(TSTS)

main: main.c
	$(CC) -o main main.c $(CFLAGS) -lsudoku


DEST := /usr/local/lib
libs: $(OBJS)
	$(CC) -shared -Wl,-soname,$(LIB_NAME) -o $(LIB_NAME).$(LIB_VER_MIN) $(OBJS) $(CFLAGS)
	install -C -m 755 $(LIB_NAME).$(LIB_VER_MIN) $(DEST)/
	ldconfig -n ./
	ln -s $(LIB_NAME) $(LIB_N)
	chown root:root *.so
	mv $(LIB_N) $(DEST)/
	mv $(LIB_NAME) $(DEST)/
