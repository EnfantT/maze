#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "cgmath.h"

#include "CObject.h"
#include "CMovable.h"
#include "CVertices.h"
#include "CShader.h"
#include "CPlayer.h"
#include "CPerspective.h"
#include "CModel.h"
#include "CView.h"

using namespace std;

CPlayer *CPlayer::m_instance = NULL;

CPlayer::CPlayer(void)
{
	m_translate.setIdentity();
	m_rotate.setIdentity();
	m_scale.setIdentity();
}

CPlayer::~CPlayer(void)
{

}

CPlayer *CPlayer::GetInstance(void)
{
	if (!m_instance) {
		try {
			m_instance = new CPlayer();
		}
		catch (...) {
			return NULL;
		}
	}

	return m_instance;
}

void CPlayer::Destroy(void)
{
	m_instance = NULL;
	delete this;
}

int CPlayer::Render(void)
{
	mat4 mvp;

	mvp = CPerspective::GetInstance()->Matrix() * CView::GetInstance()->Matrix() * m_translate * m_scale * m_rotate;

	glUniformMatrix4fv(CShader::GetInstance()->MVPId(), 1, GL_TRUE, (const GLfloat *)mvp);
	if (glGetError() != GL_NO_ERROR)
		cerr << "Failed to uniform" << endl;

	// Drawing a player
	glDrawElements(GL_TRIANGLE_STRIP, 18, GL_UNSIGNED_INT, (void *)(18 * sizeof(GL_FLOAT)));
	if (glGetError() != GL_NO_ERROR)
		cerr << __func__ << ":" << __LINE__ << endl;

	return 0;
}

int CPlayer::Load(void)
{
	return 0;
}

void CPlayer::Translate(vec4 vec)
{
	m_translate = mat4::translate(vec.x, vec.y, vec.z) * m_translate;
}

void CPlayer::Rotate(vec3 axis, float angle)
{
	m_rotate = mat4::rotate(axis, angle) * m_rotate;
}

void CPlayer::Scale(vec4 scale)
{
	m_scale = mat4::scale(scale.x, scale.y, scale.z) * m_scale;
}

/* End of a file */