/**
 * \brief
 * This class is used for manipulating Camera(aka. Eye)
 * Coordinate system of the camera is inverted of the world's one.
 */

#pragma once
#if !defined(__CVIEW_H)
#define __CVIEW_H

class CView : public CMovable {
private:
	// Camera info
	vec3 m_eye;
	vec3 m_at;
	vec3 m_up;
	bool m_updated;

	mat4 m_viewMatrix;

	vec3 m_rotateAxis;
	float m_rotateAngle;

	mat4 m_rotate;
	mat4 m_translate;

	static CView *m_instance;

	CView(void);
	virtual ~CView(void);

public:
	static CView *GetInstance(void);
	void Destroy(void);

	mat4 Matrix(void);

	bool Updated(void);

	virtual void Translate(CMovable::Direction d, float amount);
	virtual void Rotate(vec3 axis, float angle);
	virtual void Scale(vec4 scale);
};

#endif
/* End of a file */
