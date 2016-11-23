#pragma once
#if !defined(__CMODEL_H)
#define __CMODEL_H

class CModel {
private:
	mat4 m_model;
	static CModel *m_instance;
	bool m_updated;

	CModel(void);
	virtual ~CModel(void);

public:
	static CModel *GetInstance(void);
	void Destroy(void);
	bool Updated(void);
	mat4 Matrix(void);
};

#endif
/* End of a file */