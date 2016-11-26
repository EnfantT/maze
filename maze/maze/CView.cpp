#include <iostream>

#include "cgmath.h"
#include "CMovable.h"
#include "CView.h"

using namespace std;

CView *CView::m_instance = NULL;

CView::CView(void)
: m_eye(0.0f, 0.0f, 10.0f)
, m_at(0.0f, 0.0f, 0.0f)
, m_up(0.0f, 1.0f, 0.0f)
, m_rotateAxis(0.0f, 0.0f, 0.0f)
, m_rotateAngle(0.0f)
, m_updated(true)
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

void CView::SetEye(vec3 &eye)
{
	m_eye = eye;
	m_updated = true;
}

void CView::SetEye(float x, float y, float z)
{
	m_eye.set(x, y, z);
	m_updated = true;
}

void CView::SetAt(vec3 &at)
{
	m_at = at;
	m_updated = true;
}

void CView::SetAt(float x, float y, float z)
{
	m_at.set(x, y, z);
	m_updated = true;
}

void CView::SetUp(vec3 &up)
{
	m_up = up;
	m_updated = true;
}

void CView::SetUp(float x, float y, float z)
{
	m_up.set(x, y, z);
	m_updated = true;
}

mat4 CView::Matrix(void)
{
	if (m_updated) {
		m_viewMatrix = mat4::lookAt(m_eye, m_at, m_up) * m_rotate * m_translate;
		m_updated = false;
	}

	return m_viewMatrix;
}

vec3 CView::Eye(void)
{
	return m_eye;
}

vec3 CView::At(void)
{
	return m_at;
}

vec3 CView::Up(void)
{
	return m_up;
}

bool CView::Updated(void)
{
	return m_updated;
}

void CView::Rotate(vec3 axis, float angle)
{
//	m_at = mat4::rotate(axis, angle) * vec4(m_at.x, m_at.y, m_at.z, 1.0f);
	m_rotate = mat4::rotate(axis, angle) * m_rotate;
	cout << "rotate: " << m_at.x << "," << m_at.y << "," << m_at.z << endl;
	m_updated = true;
}

void CView::Translate(vec4 vec)
{
//	m_eye = mat4::translate(vec.x, vec.y, vec.z) * vec4(m_eye.x, m_eye.y, m_eye.z, 1.0f);
	m_translate = mat4::translate(vec.x, vec.y, vec.z) * m_translate;
	m_updated = true;
}

void CView::Scale(vec4 scale)
{
	/* Do nothing */
}

/* End of a file */