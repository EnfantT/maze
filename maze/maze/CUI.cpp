#include <iostream>
#include <errno.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "cgmath.h"
#include "CView.h"
#include "CUI.h"
#include "CShader.h"
#include "CVertices.h"
#include "CPlayer.h"

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
	CPlayer *player;
	static GLenum flag = GL_FILL;
	vec4 move(0.0f, 0.0f, 0.0f, 0.0f);

	player = CPlayer::GetInstance();
	if (!player) {
		cerr << "player object is not valid" << endl;
		return;
	}

	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_W: // Up : Move eye to up side
			move.x = 0.0f;
			move.y = 0.1f;
			move.z = 0.0f;
			player->Translate(move);
			break;
		case GLFW_KEY_A: // Left
			move.x = -0.1f;
			move.y = 0.0f;
			move.z = 0.0f;
			player->Translate(move);
			break;
		case GLFW_KEY_D: // Right
			move.x = 0.1f;
			move.y = 0.0f;
			move.z = 0.0f;
			player->Translate(move);
			break;
		case GLFW_KEY_S: // Down
			move.x = 0.0f;
			move.y = -0.1f;
			move.z = 0.0f;
			player->Translate(move);
			break;
		case GLFW_KEY_N:
			break;
		case GLFW_KEY_M:
			break;
		case GLFW_KEY_O:
			break;
		case GLFW_KEY_P:
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
			cout << "Key error" << endl;
			break;
		}
	}
}

CUI::CUI(void)
	: m_win(NULL)
	, m_objectList(NULL)
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
	CObject *obj;

	if (m_initialized == 0)
		return -EFAULT;

	if (m_win == NULL)
		return -EFAULT;

	while (glfwWindowShouldClose(m_win) == 0) {
		CShader::GetInstance()->UseProgram();
		CShader::GetInstance()->ApplyMVP();

		obj = m_objectList;
		CVertices::GetInstance()->BindVAO();
		CVertices::GetInstance()->BindEBO();
		while (obj) {
			obj->Render();
			obj = obj->Next();
		}
		CVertices::GetInstance()->UnbindEBO();
		CVertices::GetInstance()->UnbindVAO();


		glfwPollEvents();
		glfwSwapBuffers(m_win);
	}

	return 0;
}

int CUI::AddObject(CObject *obj)
{
	if (m_objectList)
		m_objectList->AddBack(obj);
	else
		m_objectList = obj;

	return 0;
}

int CUI::DelObject(CObject *obj)
{
	if (m_objectList == obj)
		m_objectList = obj->Next();

	obj->Remove();
	return 0;
}

/* End of a file */
