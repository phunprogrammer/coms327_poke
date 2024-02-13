CC=gcc
CFLAGS=-I$(IDIR) -Wall -Wextra -Werror -g

IDIR =./include
ODIR=./obj
SDIR=./src

LIBS=-lm

_DEPS = AStar.h Config.h NoiseGenerator.h PerlinNoise.h PPMGenerator.h PQueue.h TerrainGenerator.h EntityGenerator.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o AStar.o Config.o NoiseGenerator.o PerlinNoise.o PPMGenerator.o PQueue.o TerrainGenerator.o EntityGenerator.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS) | $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

game: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(ODIR):
	mkdir -p $(ODIR)

.PHONY: clean

clean:
	rm -fr $(ODIR)/*.o *~ core $(INCDIR)/*~ 
	rmdir $(ODIR)
	rm -f ./game
	rm -f ./map.png
	rm -f ./map.ppm