#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "cgmath.h"

#include "CMisc.h"
#include "CShader.h"
#include "CVertices.h"

using namespace std;

const vec4 CVertices::m_vertices[] = {
	{
		.vertex = { BLOCK_WIDTH,  BLOCK_WIDTH,  BLOCK_WIDTH },
		.uv = { 1.0f, 1.0f },
	},
	{
		.vertex = { -BLOCK_WIDTH,  BLOCK_WIDTH,  BLOCK_WIDTH },
		.uv = { 0.0f, 1.0f },
	},
	{
		.vertex = { -BLOCK_WIDTH, -BLOCK_WIDTH,  BLOCK_WIDTH },
		.uv = { 0.0f, 0.0f },
	},
	{
		.vertex = { BLOCK_WIDTH, -BLOCK_WIDTH,  BLOCK_WIDTH },
		.uv = { 1.0f, 0.0f },	// 5 * 4 = 20
	},

	{
		.vertex = { BLOCK_WIDTH, -BLOCK_WIDTH, -BLOCK_WIDTH },
		.uv = { 0.0f, 0.0f },
	},
	{
		.vertex = { BLOCK_WIDTH,  BLOCK_WIDTH, -BLOCK_WIDTH },
		.color = { 1.0f, 1.0f },
	},
	{
		.vertex = { -BLOCK_WIDTH,  BLOCK_WIDTH, -BLOCK_WIDTH },
		.color = { 0.0f, 1.0f },
	},
	{
		.vertex = { -BLOCK_WIDTH, -BLOCK_WIDTH, -BLOCK_WIDTH }, // Vertices for a Cube modeling.
		.color = { 0.0f, 0.0f },
	},

	{
		.vertex = { -1000.0f, 0.0f, 0.0f },
		.color = { 0.0f, 1.0f, 0.0f, 1.0f },
	},
	{
		.vertex = { 1000.0f, 0.0f, 0.0f }, // X axis
		.color = { 0.0f, 1.0f, 0.0f, 1.0f }, // X axis
	},
	{
		.vertex = { 0.0f, -1000.0f, 0.0f },
		.color = { 0.0f, 0.0f, 1.0f, 1.0f },
	},
	{
		.vertex = { 0.0f, 1000.0f, 0.0f }, // Y axis
		.color = { 0.0f, 0.0f, 1.0f, 1.0f }, // Y axis
	},
	{
		.vertex = { 0.0f, 0.0f, -1000.0f },
		.color = { 0.0f, 1.0f, 1.0f, 1.0f },
	},
	{
		.vertex = { 0.0f, 0.0f, 1000.0f }, // Z axis
		.color = { 0.0f, 1.0f, 1.0f, 1.0f }, // Z axis
	},
	{
		.vertex = { 100.0f, -BLOCK_WIDTH, -100.0f },
		.color = { 0.9f, 0.8f, 0.7f, 1.0f },
	},
	{
		.vertex = { 100.0f, -BLOCK_WIDTH, 100.0f },
		.color = { 0.9f, 0.8f, 0.7f, 1.0f },
	},
	{
		.vertex = { -100.0f, -BLOCK_WIDTH, 100.0f },
		.color = { 0.9f, 0.8f, 0.7f, 1.0f },
	},
	{
		.vertex = { -100.0f, -BLOCK_WIDTH, -100.0f }, // Land
		.color = { 0.9f, 0.8f, 0.7f, 1.0f }, // Land
	},
};

const GLuint CVertices::m_indices[] = {
	  0,  1,  2, 6, 7,
	  5,  4,  0, 3, 2,
	  4,  7,  0xFFFFFFFF,
	  0,  5,  1, 6, 0xFFFFFFFF, // cube (18, 0 - 17)

	  0,  1,  2, 6, 7,
	  5,  4,  0, 3, 2,
	  4,  7,  0xFFFFFFFF,
	  0,  5,  1, 6, 0xFFFFFFFF, // cube (18, 18 - 35)

	  8,  9,  0xFFFFFFFF, // (3, 36 - 38) X axis

	  10, 11,  0xFFFFFFFF, // (3, 39 - 41) Y axis

	  12, 13, 0xFFFFFFFF, // (3, 42 - 44) Z axis

	  14, 17, 15, 16, 0xFFFFFFFF, // (4, 45 - 48) Land
};

CVertices *CVertices::m_instance = NULL;

CVertices::CVertices(void)
{
	if (__OLD_GL) {
		glEnable(GL_PRIMITIVE_RESTART_FIXED_INDEX);
		StatusPrint();
	} else {
		glEnable(GL_PRIMITIVE_RESTART);
		glPrimitiveRestartIndex(0xFFFFFFFF);
	}

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(MAX, m_VBO);
}

CVertices::~CVertices(void)
{
	glDeleteBuffers(1, &m_VAO);
	glDeleteBuffers(MAX, m_VBO);
}

CVertices *CVertices::GetInstance(void)
{
	if (!m_instance) {
		try {
			m_instance = new CVertices();
		}
		catch (...) {
			return NULL;
		}
	}
	return m_instance;
}

void CVertices::Destroy(void)
{
	m_instance = NULL;
	delete this;
}

int CVertices::BindEBO(void)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO[INDICES]);
	StatusPrint();

	return 0;
}

int CVertices::UnbindEBO(void)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return 0;
}

int CVertices::BindVAO(void)
{
	glBindVertexArray(m_VAO);
	StatusPrint();

	return 0;
}

int CVertices::UnbindVAO(void)
{
	glBindVertexArray(0);
	return 0;
}

int CVertices::Load(void)
{
	BindVAO();
	UpdateColor();
	UpdateVertices();
	UpdateIndices();
	UnbindVAO();
	return 0;
}

int CVertices::UpdateIndices(void)
{
	BindEBO();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_STATIC_DRAW);
	UnbindEBO();
	return 0;
}

int CVertices::UpdateVertices(void)
{
	GLint position;
	GLint color;

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[VERTEX]);
	/**
	* GL_STATIC_DRAW,
	* GL_DYNAMIC_DRAW,
	*
	* The geometry or color can be changed.
	* But very rarely, the information will be updated.
	* So we do not use the dynamic_draw from here.
	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);

	position = glGetAttribLocation(CShader::GetInstance()->Program(), "position");
	cout << "position index: " << position << endl;
	if (position >= 0) {
		glEnableVertexAttribArray(position);
		glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE,
			sizeof(*m_vertices),
			(void *)0);
	}

	color = glGetAttribLocation(CShader::GetInstance()->Program(), "texCoord");
	cout << "color index: " << color << endl;
	if (color >= 0) {
		glEnableVertexAttribArray(color);
		glVertexAttribPointer(color, 2, GL_FLOAT, GL_TRUE,
			sizeof(*m_vertices),
			(void *)sizeof(m_vertices->vertex));
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return 0;
}

int CVertices::UpdateColor(void)
{
	GLint color;

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[COLOR]);
	/**
	* GL_STATIC_DRAW,
	* GL_DYNAMIC_DRAW,
	*
	* The geometry or color can be changed.
	* But very rarely, the information will be updated.
	* So we do not use the dynamic_draw from here.
	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_colors), m_colors, GL_STATIC_DRAW);

	color = glGetAttribLocation(CShader::GetInstance()->Program(), "color");
	cout << "color index: " << color << endl;
	if (color >= 0) {
		glEnableVertexAttribArray(color);
		glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE,
			sizeof(GLfloat) * 4,
			(void *)0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return 0;
}

/* End of a file */
