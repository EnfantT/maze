#include <iostream>

#include "cgmath.h"
#include "CPerspective.h"

using namespace std;

CPerspective *CPerspective::m_instance = NULL;

CPerspective::CPerspective(void)
: m_fov(PI / 60.0f)
, m_ratio(1024.0f / 768.0f)
, m_near(0.1f)
, m_far(10000.0f)
, m_updated(true)
{

}

CPerspective::~CPerspective(void)
{

}

CPerspective *CPerspective::GetInstance(void)
{
	if (!m_instance) {
		try {
			m_instance = new CPerspective();
		}
		catch (...) {
			return NULL;
		}
	}

	return m_instance;
}


void CPerspective::Destroy(void)
{
	m_instance = NULL;
	delete this;
}

void CPerspective::SetFOV(float fov)
{
	m_fov = fov;
	m_updated = true;
}

void CPerspective::SetRatio(float ratio)
{
	m_ratio = ratio;
	m_updated = true;
}

void CPerspective::SetNear(float dNear)
{
	m_near = dNear;
	m_updated = true;
}

void CPerspective::SetFar(float dFar)
{
	m_far = dFar;
	m_updated = true;
}

bool CPerspective::Updated(void)
{
	return m_updated;
}

mat4 CPerspective::Matrix(void)
{
	if (m_updated) {
		m_perspective.setPerspective(m_fov, m_ratio, m_near, m_far);
		m_updated = false;
	}

	return m_perspective;
}

/* End of a file */