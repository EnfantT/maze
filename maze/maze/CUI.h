#pragma once
// User Interface : Windows & input event handlers
class CUI {
private:
	static int m_initialized;
	static void errorCB(int error, const char *description);
	static void keyCB(GLFWwindow *win, int key, int scancode, int action, int mods);
	static void ptrCB(GLFWwindow *win, double x, double y);
	GLFWwindow *m_win;
public:
	CUI(void);
	virtual ~CUI(void);

	int CreateContext(int w = 0, int h = 0, const char *title = NULL);
	int DestroyContext(void);
	int Run(void);
};
