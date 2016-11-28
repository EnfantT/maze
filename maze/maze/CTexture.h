#pragma once
#if !defined(__CTEXTURE_H)
#define __CTEXTURE_H

class CTexture {
private:
	CTexture(void);
	virtual ~CTexture(void);

	static CTexture *m_instance;
public:
	static CTexture *GetInstance(void);
};

#endif
/* End of a file */

