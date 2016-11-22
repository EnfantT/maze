CFLAGS=-g
CFLAGS+=-I.
CFLAGS+=-std=c++11
all: glad.c maze.cpp CShader.cpp CUI.cpp Player.cpp State.cpp CBlock.cpp CObject.cpp
	@g++ -Wall -Werror ${CFLAGS} `pkg-config glfw3 --cflags --libs` -ldl CObject.cpp CBlock.cpp glad.c maze.cpp CShader.cpp CUI.cpp Player.cpp State.cpp -o maze
