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

	static const vec4 vertices[];
	static const vec4 colors[];
	static const GLuint indices[];

public:
	CBlock(void);
	virtual ~CBlock(void);

	int Load(void);
	int Render(void);
};

#endif

/* End of a file */
