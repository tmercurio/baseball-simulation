# File Name: Makefile
# Author: Thomas Mercurio
# Email: tmercuri@nd.edu
# This is the Makefile for the baseball simulation project.

PP := g++

# Flags used in compiling C++
FlAGS := -O2 -g -Wall -Wextra -Wconversion -Wshadow -pedantic -Werror -lm
CXXFLAGS := -m64 -std=c++11 -Weffc++ $(FLAGS)

# Directory variables
INC := inc
SRC := src
OBJ := obj
EXE := exe

initialize:
	rm -rf $(OBJ) $(EXE)
	mkdir $(OBJ) $(EXE)

$(OBJ)/Team.o : $(SRC)/Team.cpp
	$(PP) $(CXXFLAGS) -c $(SRC)/Team.cpp -o $@

$(OBJ)/Player.o : $(SRC)/Player.cpp
	$(PP) $(CXXFLAGS) -c $(SRC)/Player.cpp -o $@

$(OBJ)/simulation.o : $(SRC)/simulation.cpp
	$(PP) $(CXXFLAGS) -c $(SRC)/simulation.cpp -o $@

sim_obj := $(OBJ)/Team.o $(OBJ)/Player.o $(OBJ)/simulation.o

# Executable
simulation : $(sim_obj)
	$(PP) $(CXXFLAGS) $(sim_obj) -o $(EXE)/simulation


clean:
	rm -rf *.o $(OBJ)/* $(EXE)/*
