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
#include <errno.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "cgmath.h"

#include "CMovable.h"
#include "CView.h"
#include "CPerspective.h"
#include "CModel.h"

#include "CShader.h"

#if defined(_WIN32)
#define GLSL_VERSION "#version 430 core\n"
#else
#define GLSL_VERSION "#version 130\n"
#endif

using namespace std;

CShader *CShader::m_pInstance = NULL;

const GLchar * const CShader::m_vertCode =
GLSL_VERSION
"uniform mat4 mvp;\n"
"in vec4 position;\n"
"in vec4 color;\n"
"in vec4 offset;\n"
"out vec4 vertexColor;\n"
"void main()\n"
"{\n"
"   gl_Position = mvp * (position + offset);\n"
"   vertexColor = color;\n"
"}\n";

const GLchar * const CShader::m_fragCode =
GLSL_VERSION
"in vec4 vertexColor;\n"
"void main()\n"
"{\n"
"   gl_FragColor = vertexColor;\n"
"}\n";

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

int CShader::LoadNCompile(GLenum type, const char *code)
{
	GLuint shader;
	GLint status;

	shader = glCreateShader(type);
	glShaderSource(shader, 1, &code, NULL);
	glCompileShader(shader);

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

int CShader::Load(void)
{
	GLint vertShader;
	GLint fragShader;
	GLint status;

	if (m_program != 0) {
		cerr << "Program is already created" << endl;
		return -EALREADY;
	}

	vertShader = LoadNCompile(GL_VERTEX_SHADER, m_vertCode);
	fragShader = LoadNCompile(GL_FRAGMENT_SHADER, m_fragCode);

	m_program = glCreateProgram();
	if (m_program == 0) {
		return -EFAULT;
	}
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

	return 0;
}

void CShader::UseProgram(void)
{
	glUseProgram(m_program);
}


int CShader::Map(void)
{
	m_mvpId = glGetUniformLocation(m_program, "mvp");
	if (glGetError() != GL_NO_ERROR)
		cerr << "Failed to find a m_mvpId" << endl;
	else
		glEnableVertexAttribArray(m_mvpId);

	cout << "m_mvp index: " << m_mvpId << endl;

	return 0;
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