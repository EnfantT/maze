#pragma once
#if !defined(__CMODEL_H)
#define __CMODEL_H

class CModel : public CMovable {
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

	virtual void Translate(CMovable::Direction d, float amount);
	virtual void Rotate(vec3 axis, float angle);
	virtual void Scale(vec4 scale);
};

#endif
/* End of a file */
