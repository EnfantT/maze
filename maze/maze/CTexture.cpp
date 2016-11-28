#include <iostream>

#include <glad/glad.h>

#include "cgmath.h"
#include "stb_image.h"
#include "CTexture.h"

using namespace std;

CTexture *CTexture::m_instance = NULL;

CTexture::CTexture(void)
{
	glEnable(GL_TEXTURE_2D);
	/**
	 * GL_LINEAR  : More smooth
	 * GL_NEAREST : More clear (pixelated look)
	 */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);
}

CTexture::~CTexture(void)
{
}

CTexture *CTexture::GetInstance(void)
{
	if (!m_instance) {
		try {
			m_instance = new CTexture();
		} catch (...) {
			return NULL;
		}
	}

	return m_instance;
}

/* End of a file */
