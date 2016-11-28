#pragma once
#if !defined(__CMOVABLE_H)
#define __CMOVABLE

/**
 * \brief
 * Every class which can be moved, scaled, roated, should inherit this class.
 */
class CMovable {
public:
	enum Direction {
		FRONT = 0x00,
		BACK = 0x01,
		LEFT = 0x02,
		RIGHT = 0x04,
		MAX = 0xFF,
	};

	CMovable(void) {}
	virtual ~CMovable(void) {}

	virtual void Translate(Direction d, float amout) {}
	virtual void Rotate(vec3 axis, float angle) {}
	virtual void Scale(vec4 scale) {}
};

#endif
/* End of a file */
