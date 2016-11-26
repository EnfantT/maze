#include <iostream>

#include "cgmath.h"
#include "CMovable.h"
#include "CModel.h"

using namespace std;

CModel *CModel::m_instance = NULL;

CModel::CModel(void)
	: m_updated(true)
{
	m_model.setIdentity();
}

CModel::~CModel(void)
{

}

CModel *CModel::GetInstance(void)
{
	if (!m_instance) {
		try {
			m_instance = new CModel();
		}
		catch (...) {
			return NULL;
		}
	}

	return m_instance;
}

void CModel::Destroy(void)
{
	m_instance = NULL;
	delete this;
}

bool CModel::Updated(void)
{
	return m_updated;
}

mat4 CModel::Matrix(void)
{
	if (m_updated)
		m_updated = false;

	return m_model;
}

void CModel::Translate(vec4 vec)
{
	m_model = mat4::translate(vec.x, vec.y, vec.z) *m_model;
	m_updated = true;
}

void CModel::Rotate(vec3 axis, float angle)
{
	m_model = mat4::rotate(axis, angle) * m_model;
	m_updated = true; 
}

void CModel::Scale(vec4 scale)
{
	m_model = mat4::scale(scale.x, scale.y, scale.z) * m_model;
	m_updated = true;
}

/* End of a file */