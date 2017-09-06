#pragma once

#include <math.h>


#define Epsilon		1.0e-6f


bool IsZero(float checkNumber)
{
	return (static_cast<float>(fabs(checkNumber)) < Epsilon);
}