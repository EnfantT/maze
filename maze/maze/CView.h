#pragma once
#if !defined(__CVIEW_H)
#define __CVIEW_H

class CView {
private:
	// Camera info
	vec3 m_eye;
	vec3 m_at;
	vec3 m_up;

public:
	CView(void);
	virtual ~CView(void);
};

#endif
/* End of a file */