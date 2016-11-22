CFLAGS=-g
CFLAGS+=-I.
all: glad.c maze.cpp CShader.cpp CUI.cpp Player.cpp State.cpp
	@g++ -Wall -Werror ${CFLAGS} `pkg-config glfw3 --cflags --libs` -ldl glad.c maze.cpp CShader.cpp CUI.cpp Player.cpp State.cpp -o maze
