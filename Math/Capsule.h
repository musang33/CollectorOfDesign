#include "Vector3.h"
#include "LineSegment3.h"


class Capsule
{
public:
	Capsule(const Vector3& orgin, const Vector3& direction, float radius)
		: mSegment( orgin, direction )
		, mRadius(radius)
	{}

	bool Intersect(const Capsule& other) const;

public:
	float GetRadius() const { return mRadius; }
	const LineSegment3& GetSegment() const { return mSegment; };
private:
	LineSegment3	mSegment;
	float			mRadius;

};