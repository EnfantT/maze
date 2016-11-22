#include <iostream>
#include <errno.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "linmath.h"
#include "CUI.h"
#include "CShader.h"

using namespace std;

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
	CShader *shader;
	static GLenum flag = GL_FILL;

	shader = CShader::GetInstance();
	if (!shader) {
		cerr << "Shader object is not valid" << endl;
		return;
	}

	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_W: // Up
			shader->Translate(0.0f, 0.0f, 0.5f);
			break;
		case GLFW_KEY_A: // Left
			shader->Translate(0.0f, -0.5f, 0.0f);
			break;
		case GLFW_KEY_D: // Right
			shader->Translate(0.0f, 0.5f, 0.0f);
			break;
		case GLFW_KEY_S: // Down
			shader->Translate(0.0f, 0.0f, -0.5f);
			break;
		case GLFW_KEY_N:
			shader->Scale(2.0f, 2.0f, 2.0f, 1.0f);
			break;
		case GLFW_KEY_M:
			shader->Scale(0.5f, 0.5f, 0.5f, 1.0f);
			break;
		case GLFW_KEY_O:
			shader->Rotate(0.0f, 1.0f, 0.0f, mods ? -0.1f : 0.1f);
			break;
		case GLFW_KEY_P:
			shader->Rotate(1.0f, 0.0f, 0.0f, mods ? -0.1f : 0.1f);
			break;
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(win, 1);
			break;
		case GLFW_KEY_0:
			if (flag == GL_FILL)
				flag = GL_LINE;
			else
				flag = GL_FILL;
			// GL_FILL : Filling with colors.
			// GL_LINE : Drawing lines only.
			glPolygonMode(GL_FRONT_AND_BACK, flag);
			break;
		default:
			cout << "error" << endl;
			break;
		}
	}
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
		w = 1024;
		h = 768;
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
	 * gladLoadGLLoader should be called after glfwMakeContextCurrent
	 */
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glViewport(0, 0, 1024, 768);
	return 0;
}

int CUI::DestroyContext(void)
{
	if (m_initialized == 0)
		return -EFAULT;

	if (m_win == NULL)
		return -EFAULT;

	glfwDestroyWindow(m_win);
	return 0;
}

int CUI::Run(void)
{
	if (m_initialized == 0)
		return -EFAULT;

	if (m_win == NULL)
		return -EFAULT;

	while (glfwWindowShouldClose(m_win) == 0) {

		CShader::GetInstance()->ApplyMVP();

		glfwPollEvents();
		glfwSwapBuffers(m_win);
	}

	return 0;
}
