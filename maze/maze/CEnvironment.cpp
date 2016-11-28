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
#include "CMisc.h"

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
	mat4 mvp;

	mvp = CPerspective::GetInstance()->Matrix() * CView::GetInstance()->Matrix() * CModel::GetInstance()->Matrix();

	glUniformMatrix4fv(CShader::GetInstance()->MVPId(), 1, GL_TRUE, (const GLfloat *)mvp);
	StatusPrint();

	glDrawElements(GL_TRIANGLES, 8, GL_UNSIGNED_INT, (void *)(sizeof(GLuint) * 45));
	StatusPrint();

	return 0;
}

/* End of a file */
