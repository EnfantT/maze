#pragma once
#if !defined(__CBLOCK_H)
#define __CBLOCK_H

#include "CObject.h"

class CBlock : public CObject {
private:
	enum VBO {
		VERTEX = 0x00,
		COLOR = 0x01,
		MAX = 0x02
	};
	GLuint m_VAO; // Array Object
	GLuint m_VBO[MAX]; // Buffer Object
	GLuint m_EBO; // Element Object
	GLuint m_iVBO; // Instanced VBO - offset

	static const vec4 m_vertices[8];
	static const vec4 m_colors[8];
	static const GLuint m_indices[17];

	vec4 *m_offset;
	int m_iCount;

	bool m_geometry_updated;
	bool m_color_updated;

	bool m_loaded;

	int UpdateGeometry(void);
	int UpdateColor(void);
	int UpdateOffset(void);


public:
	CBlock(void);
	virtual ~CBlock(void);

	int Load(void);
	int Render(void);

	int Transform(float x, float y, float z);
	int Rotate(float x, float y, float z, float angle);
	int Rotate(bool x, bool y, bool z, float angle);

	int Scale(float x, float y, float z);
};

#endif

/* End of a file */
