#include <iostream>

#include "CMisc.h"
#include "CObject.h"

using namespace std;


int CObject::AddTail(CObject *obj)
{
	if (m_next) {
		obj->m_next = m_next;
		m_next->m_prev = obj;
	}

	obj->m_prev = this;
	m_next = obj;

	return 0;
}

int CObject::Remove(void)
{
	if (m_prev)
		m_prev->m_next = m_next;

	if (m_next)
		m_next->m_prev = m_prev;

	return 0;
}

CObject *CObject::Next(void)
{
	return m_next;
}

CObject *CObject::Prev(void)
{
	return m_prev;
}

/* End of a file */
