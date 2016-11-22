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
	CBlock block;
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

	block.Load();

	shader->Map();

	ui.AddObject(&block);

	ui.Run();

	ui.DelObject(&block);

	shader->Unload();

	shader->Destroy();

	status = ui.DestroyContext();

	return 0;
}

/* End of a file */
