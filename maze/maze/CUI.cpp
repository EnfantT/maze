#include <iostream>
#include <stdlib.h>
#include <errno.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "cgmath.h"

#include "CMisc.h"
#include "CMovable.h"
#include "CView.h"
#include "CUI.h"
#include "CShader.h"
#include "CVertices.h"
#include "CPlayer.h"
#include "CPerspective.h"
#include "CModel.h"

using namespace std;

CUI *CUI::m_instance = NULL;

void CUI::errorCB(int error, const char *description)
{
	cerr << description << "(" << error << ")" << endl;
}

void CUI::ptrCB(GLFWwindow *win, double x, double y)
{
}

void resizeCB(GLFWwindow* win, int width, int height)
{
	glViewport(0, 0, width, height);
	CPerspective::GetInstance()->SetRatio((float)width / (float)height);
}

void CUI::keyCB(GLFWwindow *win, int key, int scancode, int action, int mods)
{
	static GLenum flag = GL_FILL;
	vec4 move(0.0f, 0.0f, 0.0f, 0.0f);

	if (!CUI::GetInstance()->ControlTarget())
		CUI::GetInstance()->SetControlTarget(CPlayer::GetInstance());

	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_1:
			CUI::GetInstance()->SetControlTarget(CPlayer::GetInstance());
			cout << "Player" << endl;
			break;
		case GLFW_KEY_2:
			cout << "Camera" << endl;
			CUI::GetInstance()->SetControlTarget(CView::GetInstance());
			break;
		case GLFW_KEY_3:
			CUI::GetInstance()->SetControlTarget(CModel::GetInstance());
			cout << "Model" << endl;
			break;
		case GLFW_KEY_UP: // Front : Move eye to up side
			CUI::GetInstance()->ControlTarget()->Translate(CMovable::Direction::FRONT, 0.5f);
			break;
		case GLFW_KEY_DOWN: // Back
			CUI::GetInstance()->ControlTarget()->Translate(CMovable::Direction::BACK, 0.5f);
			break;
		case GLFW_KEY_LEFT: // Left
			CUI::GetInstance()->ControlTarget()->Translate(CMovable::Direction::LEFT, 0.5f);
			break;
		case GLFW_KEY_RIGHT: // Right
			CUI::GetInstance()->ControlTarget()->Translate(CMovable::Direction::RIGHT, 0.5f);
			break;
		case GLFW_KEY_Q:
			CUI::GetInstance()->ControlTarget()->Rotate(vec3(1.0f, 0.0f, 0.0f), PI / 18.0f);
			break;
		case GLFW_KEY_A:
			CUI::GetInstance()->ControlTarget()->Rotate(vec3(1.0f, 0.0f, 0.0f), -PI / 18.0f);
			break;

		case GLFW_KEY_W:
			CUI::GetInstance()->ControlTarget()->Rotate(vec3(0.0f, 1.0f, 0.0f), PI / 18.0f);
			break;
		case GLFW_KEY_S:
			CUI::GetInstance()->ControlTarget()->Rotate(vec3(0.0f, 1.0f, 0.0f), -PI / 18.0f);
			break;
		case GLFW_KEY_E:
			CUI::GetInstance()->ControlTarget()->Rotate(vec3(0.0f, 0.0f, 1.0f), PI / 18.0f);
			break;
		case GLFW_KEY_D:
			CUI::GetInstance()->ControlTarget()->Rotate(vec3(0.0f, 0.0f, 1.0f), -PI / 18.0f);
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
	int status;

	status = glfwInit();
	if (status == 0) {
		cout << "glfwInit: " << status;
		return;
	}

	glfwSetErrorCallback(errorCB);
	cout << glfwGetVersionString() << endl;
	if (atof(glfwGetVersionString()) >= 3.1f)
		CMisc::EnableVersion_3_1();
}

CUI::~CUI(void)
{
	glfwTerminate();
}

void CUI::SetControlTarget(CMovable *target)
{
	m_target = target;
}

CMovable *CUI::ControlTarget(void)
{
	return m_target;
}

CUI *CUI::GetInstance(void)
{
	if (!m_instance) {
		try {
			m_instance = new CUI();
		}
		catch (...) {
			return NULL;
		}
	}

	return m_instance;
}

void CUI::Destroy(void)
{
	delete this;
	m_instance = NULL;
}

int CUI::CreateContext(int w, int h, const char *title)
{
	if (w == 0 || h == 0) {
		w = 1024;
		h = 768;
	}

	if (!title)
		title = "unnamed";

	m_win = glfwCreateWindow(w, h, title, NULL, NULL);
	if (!m_win)
		return -EFAULT;

	glfwSetWindowSizeCallback(m_win, resizeCB);
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
	if (m_win == NULL)
		return -EFAULT;

	glfwDestroyWindow(m_win);
	return 0;
}

int CUI::Run(void)
{
	CObject *obj;

	if (m_win == NULL)
		return -EFAULT;

	while (glfwWindowShouldClose(m_win) == 0) {
		CShader::GetInstance()->UseProgram();

		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		obj = m_objectList;
		CVertices::GetInstance()->BindVAO();
		CVertices::GetInstance()->BindEBO();
		while (obj) {
			obj->Render();
			obj = obj->Next();
		}
		CVertices::GetInstance()->UnbindEBO();
		CVertices::GetInstance()->UnbindVAO();

		//glfwPollEvents();
		glfwWaitEvents();
		glfwSwapBuffers(m_win);
	}

	return 0;
}

int CUI::AddObject(CObject *obj)
{
	if (m_objectList)
		m_objectList->AddTail(obj);
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
