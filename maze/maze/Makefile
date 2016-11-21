CFLAGS=-g
CFLAGS+=-I.
all: glad.c maze.cpp
	@g++ ${CFLAGS} `pkg-config glfw3 --cflags --libs` -ldl glad.c maze.cpp -o maze
