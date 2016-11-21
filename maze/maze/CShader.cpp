#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "linmath.h"

#include "CShader.h"

using namespace std;

CShader *CShader::m_pInstance = NULL;

const GLchar * const CShader::m_vertCode =
"#version 130 core\n"
"uniform mat4 mvp;\n" /* mvp: ModelViewProject */
"in vec4 position;\n"
"in vec4 color;\n"
"out vec4 vertexColor;\n"
"void main()\n"
"{\n"
"   gl_Position = position * mvp;\n"
"   vertexColor = color;\n"
"}\n";

const GLchar * const CShader::m_fragCode =
"#version 130 core\n"
"in vec4 vertexColor;\n"
"void main()\n"
"{\n"
"   gl_FragColor = vertexColor;\n"
"}\n";

CShader::CShader()
	: m_program(0)
{
	mat4x4_identity(m_mvp_matrix);
	m_mvp_updated = GL_TRUE;
}

CShader::~CShader()
{
	if (m_program > 0)
		glDeleteProgram(m_program);

	glDeleteBuffers(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
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

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(m_program);
	
	if (m_mvp_updated == GL_TRUE) {
		cout << "Update MVP" << endl;
		glUniformMatrix4fv(m_mvp, 1, GL_FALSE, (const GLfloat *)m_mvp_matrix);
		m_mvp_updated = GL_FALSE;
	}

	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
	return 0;
}

int CShader::Map(void)
{
	GLint color;
	GLint position;

	struct vertex {
		float x, y, z, w;
	} vertices[] = {
		// Vertex
		{ -0.5f, -0.5f, -0.5f, 1.0f },
		{ 0.5f, -0.5f, -0.5f, 1.0f },
		{ 0.0f, 0.5f, -0.5f, 1.0f },
		// Color
		{ 1.0f, 0.0f, 0.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f, 1.0f },
	};

	if (m_program == 0) {
		cerr << "Program is not initialized" << endl;
		return -EFAULT;
	}

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	cout << sizeof(vertices) / 2 << endl;

	// [nicesj]
	// glGetAttribLocation function returns -1 if the given variable
	// has no reference in the shader.
	// And if it is a reserved name, such has prefix "gl_", returns -1 too.
	position = glGetAttribLocation(m_program, "position");
	cout << "position index: " << position << endl;
	if (position >= 0) {
		glEnableVertexAttribArray(position);
		glVertexAttribPointer(position, 4, GL_FLOAT, GL_FALSE,
			sizeof(vertices) / 2,
			(void *)0);
	}

	color = glGetAttribLocation(m_program, "color");
	cout << "color index: " << color << endl;
	if (color >= 0) {
		glEnableVertexAttribArray(color);
		glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE,
			sizeof(vertices) / 2,
			(void *)(sizeof(vertices) / 2));
	}

	// After handling the VBO,
	// We should unbound it for safety use.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_mvp = glGetUniformLocation(m_program, "mvp");
	cout << "mvp index: " << m_mvp << endl;
	if (m_mvp >= 0)
		glEnableVertexAttribArray(m_mvp);

	return 0;
}

int CShader::Unload(void)
{
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
	mat4x4_rotate(m_mvp_matrix, m_mvp_matrix, x, y, z, angle);
	m_mvp_updated = GL_TRUE;
	return 0;
}

// End of a file