/**
 * \brief
 * This class construct the MAZE.
 * Vertex list of a cube is stored in CVertices.
 * This class drawings cubes using "Instancing" method.
 */

#include <iostream>
#include <string.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "cgmath.h"

#include "CObject.h"
#include "CVertices.h"
#include "CShader.h"
#include "CBlock.h"

using namespace std;

CBlock *CBlock::m_instance = NULL;

CBlock::CBlock(void)
: m_geometry_updated(true)
, m_color_updated(true)
, m_loaded(false)
{
	int i;

	int y;
	int x;

	char map[][7] = {
		{ 1, 1, 1, 1, 1, 1, 1, },
		{ 0, 0, 0, 0, 0, 0, 1, },
		{ 1, 0, 1, 0, 1, 0, 1, },
		{ 1, 1, 1, 0, 0, 0, 1, },
		{ 1, 0, 1, 1, 1, 0, 1, },
		{ 1, 0, 0, 0, 0, 0, 1, },
		{ 1, 1, 0, 1, 1, 1, 1, },
	};

	m_iCount = 0;
	for (y = 0; y < 7; y++)
		for (x = 0; x < 7; x++)
			m_iCount += (map[y][x] == 1);

	m_offset = (vec4 *)malloc(sizeof(*m_offset) * m_iCount);
	if (m_offset == NULL) {
		cerr << "Failed to allocate heap for map" << endl;
		return;
	}

	i = 0;
	for (y = 0; y < 7; y++) {
		for (x = 0; x < 7; x++) {
			if (map[y][x] == 0)
				continue;

			m_offset[i][0] = (3 - x) * (BLOCK_WIDTH * 2);
			m_offset[i][1] = 0;
			m_offset[i][2] = (3 - y) * (BLOCK_WIDTH * 2);
			m_offset[i][3] = 1.0f;
			i++;
		}
	}

	cout << m_iCount << " instances are created " << i << endl;
	glGenBuffers(1, &m_VBO);
}

CBlock::~CBlock(void)
{
	free(m_offset);
	glDeleteBuffers(1, &m_VBO);
}

CBlock *CBlock::GetInstance(void)
{
	if (!m_instance) {
		try {
			m_instance = new CBlock();
		}
		catch (...) {
			return NULL;
		}
	}
	
	return m_instance;
}

void CBlock::Destroy(void)
{
	m_instance = NULL;
	delete this;
}

int CBlock::Load(void)
{
	CVertices::GetInstance()->BindVAO();
	UpdateOffset();
	CVertices::GetInstance()->UnbindVAO();

	m_loaded = true;
	return 0;
}

int CBlock::UpdateOffset(void)
{
	GLint offset;

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	if (glGetError() != GL_NO_ERROR)
		cerr << __func__ << ":" << __LINE__ << endl;

	glBufferData(GL_ARRAY_BUFFER, sizeof(*m_offset) * m_iCount, m_offset, GL_STATIC_DRAW);
	offset = glGetAttribLocation(CShader::GetInstance()->GetProgram(), "offset");
	cout << "offset index: " << offset << endl;
	if (offset >= 0) {
		glEnableVertexAttribArray(offset);
		glVertexAttribPointer(offset, 4, GL_FLOAT, GL_FALSE,
			sizeof(float) * 4,
			(void *)0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(offset, 1);
	return 0;
}

int CBlock::Render(void)
{
	// Drawing blocks
	glDrawElementsInstanced(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_INT, 0, m_iCount);
	if (glGetError() != GL_NO_ERROR)
		cerr << __func__ << ":" << __LINE__ << endl;

	return 0;
}

/* End of a file */
