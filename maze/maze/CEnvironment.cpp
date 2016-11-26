#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "cgmath.h"

#include "CObject.h"
#include "CShader.h"
#include "CEnvironment.h"
#include "CMovable.h"
#include "CModel.h"
#include "CPerspective.h"
#include "CView.h"

using namespace std;

CEnvironment *CEnvironment::m_instance = NULL;

CEnvironment::CEnvironment(void)
{

}

CEnvironment::~CEnvironment(void)
{

}

CEnvironment *CEnvironment::GetInstance(void)
{
	if (!m_instance) {
		try {
			m_instance = new CEnvironment();
		}
		catch (...) {
			return NULL;
		}
	}

	return m_instance;
}

void CEnvironment::Destroy(void)
{
	delete this;
	m_instance = NULL;
}

int CEnvironment::Load(void)
{
	return 0;
}

int CEnvironment::Render(void)
{
	GLenum status;
	mat4 mvp;

	mvp = CPerspective::GetInstance()->Matrix() * CView::GetInstance()->Matrix() * CModel::GetInstance()->Matrix();

	glUniformMatrix4fv(CShader::GetInstance()->MVPId(), 1, GL_TRUE, (const GLfloat *)mvp);
	if (glGetError() != GL_NO_ERROR)
		cerr << "Failed to uniform" << endl;

	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (void *)(sizeof(GLfloat) * 45));
	status = glGetError();
	if (status != GL_NO_ERROR)
		cerr << __func__ << ":" << __LINE__ << ":" << status << endl;

	return 0;
}

/* End of a file */