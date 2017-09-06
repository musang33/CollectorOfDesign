#pragma once

#include "Vector3.h"

class LineSegment3
{
public:
	LineSegment3(const Vector3& origin, const Vector3& direction)
		: mOrigin(origin)
		, mDirection(direction)
	{}

	
	float DistanceSquared(const LineSegment3& other, float & s_c, float & t_c) const;

	const Vector3& GetOrigin() const { return mOrigin; }
	const Vector3& GetDirection() const { return mDirection; }

private:
	Vector3		mOrigin;
	Vector3		mDirection;
};