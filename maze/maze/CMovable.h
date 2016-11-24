#pragma once
#if !defined(__CMOVABLE_H)
#define __CMOVABLE

class CMovable {
public:
	CMovable(void) {}
	virtual ~CMovable(void) {}

	virtual void Translate(vec4 vec) {}
	virtual void Rotate(vec3 axis, float angle) {}
	virtual void Scale(vec4 scale) {}
};

#endif
/* End of a file */