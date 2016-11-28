CFLAGS=-g
CFLAGS+=-I.
CFLAGS+=-std=c++11
all: CTexture.cpp glad.c maze.cpp CMisc.cpp CCoordinate.cpp CEnvironment.cpp CModel.cpp CObject.cpp CPerspective.cpp CPlayer.cpp CVertices.cpp CView.cpp State.cpp maze.cpp CBlock.cpp CShader.cpp CUI.cpp stb_image.h stb_image.c
	@g++ -Wall -Werror ${CFLAGS} `pkg-config glfw3 --cflags --libs` -ldl glad.c CCoordinate.cpp CEnvironment.cpp CModel.cpp CObject.cpp CPerspective.cpp CPlayer.cpp CVertices.cpp CView.cpp State.cpp maze.cpp CBlock.cpp CShader.cpp CUI.cpp CMisc.cpp CTexture.cpp stb_image.c -o maze

