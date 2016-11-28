CFLAGS=-g
CFLAGS+=-I.
CFLAGS+=-D_OLD_GL
CFLAGS+=-std=c++11
all: glad.c maze.cpp CCoordinate.cpp CEnvironment.cpp CModel.cpp CObject.cpp CPerspective.cpp CPlayer.cpp CVertices.cpp CView.cpp State.cpp maze.cpp CBlock.cpp CShader.cpp CUI.cpp
	@g++ -Wall -Werror ${CFLAGS} `pkg-config glfw3 --cflags --libs` -ldl glad.c CCoordinate.cpp CEnvironment.cpp CModel.cpp CObject.cpp CPerspective.cpp CPlayer.cpp CVertices.cpp CView.cpp State.cpp maze.cpp CBlock.cpp CShader.cpp CUI.cpp -o maze

