#pragma once
#if !defined(__CCOORDINATE_H)
#define __CCOORDINATE_H

class CCoordinate : public CObject {
private:
	CCoordinate(void);
	virtual ~CCoordinate(void);

	static CCoordinate *m_instance;
public:

	static CCoordinate *GetInstance(void);
	void Destroy(void);
	int Render(void);
	int Load(void);
};

#endif
/* End of a file */