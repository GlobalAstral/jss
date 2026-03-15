
GXX = gcc
FLAGS = -Wall -Wextra -g
LIB = Main.c $(shell dir /s/b lib\*.c) $(shell dir /s/b hashing\*.c) $(shell dir /s/b listing\*.c)
INCLUDE = -I include -I hashing -I listing
DLLS= -lws2_32
EXE = Main.exe

build:
	$(GXX) $(FLAGS) $(LIB) $(INCLUDE) $(DLLS) -o $(EXE)

