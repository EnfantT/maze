#pragma once
#if !defined(__COBJECT_H)
#define __COBJECT_H

/**
 * \brief
 * Classes which inherit this, could be managed by list.
 * Each object should be linked. to render them all in the Render function of CUI class.
 */
class CObject {
private:
	CObject *m_prev;
	CObject *m_next;

public:
	CObject(void) : m_prev(NULL), m_next(NULL) { }
	virtual ~CObject(void) { }

	virtual int Render(void) { return 0; }
	virtual int Load(void) { return 0; }

	/* List operator */
	virtual int AddTail(CObject *obj);
	virtual int Remove(void);
	virtual CObject *Next(void);
	virtual CObject *Prev(void);
};

#endif
