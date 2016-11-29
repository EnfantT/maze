/*
OpenGL Version	GLSL Version	#version tag
 1.2	none	    none
 2.0	1.10.59	    110
 2.1	1.20.8	    120
 3.0	1.30.10	    130
 3.1	1.40.08	    140
 3.2	1.50.11	    150
 3.3	3.30.6	    330
 4.0	4.00.9	    400
 4.1	4.10.6	    410
 4.2	4.20.6	    420
 4.3	4.30.6	    430
 4.5 ...
*/
#include <iostream>
#include <fstream>

#include <errno.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "cgmath.h"

#include "CMisc.h"
#include "CMovable.h"
#include "CView.h"
#include "CPerspective.h"
#include "CModel.h"
#include "CShader.h"

#if defined(_WIN32)
	// Define for windows
#else
	// Define for linux
#endif

using namespace std;

CShader *CShader::m_pInstance = NULL;

CShader::CShader()
	: m_program(0)
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
//	glEnable(GL_DEBUG_OUTPUT);
}

CShader::~CShader()
{
	if (m_program > 0) {
		glDeleteProgram(m_program);
		m_program = 0;
	}
}

void CShader::Destroy(void)
{
	m_pInstance = NULL;
	delete this;
}

CShader *CShader::GetInstance(void)
{
	if (m_pInstance == NULL) {
		try {
			m_pInstance = new CShader();
		}
		catch (...) {
			cerr << "Failed to create an instance of CShader" << endl;
			return NULL;
		}
	}

	return m_pInstance;
}

GLuint CShader::LoadNCompile(GLenum type, const char *filename)
{
	char *code;
	GLuint shader;
	GLint status;

	code = ReadFile(filename);
	if (!code)
		return 0u;

	cout << "begin" << endl;
	cout << code << endl;
	cout << "end" << endl;

	shader = glCreateShader(type);
	glShaderSource(shader, 1, &code, NULL);
	glCompileShader(shader);
	delete[] code;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		char buffer[256];
		int len;
		glGetShaderInfoLog(shader, sizeof(buffer) - 1, &len, buffer);
		buffer[len] = '\0';
		cerr << "Failed to compiler shader: " << buffer << endl;
	}

	return shader;
}

char *CShader::ReadFile(const char *filename)
{
	char *code = NULL;
	ifstream file;
	streampos size;

	file.open(filename, ios::ate);
	if (!file.is_open())
		return NULL;

	size = file.tellg();
	if (!size) {
		file.close();
		return NULL;
	}

	try {
		code = new char[size];
	} catch (...) {
		cerr << "Failed to allocate code" << endl;
		file.close();
		return NULL;
	}

	cout << "name " << filename << " size " << size << endl;

	file.seekg(0, ios::beg);
	file.read(code, size);
	file.close();

	return code;
}

int CShader::Load(const char *vFile, const char *fFile)
{
	GLint vertShader;
	GLint fragShader;
	GLint status;

	if (!vFile || !fFile) {
		cerr << "Invalid parameter " << vFile << "," << fFile << endl;
		return -EINVAL;
	}

	vertShader = LoadNCompile(GL_VERTEX_SHADER, vFile);
	fragShader = LoadNCompile(GL_FRAGMENT_SHADER, fFile);

	m_program = glCreateProgram();
	if (m_program == 0)
		return -EFAULT;

	glAttachShader(m_program, vertShader);
	glAttachShader(m_program, fragShader);
	glLinkProgram(m_program);

	// After link the shaders to a program,
	// We don't need them anymore.
	glDetachShader(m_program, vertShader);
	glDetachShader(m_program, fragShader);
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	// Check the link result
	glGetProgramiv(m_program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		cerr << "Failed to link program" << endl;
		return -EFAULT;
	}

	m_mvpId = glGetUniformLocation(m_program, "mvp");
	StatusPrint();
	cout << "m_mvp index: " << m_mvpId << endl;

	return 0;
}

void CShader::UseProgram(void)
{
	glUseProgram(m_program);
}

GLuint CShader::Program(void)
{
	return m_program;
}

GLint CShader::MVPId(void)
{
	return m_mvpId;
}

// End of a file
