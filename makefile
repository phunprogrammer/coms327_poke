IDIR =./include
CC=gcc
CFLAGS=-I$(IDIR) -Wall -Wextra -Werror -g

ODIR=./obj
SDIR=./src

LIBS=-lm

_DEPS = AStar.h Config.h NoiseGenerator.h PerlinNoise.h PPMGenerator.h PQueue.h TerrainGenerator.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o AStar.o Config.o NoiseGenerator.o PerlinNoise.o PPMGenerator.o PQueue.o TerrainGenerator.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

game: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -fr $(ODIR)/*.o *~ core $(INCDIR)/*~ 
	rm -f ./game