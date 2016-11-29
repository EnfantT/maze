#pragma once

#if !defined(_CSHADER_H)
#define _CSHADER_H

class CShader {
private:
	GLuint m_program;
	GLint m_mvpId;
	GLboolean m_mvpUpdated;

	static CShader *m_pInstance;

	char *ReadFile(const char *filename);
	GLuint LoadNCompile(GLenum type, const char *code);

	CShader(void);
	virtual ~CShader(void);

public:
	static CShader *GetInstance(void);
	void Destroy(void);

	int Load(const char *vFile = NULL, const char *fFile = NULL);

	GLint MVPId(void);
	GLuint Program(void);
	void UseProgram(void);
};

#endif
/* End of a file */
