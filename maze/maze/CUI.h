#pragma once
#if !defined(__CUI_H)
#define __CUI_H

#include "CObject.h"

// User Interface : Windows & input event handlers
class CUI {
private:
	static void errorCB(int error, const char *description);
	static void keyCB(GLFWwindow *win, int key, int scancode, int action, int mods);
	static void ptrCB(GLFWwindow *win, double x, double y);
	GLFWwindow *m_win;
	CObject *m_objectList;
	CMovable *m_target;

	CUI(void);
	virtual ~CUI(void);

	static CUI *m_instance;

public:
	static CUI *GetInstance(void);
	void Destroy(void);
	int CreateContext(int w = 0, int h = 0, const char *title = NULL);
	int DestroyContext(void);
	int Run(void);

	void SetControlTarget(CMovable *target);
	CMovable *ControlTarget(void);

	int AddObject(CObject *obj);
	int DelObject(CObject *obj);
};

#endif
/* End of a file */
