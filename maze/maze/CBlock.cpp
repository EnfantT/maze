#include <iostream>
#include <string.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "linmath.h"

#include "CShader.h"
#include "CBlock.h"

using namespace std;

static const vec4 g_vertices[] = {
	{  0.5f,  0.5f,  0.5f, 1.0f },
	{ -0.5f,  0.5f,  0.5f, 1.0f },
	{ -0.5f, -0.5f,  0.5f, 1.0f },
	{  0.5f, -0.5f,  0.5f, 1.0f },
	{  0.5f, -0.5f, -0.5f, 1.0f },
	{  0.5f,  0.5f, -0.5f, 1.0f },
	{ -0.5f,  0.5f, -0.5f, 1.0f },
	{ -0.5f, -0.5f, -0.5f, 1.0f },
};

static const vec4 g_colors[] = {
	{ 1.0f, 0.0f, 0.0f, 1.0f },
	{ 1.0f, 0.0f, 0.0f, 1.0f },
	{ 1.0f, 0.0f, 0.0f, 1.0f },
	{ 1.0f, 0.0f, 0.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f },
};

static const GLuint g_indices[] = {
	0, 1, 2, 6, 7, 5, 4, 0, 3, 2, 4, 7,
	0, 5, 1, 6,
};

CBlock::CBlock(void)
: m_geometry_updated(true)
, m_color_updated(true)
, m_loaded(false)
{
	memcpy(m_vertices, g_vertices, sizeof(m_vertices));
	memcpy(m_colors, g_colors, sizeof(m_colors));
	memcpy(m_indices, g_indices, sizeof(m_indices));
}

CBlock::~CBlock(void)
{
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

	glBindVertexArray(m_VAO);

	UpdateGeometry();
	UpdateColor();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_STATIC_DRAW);

	glBindVertexArray(0);

	m_loaded = true;
	return 0;
}

int CBlock::Render(void)
{
	/**
	 * [nices]
	 * If the geometry is updated, this function will re-binding the vertices.
	 * If the color is updated, this function will re-binding the colors.
	 */
	UpdateGeometry();
	UpdateColor();

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLE_STRIP, 12, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (void *)(sizeof(GLuint) * 12));
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

	mat4x4 m;
	int i;

	/**
	 * Create translation matrix
	 */
	mat4x4_translate(m, x, y, z);

	/**
	 * Apply it to the all vertices.
	 */
	for (i = 0; i < 8; i++)
		mat4x4_mul_vec4(m_vertices[i], m, m_vertices[i]);

	return 0;
}

int CBlock::Rotate(float x, float y, float z, float angle)
{
	if (m_loaded)
		cerr << "Object is already loaded." << endl;

	mat4x4 m;
	int i;

	mat4x4_identity(m);
	mat4x4_rotate(m, m, x, y, z, angle);

	for (i = 0; i < 8; i++)
		mat4x4_mul_vec4(m_vertices[i], m, m_vertices[i]);

	return 0;
}

int CBlock::Rotate(bool x, bool y, bool z, float angle)
{
	mat4x4 m;
	int i;

	mat4x4_identity(m);

	if (x)
		mat4x4_rotate_X(m, m, angle);
	if (y)
		mat4x4_rotate_Y(m, m, angle);
	if (z)
		mat4x4_rotate_Z(m, m, angle);

	for (i = 0; i < 8; i++)
		mat4x4_mul_vec4(m_vertices[i], m, m_vertices[i]);

	return 0;
}

int CBlock::Scale(float x, float y, float z)
{
	if (m_loaded)
		cerr << "Object is already loaded." << endl;

	int i;

	for (i = 0; i < 8; i++) {
		if (x != 0.0)
			m_vertices[i][0] *= x;

		if (y != 0.0)
			m_vertices[i][1] *= y;

		if (z != 0.0)
			m_vertices[i][2] *= z;
	}

	return 0;
}

/* End of a file */
