#pragma once
#if !defined(__CMISC_H)
#define __CMISC_H

#ifdef _MSC_VER
#define __func__ __FUNCTION__
#endif

#define StatusPrint() do { GLenum status; status = glGetError(); if (status != GL_NO_ERROR) cerr << __func__ << ":" << __LINE__ << ":" << status << endl; } while (0)

#define __OLD_GL	!IsGLVersion_3_1()

class CMisc {
private:
	static bool m_ver3_1;	// glPrimitiveRestartIndex

	CMisc(void);
	virtual ~CMisc(void);

public:
	static const char * const m_oldVertexShaderFile;
	static const char * const m_oldFragmentShaderFile;
	static const char * const m_vertexShaderFile;
	static const char * const m_fragmentShaderFile;

	static bool IsGLVersion_3_1(void);
	static void EnableVersion_3_1(void);
};

static inline bool IsGLVersion_3_1(void)
{
	return CMisc::IsGLVersion_3_1();
}

#endif
/* end of a file */
