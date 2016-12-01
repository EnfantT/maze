#pragma once
#include "cgmath.h"

struct trackball
{
	bool	bTracking;
	float	scale;			// controls how much rotation is applied
	mat4	view_matrix0;	// initial view matrix, current view matrix
	vec2	m0;				// the last mouse position

	trackball( float rot_scale=1.0f ):bTracking(false),scale(rot_scale){}

	void end(){ bTracking=false; }
	void begin( const mat4& view_matrix, float x, float y )
	{
		bTracking = true;			// enable trackball tracking
		m0 = vec2(x,y)*2.0f-1.0f;	// convert (x,y) in [0,1] to [-1,1]
		view_matrix0 = view_matrix;	// save current view matrix
	}
	
	mat4 update( float x, float y )
	{
		// project a 2D mouse position to a unit sphere
		static const vec3 p0 = vec3(0,0,1.0f);						// original position of the camera
		vec3 p1 = vec3(x*2.0f-1.0f-m0.x, m0.y-y*2.0f+1.0f,0);		// calculate displacement with vertical y-swapping
		if(!bTracking||length(p1)<0.0001f) return view_matrix0;		// ignore subtle movement
		p1 = vec3(p1.x*scale,p1.y*scale,sqrtf(max(0.0f,1.0f-length(p1)))).normalize();	// adjust z to make unit sphere

		// find rotation axis and angle (with inverse view rotation to the world coordinate)
		vec3 n = p0.cross(p1)*((mat3)view_matrix0);
		float angle = asin( min(n.length(),0.999f) );
		
		// return resulting rotation matrix
		return view_matrix0 * mat4::rotate(n.normalize(),angle);
	}
};