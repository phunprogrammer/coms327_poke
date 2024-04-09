INC_DIR := include
SRC_DIR := src
OBJ_DIR := obj
INC_DIRS := $(shell find $(INC_DIR) -type d)
SRC_DIRS := $(shell find $(SRC_DIR) -type d)
CC := g++
LINKS := -lm -lncurses
CFLAGS := $(addprefix -I, $(INC_DIRS)) -Wall -Wextra -g #-Werror
DEPS := $(wildcard $(addsuffix /*.h, $(INC_DIRS)))
SRCS := $(wildcard $(addsuffix /*.cpp, $(SRC_DIRS)))
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
TARGET := ./game

$(shell mkdir -p $(OBJ_DIR))
$(shell mkdir -p $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(SRC_DIRS)))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJS) $(DEPS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LINKS)

.PHONY: clean

clean:
	rm -fr $(OBJ_DIR)
	rm -f ./game
	rm -f ./map.png
	rm -f ./map.ppm