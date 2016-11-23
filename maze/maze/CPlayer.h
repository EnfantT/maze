#pragma once
#if !defined(__CPLAYER_H)
#define __CPLAYER_H

class CPlayer : public CObject {
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

	int Translate(vec4 offset);
};

#endif
/* End of a file */
