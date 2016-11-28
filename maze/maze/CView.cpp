#include <iostream>

#include "cgmath.h"
#include "CMovable.h"
#include "CView.h"

using namespace std;

CView *CView::m_instance = NULL;

CView::CView(void)
: m_eye(0.0f, 0.0f, 0.0f)
, m_at(0.0f, 0.0f, -1.0f)
, m_up(0.0f, 1.0f, 0.0f)
, m_updated(true)
, m_rotateAxis(0.0f, 0.0f, 0.0f)
, m_rotateAngle(0.0f)
{
	m_rotate.setIdentity();
	m_translate.setIdentity();
}

CView::~CView(void)
{

}

CView *CView::GetInstance(void)
{
	if (!m_instance) {
		try {
			m_instance = new CView();
		}
		catch (...) {
			return NULL;
		}
	}

	return m_instance;
}

void CView::Destroy(void)
{
	m_instance = NULL;
	delete this;
}

mat4 CView::Matrix(void)
{
	if (m_updated) {
		m_viewMatrix = mat4::lookAt(m_eye, m_at, m_up) * m_rotate * m_translate;
		m_updated = false;
	}

	return m_viewMatrix;
}

bool CView::Updated(void)
{
	return m_updated;
}

void CView::Rotate(vec3 axis, float angle)
{
	m_rotate = mat4::rotate(axis, angle) * m_rotate;
	m_updated = true;
}

void CView::Translate(CMovable::Direction d, float amount)
{
	vec4 r;
	switch (d) {
	case FRONT:
		r = m_rotate * vec4(0.0f, 0.0f, amount, 1.0f);
		break;
	case BACK:
		r = m_rotate * vec4(0.0f, 0.0f, -amount, 1.0f);
		break;
	case LEFT:
		r = m_rotate * vec4(amount, 0.0f, 0.0f, 1.0f);
		break;
	case RIGHT:
		r = m_rotate * vec4(-amount, 0.0f, 0.0f, 1.0f);
		break;
	default:
		break;
	}
	cout << r.x << "," << r.y << "," << r.z << endl;
	m_translate = mat4::translate(-r.x, -r.y, r.z) * m_translate;
	m_updated = true;
}

void CView::Scale(vec4 scale)
{
	/* Do nothing */
}

/* End of a file */
