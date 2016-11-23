#include <iostream>

#include "cgmath.h"
#include "CView.h"

using namespace std;

CView *CView::m_instance = NULL;

CView::CView(void)
: m_eye(0.0f, 10.0f, 5.0f)
, m_at(0.0f, 0.0f, 0.0f)
, m_up(0.0f, 1.0f, 0.0f)
, m_translate(0.0f, 0.0f, 0.0f)
, m_rotateAxis(0.0f, 0.0f, 0.0f)
, m_rotateAngle(0.0f)
, m_updated(true)
{

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
		m_viewMatrix.setLookAt(m_eye, m_at, m_up);
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

/* End of a file */