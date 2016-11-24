#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "cgmath.h"

#include "CShader.h"
#include "CVertices.h"

using namespace std;

const vec4 CVertices::m_vertices[] = {
	{ BLOCK_WIDTH,  BLOCK_WIDTH,  BLOCK_WIDTH, 1.0f },
	{ -BLOCK_WIDTH,  BLOCK_WIDTH,  BLOCK_WIDTH, 1.0f },
	{ -BLOCK_WIDTH, -BLOCK_WIDTH,  BLOCK_WIDTH, 1.0f },
	{ BLOCK_WIDTH, -BLOCK_WIDTH,  BLOCK_WIDTH, 1.0f },
	
	{ BLOCK_WIDTH, -BLOCK_WIDTH, -BLOCK_WIDTH, 1.0f },
	{ BLOCK_WIDTH,  BLOCK_WIDTH, -BLOCK_WIDTH, 1.0f },
	{ -BLOCK_WIDTH,  BLOCK_WIDTH, -BLOCK_WIDTH, 1.0f },
	{ -BLOCK_WIDTH, -BLOCK_WIDTH, -BLOCK_WIDTH, 1.0f }, // Vertices for a Cube modeling.

	{ -1000.0f, 0.0f, 0.0f, 1.0f },
	{ 1000.0f, 0.0f, 0.0f, 1.0f }, // X axis
	{ 0.0f, -1000.0f, 0.0f, 1.0f },
	{ 0.0f, 1000.0f, 0.0f, 1.0f }, // Y axis
	{ 0.0f, 0.0f, -1000.0f, 1.0f },
	{ 0.0f, 0.0f, 1000.0f, 1.0f }, // Z axis
};

const vec4 CVertices::m_colors[] = {
	{ 1.0f, 0.0f, 0.0f, 1.0f },
	{ 1.0f, 0.0f, 0.0f, 1.0f },
	{ 1.0f, 0.0f, 0.0f, 1.0f },
	{ 1.0f, 0.0f, 0.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f },

	{ 0.0f, 1.0f, 0.0f, 1.0f },
	{ 0.0f, 1.0f, 0.0f, 1.0f }, // X axis

	{ 0.0f, 0.0f, 1.0f, 1.0f },
	{ 0.0f, 0.0f, 1.0f, 1.0f }, // Y axis

	{ 0.0f, 1.0f, 1.0f, 1.0f },
	{ 0.0f, 1.0f, 1.0f, 1.0f }, // Z axis
};

const GLuint CVertices::m_indices[] = {
	  0,  1,  2, 6, 7,
	  5,  4,  0, 3, 2,
	  4,  7,  0xFFFFFFFF,
	  0,  5,  1, 6, 0xFFFFFFFF,
	  0,  1,  2, 0xFFFFFFFF,

	  8,  9,  0xFFFFFFFF, // X axis
	 10, 11,  0xFFFFFFFF, // Y axis
	 12, 13, // Z axis

};

CVertices *CVertices::m_instance = NULL;

CVertices::CVertices(void)
{
	glEnable(GL_PRIMITIVE_RESTART);

	glPrimitiveRestartIndex(0xFFFFFFFF);

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
	if (glGetError() != GL_NO_ERROR)
		cerr << __func__ << ":" << __LINE__ << endl;

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
	if (glGetError() != GL_NO_ERROR)
	cerr << __func__ << ":" << __LINE__ << endl;

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
	position = glGetAttribLocation(CShader::GetInstance()->GetProgram(), "position");
	cout << "position index: " << position << endl;
	if (position >= 0) {
		glEnableVertexAttribArray(position);
		glVertexAttribPointer(position, 4, GL_FLOAT, GL_FALSE,
			sizeof(float) * 4,
			(void *)0);
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
	color = glGetAttribLocation(CShader::GetInstance()->GetProgram(), "color");
	cout << "color index: " << color << endl;
	if (color >= 0) {
		glEnableVertexAttribArray(color);
		glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE,
			sizeof(float) * 4,
			(void *)0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return 0;
}

/* End of a file */