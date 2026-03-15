
GXX = gcc
FLAGS = -Wall -Wextra -g
LIB = Main.c $(shell dir /s/b lib\*.c) $(shell dir /s/b hashing\*.c)
INCLUDE = -I include -I hashing
DLLS= -lws2_32
EXE = Main.exe

build:
	$(GXX) $(FLAGS) $(LIB) $(INCLUDE) $(DLLS) -o $(EXE)

