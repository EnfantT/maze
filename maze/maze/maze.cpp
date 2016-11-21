#include <iostream>
#include <stdlib.h>
#include <errno.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "linmath.h"

using namespace std;

class CMaze {
public:
	CMaze();
	virtual ~CMaze();
};

CMaze::CMaze()
{
}

CMaze::~CMaze()
{
}

// User Interface : Windows & input event handlers
class CUI {
private:
	static int m_initialized;
	static void errorCB(int error, const char *description);
	static void keyCB(GLFWwindow *win, int key, int scancode, int action, int mods);
	static void ptrCB(GLFWwindow *win, double x, double y);
	GLFWwindow *m_win;
public:
	CUI(void);
	virtual ~CUI(void);

	int CreateContext(int w = 0, int h = 0, const char *title = NULL);
	int DestroyContext(void);
	int Run(void);
};

int CUI::m_initialized = 0;

void CUI::errorCB(int error, const char *description)
{
	cerr << description << "(" << error << ")" << endl;
}

void CUI::ptrCB(GLFWwindow *win, double x, double y)
{
}

void CUI::keyCB(GLFWwindow *win, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(win, 1);
}

CUI::CUI(void)
: m_win(NULL)
{
	if (m_initialized == 0) {
		int status;

		status = glfwInit();
		if (status == 0) {
			cout << "glfwInit: " << status;
			return;
		}

		glfwSetErrorCallback(errorCB);

		m_initialized = 1;

		cout << glfwGetVersionString() << endl;
	}
}

CUI::~CUI(void)
{
	if (m_initialized == 1) {
		glfwTerminate();
		m_initialized = 0;
	}
}

int CUI::CreateContext(int w, int h, const char *title)
{
	if (m_initialized == 0)
		return -EFAULT;

	if (w == 0 || h == 0) {
		w = 640;
		h = 480;
	}

	if (!title)
		title = "unnamed";

	m_win = glfwCreateWindow(w, h, title, NULL, NULL);
	if (!m_win)
		return -EFAULT;

	glfwMakeContextCurrent(m_win);
	glfwSetKeyCallback(m_win, keyCB);
	glfwSetCursorPosCallback(m_win, ptrCB);
	glfwSwapInterval(1);

	/**
	 * \brief
	 * glad 를 이용하여, GL 함수들을 매핑 시킨다.
	 * 
	 * \note
	 * glfwMakeContextCurrent 이후에 호출되어야 한다.
	 */
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

int CUI::DestroyContext(void)
{
	if (m_initialized == 0)
		return -EFAULT;

	if (m_win == NULL)
		return -EFAULT;

	glfwDestroyWindow(m_win);
}

int CUI::Run(void)
{
	if (m_initialized == 0)
		return -EFAULT;

	if (m_win == NULL)
		return -EFAULT;

	while (glfwWindowShouldClose(m_win) == 0) {
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwPollEvents();
		glfwSwapBuffers(m_win);
	}
}

class CShader {
private:
	GLuint m_vert;
	GLuint m_frag;
	GLuint m_program;

	static const char * const m_vertCode;
	static const char * const m_fragCode;

	int LoadNCompile(GLenum type, const char *code);

public:
	CShader(void);
	virtual ~CShader(void);

	int Load(void);
	int Map(void);
	int Unload(void);
};

const char * const CShader::m_vertCode =
	"#version 130\n"
	"in vec4 position;\n"
	"in vec4 color;\n"
	"out vec4 vertexColor;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = position;\n"
	"   vertexColor = color;\n"
	"}\n";

const char * const CShader::m_fragCode =
	"#version 130\n"
	"in vec4 vertexColor;\n"
	"void main()\n"
	"{\n"
	"   gl_FragColor = vertexColor;\n"//vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
	"}\n";

CShader::CShader()
{
}

CShader::~CShader()
{
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
	GLint status;

	m_vert = LoadNCompile(GL_VERTEX_SHADER, m_vertCode);
	m_frag = LoadNCompile(GL_FRAGMENT_SHADER, m_fragCode);

	m_program = glCreateProgram();
	glAttachShader(m_program, m_vert);
	glAttachShader(m_program, m_frag);
	glLinkProgram(m_program);

	glGetProgramiv(m_program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		cerr << "Failed to link program" << endl;
		return -EFAULT;
	}

	return 0;
}

int CShader::Map(void)
{
	GLint color;
	GLint position;
	GLuint vBuffer;

	struct vertex {
		float x, y, z, w;
	} vertices[6] = {
		// Vertex
		{ 0.5f, 0.0f, 0.5f, 1.0f },
		{ -0.5f, 0.0f, -0.5f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		// Color
		{ 1.0f, 0.0f, 0.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f, 1.0f },
	};

	glGenBuffers(1, &vBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	cout << sizeof(vertices) << endl;

	position = glGetAttribLocation(m_program, "position");
	cout << "position index: " << position << endl;
	if (position >= 0) {
		glEnableVertexAttribArray(position);
		glVertexAttribPointer(position, 4, GL_FLOAT, GL_FALSE,
						sizeof(float) * 4,
						(void *)0);
	}

	// glGetAttribLocation 함수는, 찾고자 하는 변수가 shader 안에서
	// 사용되지 않는 경우 (선언은 되어 있으나, reference 되지 않아도)
	// 또는 접두사 gl_ 로 시작하는 예약어에 접근할 때 -1 을 반환한다.
	// 선언 해두고 사용하지 않는 변수에 접근할 때 자꾸 -1 을 반환해서
	// 삽질 많이 했음
	color = glGetAttribLocation(m_program, "color");
	cout << "color index: " << color << endl;
	if (color >= 0) {
		glEnableVertexAttribArray(color);
		glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE,
						sizeof(float) * 4,
						(void *)(sizeof(float) * 12));
	}

	glUseProgram(m_program);
	return 0;
}

int CShader::Unload(void)
{
	return 0;
}

int main(int argc, char *argv[])
{
	CUI ui;
	CShader shader;
	int status;

	srand(time(0));

	status = ui.CreateContext();
	if (status < 0)
		return status;

//	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_CULL_FACE);

	shader.Load();

	shader.Map();

	ui.Run();

	shader.Unload();

	status = ui.DestroyContext();

	return 0;
}

/* End of a file */
