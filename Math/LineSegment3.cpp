#include "LineSegment3.h"

#include "Vector3.h"
#include "MathCommon.h"


float LineSegment3::DistanceSquared(const LineSegment3 & other, float & s_c, float & t_c) const
{
	// compute intermediate parameters
	Vector3 w0 = GetOrigin() - other.GetOrigin();
	float a = mDirection.Dot(GetDirection());
	float b = mDirection.Dot(other.GetDirection());
	float c = other.GetDirection().Dot(other.GetDirection());
	float d = mDirection.Dot(w0);
	float e = other.GetDirection().Dot(w0);

	float denom = a*c - b*b;
	// parameters to compute s_c, t_c
	float sn, sd, tn, td;

	// if denom is zero, try finding closest point on other to origin0
	if (IsZero(denom))
	{
		// clamp s_c to 0
		sd = td = c;
		sn = 0.0f;
		tn = e;
	}
	else
	{
		// clamp s_c within [0,1]
		sd = td = denom;
		sn = b*e - c*d;
		tn = a*e - b*d;

		// clamp s_c to 0
		if (sn < 0.0f)
		{
			sn = 0.0f;
			tn = e;
			td = c;
		}
		// clamp s_c to 1
		else if (sn > sd)
		{
			sn = sd;
			tn = e + b;
			td = c;
		}
	}

	// clamp t_c within [0,1]
	// clamp t_c to 0
	if (tn < 0.0f)
	{
		t_c = 0.0f;
		// clamp s_c to 0
		if (-d < 0.0f)
		{
			s_c = 0.0f;
		}
		// clamp s_c to 1
		else if (-d > a)
		{
			s_c = 1.0f;
		}
		else
		{
			s_c = -d / a;
		}
	}
	// clamp t_c to 1
	else if (tn > td)
	{
		t_c = 1.0f;
		// clamp s_c to 0
		if ((-d + b) < 0.0f)
		{
			s_c = 0.0f;
		}
		// clamp s_c to 1
		else if ((-d + b) > a)
		{
			s_c = 1.0f;
		}
		else
		{
			s_c = (-d + b) / a;
		}
	}
	else
	{
		t_c = tn / td;
		s_c = sn / sd;
	}

	// compute difference vector and distance squared
	Vector3 wc = w0 + GetDirection()*s_c - t_c*other.GetDirection();
	
	return wc.Dot(wc);	
}
