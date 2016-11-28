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
#include "CMisc.h"

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
	StatusPrint();

	// Drawing a player
#if !defined(_OLD_GL)
	glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_INT, (void *)(18 * sizeof(GLuint)));
	StatusPrint();
#else
	glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_INT, (void *)(18 * sizeof(GLuint)));
	StatusPrint();
#endif	

	return 0;
}

int CPlayer::Load(void)
{
	return 0;
}

void CPlayer::Translate(CMovable::Direction d, float amount)
{
	switch (d) {
	case FRONT:
		m_translate = mat4::translate(0.0f, 0.0f, amount) * m_translate;
		break;
	case BACK:
		m_translate = mat4::translate(0.0f, 0.0f, -amount) * m_translate;
		break;
	case LEFT:
		m_translate = mat4::translate(-amount, 0.0f, 0.0f) * m_translate;
		break;
	case RIGHT:
		m_translate = mat4::translate(amount, 0.0f, 0.0f) * m_translate;
		break;
	default:
		break;
	}
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
