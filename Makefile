.RECIPEPREFIX := >
CXX=g++
CXXFLAGS=-std=c++17 -Wall $(shell sdl2-config --cflags)
LDFLAGS=$(shell sdl2-config --libs) -lSDL2_ttf -lGLEW -lGLU -lGL

SRC=src/main.cpp

space_combat: $(SRC)
>$(CXX) $(CXXFLAGS) -Iinclude $(SRC) -o $@ $(LDFLAGS)

clean:
>rm -f space_combat
