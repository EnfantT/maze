#pragma once
#if !defined(__CBLOCK_H)
#define __CBLOCK_H

class CBlock : public CObject {
private:
	vec4 *m_offset;
	GLuint m_VBO; // Vertex Buffer Object
	GLuint m_texImageId;
	GLint m_offsetId;
	GLint m_isBlockId;
	int m_iCount;

	bool m_geometry_updated;
	bool m_color_updated;
	bool m_loaded;	

	CBlock(void);
	virtual ~CBlock(void);

	static CBlock *m_instance;

public:

	static CBlock *GetInstance(void);
	void Destroy(void);
	void ChangeTex(void);
	int Load(void);
	int Render(void);
};

#endif

/* End of a file */
