#pragma once
#if !defined(__CMATRIX_H)
#define __CMATRIX_H

class {
private:
	mat4 m_model;
	mat4 m_view;
	mat4 m_perspective;

public:
	CMatrix(void);
	virtual ~CMatrix(void);

	void SetView(vec3 m_eye, vec3 m_at, vec3 m_up);
};

#endif
/* End of a file */