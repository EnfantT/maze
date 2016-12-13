#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "cgmath.h"

#include "CMisc.h"
#include "CObject.h"
#include "CShader.h"
#include "CEnvironment.h"
#include "CMovable.h"
#include "CModel.h"
#include "CPerspective.h"
#include "CView.h"
#include "CTexture.h"
#include "CVertices.h"

using namespace std;

CEnvironment *CEnvironment::m_instance = NULL;

CEnvironment::CEnvironment(void)
{
	glGenBuffers(1, &m_VBO);
}

CEnvironment::~CEnvironment(void)
{
	glDeleteBuffers(1, &m_VBO);
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

	
	/*
	m_isEnvIdx = glGetUniformLocation(CShader::GetInstance()->Program(), "isFloor");
	cout << "isFloor index: " << m_isEnvIdx << endl;

	m_FloortexImageId = CTexture::GetInstance()->Load();// "wall.jpg", 1);
	if (m_FloortexImageId == 0) {
		cerr << "Failed to create a texture image id" << endl;
	}
	else {
		GLint texId; // Texture Sampler 2D
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_FloortexImageId);

		texId = glGetUniformLocation(CShader::GetInstance()->Program(), "tex_F");
		glUniform1i(texId, 0);	// map GL_TEXTURE0
	}
	*/


	return 0;
}

int CEnvironment::Render(void)
{
	mat4 mvp;

	mvp = CPerspective::GetInstance()->Matrix() * CView::GetInstance()->Matrix() * CModel::GetInstance()->Matrix();

	glUniformMatrix4fv(CShader::GetInstance()->MVPId(), 1, GL_TRUE, (const GLfloat *)mvp);
	StatusPrint();

	glUniform1i(m_isEnvIdx, 1);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (void *)(sizeof(GLuint) * 46));
	StatusPrint();

	return 0;
}

/* End of a file */
