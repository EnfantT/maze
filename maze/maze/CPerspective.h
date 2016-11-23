#pragma once
#if !defined(__CPERSPECTIVE_H)
#define __CPERSPECTIVE_H

class CPerspective {
private:
	mat4 m_perspective;
	float m_fov;
	float m_ratio;
	float m_near;
	float m_far;
	bool m_updated;

	static CPerspective *m_instance;

	CPerspective(void);
	virtual ~CPerspective(void);

public:

	static CPerspective *GetInstance(void);
	void Destroy(void);

	void SetFOV(float fov);
	void SetRatio(float ratio);
	void SetNear(float dNear);
	void SetFar(float dFar);

	bool Updated(void);

	mat4 Matrix(void);
};

#endif
/* End of a file */