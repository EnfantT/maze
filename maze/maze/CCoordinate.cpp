#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "cgmath.h"
#include "CObject.h"
#include "CCoordinate.h"

using namespace std;

CCoordinate *CCoordinate::m_instance = NULL;

CCoordinate::CCoordinate(void)
{

}

CCoordinate::~CCoordinate(void)
{

}

CCoordinate *CCoordinate::GetInstance(void)
{
	if (!m_instance) {
		try {
			m_instance = new CCoordinate();
		}
		catch (...) {
			return NULL;
		}
	}

	return m_instance;
}

void CCoordinate::Destroy(void)
{
	delete this;
	m_instance = NULL;
}

int CCoordinate::Render(void)
{
	GLenum status;

	glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, (void *)(sizeof(GLfloat) * 22));
	status = glGetError();
	if (status != GL_NO_ERROR)
		cerr << __func__ << ":" << __LINE__ << ":" << status << endl;

	return 0;
}

int CCoordinate::Load(void)
{
	return 0;
}

/* End of a file */