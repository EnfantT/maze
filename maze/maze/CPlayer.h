#pragma once
#if !defined(__CPLAYER_H)
#define __CPLAYER_H

class CPlayer : public CObject, public CMovable {
private:
	CPlayer(void);
	virtual ~CPlayer(void);

	static CPlayer *m_instance;

	mat4 m_translate;
	mat4 m_rotate;
	mat4 m_scale;

public:
	static CPlayer *GetInstance(void);
	void Destroy(void);
	int Render(void);
	int Load(void);

	virtual void Translate(vec4 vec);
	virtual void Rotate(vec3 axis, float angle);
	virtual void Scale(vec4 scale);
};

#endif
/* End of a file */
