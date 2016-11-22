#include <iostream>
#include <errno.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "linmath.h"

#include "CShader.h"

#if defined(_WIN32)
#define GLSL_VERSION "#version 130 core\n"
#else
#define GLSL_VERSION "#version 130\n"
#endif

using namespace std;

CShader *CShader::m_pInstance = NULL;

const GLchar * const CShader::m_vertCode =
GLSL_VERSION
"uniform mat4 mvp;\n" /* mvp: ModelViewProject */
"in vec4 position;\n"
"in vec4 color;\n"
"in vec4 offset;\n"
"out vec4 vertexColor;\n"
"void main()\n"
"{\n"
"   gl_Position = (position + offset) * mvp;\n"
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
	mat4x4 perspective;
	mat4x4 camera;

	/**
	 * [nicesj]
	 * We have to handle these matrices.
	 * Camera & Perspective.
	vec3 eye = { 0.0f, 30.0f, 300.0f };
	vec3 center = { 0.0f, 0.0f, 0.0f };
	vec3 up = { 0.0f, 1.0f, 0.0f };
	mat4x4_look_at(camera, eye, center, up);
	mat4x4_perspective(perspective, 3.141592f/4.0f, 1024/768, 1.0f, 1000.0f);
	 */

	mat4x4_identity(camera);
	mat4x4_identity(perspective);

	mat4x4_mul(m_mvp_matrix, camera, perspective);
	m_mvp_updated = GL_TRUE;

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
}

CShader::~CShader()
{
	// m_program should be destroyed via Unload.
	glDeleteBuffers(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
}

void CShader::Destroy(void)
{
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

int CShader::ApplyMVP(void)
{
	if (m_program == 0)
		return -EFAULT;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_program);
	
	if (m_mvp_updated == GL_TRUE) {
		cout << "Update MVP" << endl;
		glUniformMatrix4fv(m_mvp, 1, GL_FALSE, (const GLfloat *)m_mvp_matrix);
		m_mvp_updated = GL_FALSE;
	}

	return 0;
}

int CShader::Map(void)
{
	m_mvp = glGetUniformLocation(m_program, "mvp");
	cout << "mvp index: " << m_mvp << endl;
	if (m_mvp >= 0)
		glEnableVertexAttribArray(m_mvp);

	return 0;
}

int CShader::Unload(void)
{
	if (m_program > 0)
		glDeleteProgram(m_program);
	m_program = 0;
	return 0;
}

int CShader::Translate(float x, float y, float z)
{
	mat4x4 t;

	mat4x4_translate(t, x, y, z);
	mat4x4_mul(m_mvp_matrix, m_mvp_matrix, t);

	m_mvp_updated = GL_TRUE;
	return 0;
}

int CShader::Scale(float x, float y, float z, float scale)
{
	mat4x4 a;

	mat4x4_identity(a);

	a[0][0] = x;
	a[1][1] = y;
	a[2][2] = z;

	mat4x4_scale(m_mvp_matrix, a, scale);

	m_mvp_updated = GL_TRUE;
	return 0;
}

int CShader::Rotate(float x, float y, float z, float angle)
{
	if (x >= 1.0f)
		mat4x4_rotate_X(m_mvp_matrix, m_mvp_matrix, angle);

	if (y >= 1.0f)
		mat4x4_rotate_Y(m_mvp_matrix, m_mvp_matrix, angle);

	if (z >= 1.0f)
		mat4x4_rotate_Z(m_mvp_matrix, m_mvp_matrix, angle);

	m_mvp_updated = GL_TRUE;
	return 0;
}

GLuint CShader::GetProgram(void)
{
	return m_program;
}

// End of a file
