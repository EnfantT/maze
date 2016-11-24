#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "cgmath.h"

#include "CObject.h"
#include "CMovable.h"
#include "CVertices.h"
#include "CShader.h"
#include "CPlayer.h"

using namespace std;

CPlayer *CPlayer::m_instance = NULL;

CPlayer::CPlayer(void)
: m_playerOffset(0.0f, 0.0f, 0.0f, 1.0f)
, m_isPlayer(true)
, m_player(0)
{
}

CPlayer::~CPlayer(void)
{

}

CPlayer *CPlayer::GetInstance(void)
{
	if (!m_instance) {
		try {
			m_instance = new CPlayer();
		}
		catch (...) {
			return NULL;
		}
	}

	return m_instance;
}

void CPlayer::Destroy(void)
{
	m_instance = NULL;
	delete this;
}

int CPlayer::Render(void)
{
	// Only if the player offset is updated.
	glUniform4f(m_player, m_playerOffset.x, m_playerOffset.y, m_playerOffset.z, m_playerOffset.w);
	glUniform1i(m_isPlayer, 1);

	// Drawing a player
	glDrawElements(GL_TRIANGLE_FAN, 3, GL_UNSIGNED_INT, (void *)(18 * sizeof(GL_FLOAT)));
	if (glGetError() != GL_NO_ERROR)
		cerr << __func__ << ":" << __LINE__ << endl;

	glUniform1i(m_isPlayer, 0);

	return 0;
}

int CPlayer::Load(void)
{
	m_isPlayer = glGetUniformLocation(CShader::GetInstance()->GetProgram(), "isPlayer");
	if (glGetError() != GL_NO_ERROR)
		cerr << __func__ << ":" << __LINE__ << endl;
	else
		glEnableVertexAttribArray(m_isPlayer);

	cout << "isPlayer index: " << m_isPlayer << endl;

	m_player = glGetUniformLocation(CShader::GetInstance()->GetProgram(), "playerOffset");
	if (glGetError() != GL_NO_ERROR)
		cerr << __func__ << ":" << __LINE__ << endl;
	else
		glEnableVertexAttribArray(m_player);

	cout << "playerOffset index: " << m_player << endl;

	return 0;
}

void CPlayer::Translate(vec4 vec)
{
	m_playerOffset = mat4::translate(vec.x, vec.y, vec.z) * m_playerOffset;
}

void CPlayer::Rotate(vec3 axis, float angle)
{
	m_playerOffset = mat4::rotate(axis, angle) * m_playerOffset;
}

void CPlayer::Scale(vec4 scale)
{
	m_playerOffset = mat4::scale(scale.x, scale.y, scale.z) * m_playerOffset;
}

/* End of a file */