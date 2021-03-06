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

GLuint CTexture::Load()
{
	unsigned char *image0;
	unsigned char *image1;
	
	int width;
	int height;
	int comp;
	GLuint texId[2];
	

	// load an image
	image0 = stbi_load("test.jpg", &width, &height, &comp, 0);
	image1 = stbi_load("wall.jpg", &width, &height, &comp, 0);
	
	if (!image0 && !image1) {
		cerr << "Failed to load an image" << endl;
		return 0;
	}

	glGenTextures(1, &texId[0]);
	glBindTexture(GL_TEXTURE_2D, texId[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image0);
	StatusPrint();
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glActiveTexture(GL_TEXTURE0);

	glGenTextures(1, &texId[1]);
	glBindTexture(GL_TEXTURE_2D, texId[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);
	StatusPrint();
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glActiveTexture(GL_TEXTURE1);
	

	stbi_image_free(image0); // release the original image
	stbi_image_free(image1); // release the original image
	
//	glBindTexture(GL_TEXTURE_2D, 0);

	return 1;
}

/* End of a file */
