#pragma once
#if !defined(__STATE_H)
#define __STATE_H

class State {
private:
	int m_curDir;
	double m_distance;
	unsigned int m_texId;
	unsigned int m_texFloor;
	int m_pathLen;
	int m_myLen;
public:
	State();
	~State();
};

#endif
/* End of a file */
