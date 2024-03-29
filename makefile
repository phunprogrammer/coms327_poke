CXX=g++
CXXFLAGS=-I$(IDIR) -Wall -Wextra -Werror -g -lncurses

IDIR=./include
ODIR=./obj
SDIR=./src

LIBS=-lm

_DEPS = AStar.h Config.h NoiseGenerator.h PerlinNoise.h PPMGenerator.h PQueue.h TerrainGenerator.h EntityGenerator.h EntityMover.h InputController.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o AStar.o Config.o NoiseGenerator.o PerlinNoise.o PPMGenerator.o PQueue.o TerrainGenerator.o EntityGenerator.o EntityMover.o InputController.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS) | $(ODIR)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

game: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

$(ODIR):
	mkdir -p $(ODIR)

.PHONY: clean

clean:
	rm -fr $(ODIR)/*.o *~ core $(INCDIR)/*~ 
	rmdir $(ODIR)
	rm -f ./game
	rm -f ./map.png
	rm -f ./map.ppm