#pragma once
#if !defined(__CVERTICES_H)
#define __CVERTICES_H

#define BLOCK_WIDTH 4.0f

class CVertices {
private:
	enum VBO {
		VERTEX = 0x00,
		INDICES = 0x01,
		MAX = 0x02
	};

	GLuint m_VAO; // Array Object
	GLuint m_VBO[MAX]; // Buffer Object

	static struct {
		vec3 vertex;
		union {
			vec2 uv;
			vec4 color;
		};
	} m_vertices[18];
	static const GLuint m_indices[50];

	int UpdateColor(void);
	int UpdateVertices(void);
	int UpdateIndices(void);
	CVertices(void);
	virtual ~CVertices(void);

	static CVertices *m_instance;

public:

	static CVertices *GetInstance(void);
	void Destroy(void);

	int Load(void);

	int BindVAO(void); // Array
	int UnbindVAO(void);

	int BindEBO(void); // Indices
	int UnbindEBO(void);
};

#endif
/* End of a file */
