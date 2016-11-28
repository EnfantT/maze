#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "cgmath.h"

#include "CMisc.h"
#include "CShader.h"
#include "CVertices.h"

using namespace std;

const CVertices::VertexInfo CVertices::m_vertices[] = {
	{ vec3(BLOCK_WIDTH,  BLOCK_WIDTH,  BLOCK_WIDTH), vec4(0.0f, 1.0f, 1.0f, 1.0f) },
	{ vec3(BLOCK_WIDTH,  BLOCK_WIDTH,  BLOCK_WIDTH), vec4(1.0f, 0.0f, 1.0f, 1.0f) },
	{ vec3(BLOCK_WIDTH,  BLOCK_WIDTH,  BLOCK_WIDTH), vec4(1.0f, 1.0f, 1.0f, 1.0f) }, // 0

	{ vec3(-BLOCK_WIDTH,  BLOCK_WIDTH,  BLOCK_WIDTH), vec4(0.0f, 1.0f, 1.0f, 1.0f) },
	{ vec3(-BLOCK_WIDTH,  BLOCK_WIDTH,  BLOCK_WIDTH), vec4(1.0f, 1.0f, 1.0f, 1.0f) },
	{ vec3(-BLOCK_WIDTH,  BLOCK_WIDTH,  BLOCK_WIDTH), vec4(0.0f, 0.0f, 1.0f, 1.0f) }, // 1

	{ vec3(-BLOCK_WIDTH, -BLOCK_WIDTH,  BLOCK_WIDTH), vec4(0.0f, 0.0f, 1.0f, 1.0f) },
	{ vec3(-BLOCK_WIDTH, -BLOCK_WIDTH,  BLOCK_WIDTH), vec4(1.0f, 0.0f, 1.0f, 1.0f) }, // 2

	{ vec3(BLOCK_WIDTH, -BLOCK_WIDTH,  BLOCK_WIDTH), vec4(0.0f, 0.0f, 1.0f, 1.0f) }, 
	{ vec3(BLOCK_WIDTH, -BLOCK_WIDTH,  BLOCK_WIDTH), vec4(1.0f, 0.0f, 1.0f, 1.0f) }, // 3

	{ vec3(BLOCK_WIDTH, -BLOCK_WIDTH, -BLOCK_WIDTH), vec4(1.0f, 1.0f, 1.0f, 1.0f) },
	{ vec3(BLOCK_WIDTH, -BLOCK_WIDTH, -BLOCK_WIDTH), vec4(1.0f, 0.0f, 1.0f, 1.0f) }, // 4

	{ vec3(BLOCK_WIDTH,  BLOCK_WIDTH, -BLOCK_WIDTH), vec4(1.0f, 1.0f, 1.0f, 1.0f) }, // 5

	{ vec3(-BLOCK_WIDTH,  BLOCK_WIDTH, -BLOCK_WIDTH), vec4(0.0f, 1.0f, 1.0f, 1.0f) }, // 6

	{ vec3(-BLOCK_WIDTH, -BLOCK_WIDTH, -BLOCK_WIDTH), vec4(0.0f, 1.0f, 1.0f, 1.0f) },
	{ vec3(-BLOCK_WIDTH, -BLOCK_WIDTH, -BLOCK_WIDTH), vec4(0.0f, 0.0f, 1.0f, 1.0f) }, // 7 
	
	// 16
	{ vec3(-1000.0f, 0.0f, 0.0f), vec4(0.0f, 1.0f, 0.0f, 1.0f) },
	{ vec3(1000.0f, 0.0f, 0.0f), vec4(0.0f, 1.0f, 0.0f, 1.0f) }, // X axis
	{ vec3(0.0f, -1000.0f, 0.0f), vec4(0.0f, 0.0f, 1.0f, 1.0f) },
	{ vec3(0.0f, 1000.0f, 0.0f), vec4(0.0f, 0.0f, 1.0f, 1.0f) }, // Y axis
	{ vec3(0.0f, 0.0f, -1000.0f), vec4(0.0f, 1.0f, 1.0f, 1.0f) },
	{ vec3(0.0f, 0.0f, 1000.0f), vec4(0.0f, 1.0f, 1.0f, 1.0f) }, // Z axis
	{ vec3(1000.0f, -BLOCK_WIDTH, -1000.0f), vec4(0.9f, 0.8f, 0.7f, 1.0f) },
	{ vec3(1000.0f, -BLOCK_WIDTH, 1000.0f), vec4(0.9f, 0.8f, 0.7f, 1.0f) },
	{ vec3(-1000.0f, -BLOCK_WIDTH, 1000.0f), vec4(0.9f, 0.8f, 0.7f, 1.0f) },
	{ vec3(-1000.0f, -BLOCK_WIDTH, -1000.0f), vec4(0.9f, 0.8f, 0.7f, 1.0f) }, // Land
};

const GLuint CVertices::m_indices[] = {
	2, 3, 6, 
	6, 9, 2, // Front
	0, 8, 11,
	11, 12, 0, // Right
	15, 7, 4,
	4, 13, 15, // Left
	15, 13, 12,
	12, 11, 15, // Back 4 = 11, 5 = 12, 6 = 13, 7 = 15
	13, 5, 1,
	1, 12, 13, // Up
	6, 14, 10, 
	10, 9, 6, // Down   2 = 6, 3 = 9, 4 = 10, 7 = 14
	0xFFFFFFFF, // Cube (36, 0 - 35)
	16, 17,  0xFFFFFFFF, // (2, 37 - 38) X axis
	18, 19,  0xFFFFFFFF, // (2, 40 - 41) Y axis
	20, 21, 0xFFFFFFFF, // (2, 43 - 44) Z axis
	22, 25, 23, 24, 0xFFFFFFFF // (4, 46 - 49) Land
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
	GLint vertexId;
	GLint texCoordId;
	GLint colorId;

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

	vertexId = glGetAttribLocation(CShader::GetInstance()->Program(), "position");
	cout << "position index: " << vertexId << endl;
	if (vertexId >= 0) {
		glEnableVertexAttribArray(vertexId);
		glVertexAttribPointer(vertexId, 3, GL_FLOAT, GL_FALSE,
			sizeof(*m_vertices),
			(void *)0);
	}

	texCoordId = glGetAttribLocation(CShader::GetInstance()->Program(), "texCoord");
	cout << "texCoord index: " << texCoordId << endl;
	if (texCoordId >= 0) {
		glEnableVertexAttribArray(texCoordId);
		glVertexAttribPointer(texCoordId, 2, GL_FLOAT, GL_TRUE,
			sizeof(*m_vertices),
			(void *)sizeof(m_vertices->vertex));
	}

	colorId = glGetAttribLocation(CShader::GetInstance()->Program(), "color");
	cout << "color index: " << colorId << endl;
	if (colorId >= 0) {
		glEnableVertexAttribArray(colorId);
		glVertexAttribPointer(colorId, 4, GL_FLOAT, GL_FALSE,
			sizeof(*m_vertices),
			(void *)sizeof(m_vertices->vertex));
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return 0;
}

/* End of a file */
