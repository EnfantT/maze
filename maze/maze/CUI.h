#pragma once
#if !defined(__CUI_H)
#define __CUI_H

#include "CObject.h"

// User Interface : Windows & input event handlers
class CUI {
private:
	static int m_initialized;
	static void errorCB(int error, const char *description);
	static void keyCB(GLFWwindow *win, int key, int scancode, int action, int mods);
	static void ptrCB(GLFWwindow *win, double x, double y);
	GLFWwindow *m_win;

	CObject *m_objectList;

	static CMovable *m_target;

public:
	CUI(void);
	virtual ~CUI(void);

	int CreateContext(int w = 0, int h = 0, const char *title = NULL);
	int DestroyContext(void);
	int Run(void);

	int AddObject(CObject *obj);
	int DelObject(CObject *obj);
};

#endif
/* End of a file */
