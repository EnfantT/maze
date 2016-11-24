#pragma once
#if !defined(__CPLAYER_H)
#define __CPLAYER_H

class CPlayer : public CObject, public CMovable {
private:
	CPlayer(void);
	virtual ~CPlayer(void);

	static CPlayer *m_instance;

	vec4 m_playerOffset;
	GLint m_isPlayer;
	GLint m_player;

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
