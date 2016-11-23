#pragma once

#if !defined(_CSHADER_H)
#define _CSHADER_H

class CShader {
private:
	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_EBO;
	GLuint m_program;
	GLint m_mvp;
	GLboolean m_mvpUpdated;
	mat4 m_mvpMatrix;

	static const char * const m_vertCode;
	static const char * const m_fragCode;
	static CShader *m_pInstance;

	int LoadNCompile(GLenum type, const char *code);

	CShader(void);
	virtual ~CShader(void);

public:
	static CShader *GetInstance(void);
	void Destroy(void);

	int Load(void);
	int Map(void);
	int Unload(void);
	int ApplyMVP(void);

	GLuint GetProgram(void);

	// Model
	int Translate(float x, float y, float z);
	int Scale(float x, float y, float z, float scale);
	int Rotate(float x, float y, float z, float angle);

	// View (Rotate, Translate)

	// Project (frustrum)
};

#endif
/* End of a file */
