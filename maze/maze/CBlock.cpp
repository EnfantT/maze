#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "linmath.h"

#include "CShader.h"
#include "CBlock.h"

using namespace std;

const vec4 CBlock::vertices[] = {
	{  0.5f,  0.5f,  0.5f, 1.0f },
	{ -0.5f,  0.5f,  0.5f, 1.0f },
	{ -0.5f, -0.5f,  0.5f, 1.0f },
	{  0.5f, -0.5f,  0.5f, 1.0f },
	{  0.5f, -0.5f, -0.5f, 1.0f },
	{  0.5f,  0.5f, -0.5f, 1.0f },
	{ -0.5f,  0.5f, -0.5f, 1.0f },
	{ -0.5f, -0.5f, -0.5f, 1.0f },
};

const vec4 CBlock::colors[] = {
	{ 1.0f, 0.0f, 0.0f, 1.0f },
	{ 1.0f, 0.0f, 0.0f, 1.0f },
	{ 1.0f, 0.0f, 0.0f, 1.0f },
	{ 1.0f, 0.0f, 0.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f },
};

const GLuint CBlock::indices[] = {
	0, 1, 2, 6, 7, 5, 4, 0, 3, 2, 4, 7,
	0, 5, 1, 6,
};

CBlock::CBlock(void)
{
}

CBlock::~CBlock(void)
{
}

int CBlock::Load(void)
{
	CShader *shader;
	GLint color;
	GLint position;

	shader = CShader::GetInstance();

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(2, m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[VERTEX]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// [nicesj]
	// glGetAttribLocation function returns -1 if the given variable
	// has no reference in the shader.
	// And if it is a reserved name, such has prefix "gl_", returns -1 too.
	position = glGetAttribLocation(shader->GetProgram(), "position");
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

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[COLOR]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	color = glGetAttribLocation(shader->GetProgram(), "color");
	cout << "color index: " << color << endl;
	if (color >= 0) {
		glEnableVertexAttribArray(color);
		glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE,
			sizeof(float) * 4,
			(void *)0);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	return 0;
}

int CBlock::Render(void)
{
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLE_STRIP, 12, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (void *)(sizeof(GLuint) * 12));
	glBindVertexArray(0);
	return 0;
}

/* End of a file */
