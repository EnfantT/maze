#include <iostream>
#include "glad/glad.h"

#include "cgmath.h"
#include "stb_image.h"

#include "CMisc.h"
#include "CTexture.h"

using namespace std;

CTexture *CTexture::m_instance = NULL;

CTexture::CTexture(void)
{
	glEnable(GL_TEXTURE_2D);
	/**
	 * GL_LINEAR  : More smooth
	 * GL_NEAREST : More clear (pixelated look)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	 */
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

GLuint CTexture::Load(const char *filename)
{
	unsigned char *image;
	int width;
	int height;
	int comp;
	GLuint texId;

	// load an image
	image = stbi_load(filename, &width, &height, &comp, 0);
	if (!image) {
		cerr << "Failed to load an image" << endl;
		return 0;
	}

	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	StatusPrint();

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	stbi_image_free(image); // release the original image
	glBindTexture(GL_TEXTURE_2D, 0);

	return texId;
}

/* End of a file */
