#pragma comment(lib, "glfw3dll.lib")

#include <iostream>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "linmath.h"
#include "CShader.h"
#include "CUI.h"

using namespace std;

int main(int argc, char *argv[])
{
	CShader *shader;
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

	shader->Map();

	ui.Run();

	shader->Unload();

	shader->Destroy();

	status = ui.DestroyContext();

	return 0;
}

/* End of a file */
