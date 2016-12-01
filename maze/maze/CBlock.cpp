/**
 * \brief
 * This class construct the MAZE.
 * Vertex list of a cube is stored in CVertices.
 * This class drawings cubes using "Instancing" method.
 *
 * wall.png: http://orig06.deviantart.net/c87a/f/2013/275/e/5/brick_wall_by_arvin61r58-d6oxnt1.png
 * wall.jpg: http://thief.washboardabs.net/textures/stone/
 */

#include <iostream>
#include <string.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "cgmath.h"

#include "CMisc.h"
#include "CObject.h"
#include "CVertices.h"
#include "CShader.h"
#include "CBlock.h"
#include "CMovable.h"
#include "CModel.h"
#include "CPerspective.h"
#include "CView.h"
#include "CTexture.h"

using namespace std;

CBlock *CBlock::m_instance = NULL;
#define MAZE_SIZE 10

CBlock::CBlock(void)
: m_geometry_updated(true)
, m_color_updated(true)
, m_loaded(false)
{
	int i;

	int y;
	int x;

	char map[][MAZE_SIZE] = {
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 0, 0, 0, 1, 1, 0, 0, 0, 0, 1 },
		{ 1, 1, 0, 0, 0, 0, 1, 0, 1, 1 },
		{ 1, 1, 1, 1, 1, 0, 0, 0, 1, 1 },
		{ 1, 0, 0, 1, 1, 0, 1, 0, 0, 1 },
		{ 1, 1, 0, 1, 0, 0, 1, 1, 0, 1 },
		{ 1, 0, 0, 1, 1, 1, 1, 0, 0, 1 },
		{ 1, 0, 1, 1, 0, 0, 0, 0, 1, 1 },
		{ 1, 0, 0, 0, 0, 1, 1, 0, 0, 1 },
		{ 1, 1, 0, 1, 1, 1, 1, 1, 1, 1 }
	};

	m_iCount = 0;
	for (y = 0; y < MAZE_SIZE; y++)
		for (x = 0; x < MAZE_SIZE; x++)
			m_iCount += (map[y][x] == 1);

	try {
		m_offset = new vec4[m_iCount];
	} catch (...) {
		cerr << "Failed to allocate m_offset" << endl;
		m_iCount = 1;
		return;
	}

	i = 0;
	for (y = 0; y < MAZE_SIZE; y++) {
		for (x = 0; x < MAZE_SIZE; x++) {
			if (map[y][x] == 0)
				continue;

			m_offset[i][0] = (x - (MAZE_SIZE / 2)) * (BLOCK_WIDTH * 2);
			m_offset[i][1] = 0.0f;
			m_offset[i][2] = (y - (MAZE_SIZE / 2)) * (BLOCK_WIDTH * 2);
			m_offset[i][3] = 1.0f;
			i++;
		}
	}

	cout << m_iCount << " instances are created " << i << endl;
	glGenBuffers(1, &m_VBO);
}

CBlock::~CBlock(void)
{
	delete[] m_offset;
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
	if (__OLD_GL) {
		m_offsetId = glGetUniformLocation(CShader::GetInstance()->Program(), "offset");
		cout << "offset index: " << m_offsetId << endl;
	} else {
		CVertices::GetInstance()->BindVAO();
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		StatusPrint();

		glBufferData(GL_ARRAY_BUFFER, sizeof(*m_offset) * m_iCount, m_offset, GL_STATIC_DRAW);

		m_offsetId = glGetAttribLocation(CShader::GetInstance()->Program(), "offset");
		cout << "offset index: " << m_offsetId << endl;
		if (m_offsetId >= 0) {
			glEnableVertexAttribArray(m_offsetId);
			glVertexAttribPointer(m_offsetId, 4, GL_FLOAT, GL_FALSE,
				sizeof(GLfloat) * 4,
				0);
		}
		// note
		// glVertexAttribDivisor should be called before unmapping the VAO
		glVertexAttribDivisor(m_offsetId, 1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		CVertices::GetInstance()->UnbindVAO();

	}

	m_isBlockId = glGetUniformLocation(CShader::GetInstance()->Program(), "isBlock");
	cout << "isBlock index: " << m_isBlockId << endl;

	m_texImageId = CTexture::GetInstance()->Load("puzzle.jpg");
	if (m_texImageId == 0) {
		cerr << "Failed to create a texture image id" << endl;
	}
	else {
		GLint texId; // Texture Sampler 2D
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texImageId);

		texId = glGetUniformLocation(CShader::GetInstance()->Program(), "tex");
		glUniform1i(texId, 0);	// map GL_TEXTURE0
	}

	m_loaded = true;
	return 0;
}

int CBlock::Render(void)
{
	mat4 mvp;

	mvp = CPerspective::GetInstance()->Matrix() * CView::GetInstance()->Matrix() * CModel::GetInstance()->Matrix();

	glUniformMatrix4fv(CShader::GetInstance()->MVPId(), 1, GL_TRUE, (const GLfloat *)mvp);
	StatusPrint();

	glUniform1i(m_isBlockId, 1);

	// Drawing blocks
	if (__OLD_GL) {
		int i;

		for (i = 0; i < m_iCount; i++) {
			glUniform4f(m_offsetId, m_offset[i].x, m_offset[i].y, m_offset[i].z, m_offset[i].w);
			StatusPrint();
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			StatusPrint();
		}
	} else {
		glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, m_iCount);
		StatusPrint();
	}
	glUniform1i(m_isBlockId, 0);

	return 0;
}

/* End of a file */
