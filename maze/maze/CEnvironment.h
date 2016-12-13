#pragma once
#if !defined(__CENVIRONMENT_H)
#define __CENVIRONMENT_H

class CEnvironment : public CObject {
private:
	CEnvironment(void);
	virtual ~CEnvironment(void);
	static CEnvironment *m_instance;
	GLint m_isEnvIdx;
	GLuint m_FloortexImageId;
	GLuint m_VBO; // Vertex Buffer Object

public:
	static CEnvironment *GetInstance(void);
	void Destroy(void);

	int Load(void);
	int Render(void);
};

#endif
/* End of a file */