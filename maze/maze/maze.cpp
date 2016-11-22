#if defined(_WIN32)
#pragma comment(lib, "glfw3dll.lib")
#endif

#include <iostream>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "linmath.h"
#include "CShader.h"
#include "CBlock.h"
#include "CUI.h"

using namespace std;

int main(int argc, char *argv[])
{
	CShader *shader;
	CBlock block, block2, block3, block4;
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

	shader->Load();

	block4.Transform(0.0f, 0.0f, -0.5f);
	block3.Transform(0.0f, -0.5f, 0.0f);
	block2.Transform(-0.5f, 0.0f, 0.0f);

	block4.Load();
	block3.Load();
	block2.Load();
	block.Load();

	shader->Map();

	ui.AddObject(&block);
	ui.AddObject(&block2);
	ui.AddObject(&block3);
	ui.AddObject(&block4);

	ui.Run();

	ui.DelObject(&block);
	ui.DelObject(&block2);
	ui.DelObject(&block3);
	ui.DelObject(&block4);

	shader->Unload();

	shader->Destroy();

	status = ui.DestroyContext();

	return 0;
}

/* End of a file */
