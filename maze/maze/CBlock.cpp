#include <iostream>
#include <string.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "linmath.h"

#include "CShader.h"
#include "CBlock.h"

using namespace std;

const vec4 CBlock::m_vertices[] = {
	{  0.125f,  0.125f,  0.125f, 1.0f },
	{ -0.125f,  0.125f,  0.125f, 1.0f },
	{ -0.125f, -0.125f,  0.125f, 1.0f },
	{  0.125f, -0.125f,  0.125f, 1.0f },
	{  0.125f, -0.125f, -0.125f, 1.0f },
	{  0.125f,  0.125f, -0.125f, 1.0f },
	{ -0.125f,  0.125f, -0.125f, 1.0f },
	{ -0.125f, -0.125f, -0.125f, 1.0f },
};

const vec4 CBlock::m_colors[] = {
	{ 1.0f, 0.0f, 0.0f, 1.0f },
	{ 1.0f, 0.0f, 0.0f, 1.0f },
	{ 1.0f, 0.0f, 0.0f, 1.0f },
	{ 1.0f, 0.0f, 0.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f },
};

const GLuint CBlock::m_indices[] = {
	0, 1, 2, 6, 7, 5, 4, 0, 3, 2, 4, 7,
	0, 5, 1, 6,
};

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

			m_offset[i][0] = x * 0.25f;
			m_offset[i][1] = y * 0.25f;
			m_offset[i][2] = 0;
			m_offset[i][3] = 1.0f;
			i++;
		}
	}

	cout << m_iCount << " instances are created " << i << endl;
}

CBlock::~CBlock(void)
{
	free(m_offset);
}

int CBlock::UpdateGeometry(void)
{
	GLint position;

	if (!m_geometry_updated)
		return 0;

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[VERTEX]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);
	// [nicesj]
	// glGetAttribLocation function returns -1 if the given variable
	// has no reference in the shader.
	// And if it is a reserved name, such has prefix "gl_", returns -1 too.
	position = glGetAttribLocation(CShader::GetInstance()->GetProgram(), "position");
	cout << "position index: " << position << endl;
	if (position >= 0) {
		glEnableVertexAttribArray(position);
		// [nicesj]
		// Because of "stride" value, I lost 3 more hours.
		// I set it with wrong value (too big one), the triangle is disappeared.
		glVertexAttribPointer(position, 4, GL_FLOAT, GL_FALSE,
			sizeof(float) * 4,
			(void *)0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	m_geometry_updated = false;
	return 0;
}

int CBlock::UpdateOffset(void)
{
	GLint offset;

	glBindBuffer(GL_ARRAY_BUFFER, m_iVBO);
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

int CBlock::UpdateColor(void)
{
	GLint color;

	if (!m_color_updated)
		return 0;

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
	m_color_updated = false;

	return 0;
}

int CBlock::Load(void)
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(2, m_VBO);
	glGenBuffers(1, &m_EBO);
	glGenBuffers(1, &m_iVBO);

	glBindVertexArray(m_VAO);

	UpdateGeometry();
	UpdateColor();
	UpdateOffset();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_STATIC_DRAW);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	m_loaded = true;
	return 0;
}

int CBlock::Render(void)
{
	glBindVertexArray(m_VAO);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

	glDrawElementsInstanced(GL_TRIANGLE_STRIP, 12, GL_UNSIGNED_INT, 0, m_iCount);
	glDrawElementsInstanced(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (void *)(sizeof(GLuint) * 12), m_iCount);

//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return 0;
}

/**
 * [nicesj]
 * Below three functions are used for manipulate the block.
 * Block information will not be changed frequently.
 * But it could be updated.
 * So we give some method to do that.
 * After changing the block, it should be rebinded to the shader before rendering it.
 *
 * RECOMEND:
 *    Setup the geometry before Load.
 */
int CBlock::Transform(float x, float y, float z)
{
	/**
	 * Just for warn.
	 */
	if (m_loaded)
		cerr << "Object is already loaded." << endl;

	return 0;
}

int CBlock::Rotate(float x, float y, float z, float angle)
{
	if (m_loaded)
		cerr << "Object is already loaded." << endl;

	m_geometry_updated = true;
	return 0;
}

int CBlock::Rotate(bool x, bool y, bool z, float angle)
{
	if (m_loaded)
		cerr << "Object is already loaded." << endl;

	m_geometry_updated = true;
	return 0;
}

int CBlock::Scale(float x, float y, float z)
{
	if (m_loaded)
		cerr << "Object is already loaded." << endl;

	m_geometry_updated = true;
	return 0;
}

/* End of a file */
