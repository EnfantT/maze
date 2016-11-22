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
"out vec4 vertexColor;\n"
"void main()\n"
"{\n"
"   gl_Position = position * mvp;\n"
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
	vec3 eye;
	vec3 center;
	vec3 up;
	mat4x4 perspective;
	mat4x4 camera;

	//mat4x4_identity(m_mvp_matrix);

	eye[0] = 0.0f;
	eye[1] = 30.0f;
	eye[2] = 300.0f;

	center[0] = 0.0f;
	center[1] = 0.0f;
	center[2] = 0.0f;

	up[0] = 0.0;
	up[1] = 1.0f;
	up[2] = 0.0f;

	mat4x4_look_at(camera, eye, center, up);
	mat4x4_perspective(perspective, 3.141592f/4.0f, 1024/768, 1.0f, 1000.0f);
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

	glBindVertexArray(m_VAO);
	//glDrawArrays(GL_TRIANGLE_FAN, 0, 9);
	glDrawElements(GL_TRIANGLE_STRIP, 12, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (void *)(sizeof(GLuint) * 12));
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
		// Front
		{ 0.5f, 0.5f, 0.5f, 1.0f },
		{ -0.5f, 0.5f, 0.5f, 1.0f },
		{ -0.5f, -0.5f, 0.5f, 1.0f },
		{ 0.5f, -0.5f, 0.5f, 1.0f },
		{ 0.5f, -0.5f, -0.5f, 1.0f },
		{ 0.5f, 0.5f, -0.5f, 1.0f },
		{ -0.5f, 0.5f, -0.5f, 1.0f },
		{ -0.5f, -0.5f, -0.5f, 1.0f },

		// color
		{ 1.0f, 0.0f, 0.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f },
	};
	GLuint indices[] = {
		0, 1, 2, 6, 7, 5, 4, 0, 3, 2, 4, 7,
		0, 5, 1, 6,
	};

	if (m_program == 0) {
		cerr << "Program is not initialized" << endl;
		return -EFAULT;
	}

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	cout << sizeof(vertices) / 2 << endl;

	// [nicesj]
	// glGetAttribLocation function returns -1 if the given variable
	// has no reference in the shader.
	// And if it is a reserved name, such has prefix "gl_", returns -1 too.
	position = glGetAttribLocation(m_program, "position");
	cout << "position index: " << position << endl;
	if (position >= 0) {
		glEnableVertexAttribArray(position);
		// [nicesj]
		// Because of "stride" value, I lost 3 more hours.
		// I set it with wrong value (too big one), the triangle is disappeared.
		glVertexAttribPointer(position, 4, GL_FLOAT, GL_FALSE,
			sizeof(float) * 4,
			(void *)0);
	}

	color = glGetAttribLocation(m_program, "color");
	cout << "color index: " << color << endl;
	if (color >= 0) {
		glEnableVertexAttribArray(color);
		glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE,
			sizeof(float) * 4,
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

// End of a file
