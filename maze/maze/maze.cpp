#if defined(_WIN32)
#pragma comment(lib, "glfw3dll.lib")
#endif

#include <iostream>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "cgmath.h"
#include "CShader.h"

#include "CVertices.h"
#include "CObject.h"
#include "CMovable.h"
#include "CBlock.h"
#include "CPlayer.h"
#include "CCoordinate.h"

#include "CUI.h"

using namespace std;

int main(int argc, char *argv[])
{
	CShader *shader;
	CVertices *vertices;
	CPlayer *player;
	CBlock *block;
	CCoordinate *coord;
	CUI ui;
	int status;

	srand((unsigned int)time(NULL));

	status = ui.CreateContext();
	if (status < 0)
		return status;

	shader = CShader::GetInstance();
	if (!shader) {
		ui.DestroyContext();
		return -EFAULT;
	}

	vertices = CVertices::GetInstance();
	if (!vertices) {
		shader->Destroy();
		ui.DestroyContext();
		return -EFAULT;
	}

	player = CPlayer::GetInstance();
	if (!player) {
		vertices->Destroy();
		shader->Destroy();
		ui.DestroyContext();
		return -EFAULT;
	}

	block = CBlock::GetInstance();
	if (!block) {
		player->Destroy();
		vertices->Destroy();
		shader->Destroy();
		ui.DestroyContext();
		return -EFAULT;
	}

	coord = CCoordinate::GetInstance();
	if (!coord) {
		block->Destroy();
		player->Destroy();
		vertices->Destroy();
		shader->Destroy();
		ui.DestroyContext();
		return -EFAULT;
	}

	coord->Load();
	shader->Load();
	vertices->Load();
	block->Load();
	player->Load();

	shader->Map();

	ui.AddObject(block);
	ui.AddObject(player);
	ui.AddObject(coord);

	ui.Run();

	ui.DelObject(coord);
	ui.DelObject(player);
	ui.DelObject(block);

	coord->Destroy();
	player->Destroy();
	block->Destroy();
	vertices->Destroy();

	shader->Unload();
	shader->Destroy();

	status = ui.DestroyContext();

	return 0;
}

/* End of a file */
