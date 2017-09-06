#include "Capsule.h"

bool Capsule::Intersect(const Capsule & other) const
{
	float radiusSum = mRadius + other.GetRadius();

	float s, t;
	float distancesq = mSegment.DistanceSquared(other.GetSegment(), s, t);

	return ( distancesq <= radiusSum*radiusSum );
}
