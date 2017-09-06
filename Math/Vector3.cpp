#include "Vector3.h"

#include "MathCommon.h"


float Vector3::Length() const
{
	return sqrtf(mX*mX + mY*mY + mZ*mZ);
}

float Vector3::LengthSquared() const
{
	return mX*mX + mY*mY + mZ*mZ;
}

float Vector3::Distance(const Vector3 & other)
{
	float subX = mX - other.mX;
	float subY = mY - other.mY;
	float subZ = mZ - other.mZ;

	return sqrtf(subX * subX + subY * subY + subZ * subZ);
}

float Vector3::DistanceSquared(const Vector3 & other)
{
	float subX = mX - other.mX;
	float subY = mY - other.mY;
	float subZ = mZ - other.mZ;

	return (subX * subX + subY * subY + subZ * subZ);
}

void Vector3::Normalize()
{
	float lengthSquared = LengthSquared();

	if (IsZero(lengthSquared))
	{
		mX = 0.f, mY = 0.f, mZ = 0.f;
	}

	float invertSqrt = 1 / sqrtf(lengthSquared);

	mX = mX * invertSqrt;
	mY = mY * invertSqrt;
	mZ = mZ * invertSqrt;
}

float Vector3::Dot(const Vector3 & other) const
{
	return mX*other.mX + mY*other.mY + mZ*other.mZ;
}

Vector3 Vector3::Cross(const Vector3 & other) const
{
	return Vector3(mY*other.mX - mZ*other.mY,
		mZ*other.mX - mX*other.mZ,
		mX*other.mY - mY*other.mX );
}

void Vector3::Rotate(float radian, const Vector3& center)
{
	float cs = cos(radian);
	float sn = sin(radian);

	mX -= center.mX;
	mY -= center.mY;
	
	mX = mX*cs - mY*sn;
	mY = mX*sn + mY*cs;

	mX += center.mX;
	mY += center.mY;
}


Vector3 Vector3::operator-(const Vector3 & other)
{
	return Vector3(mX - other.mX, mY - other.mY, mZ - other.mZ);
}

Vector3 operator-(const Vector3 & a, const Vector3 & b)
{
	return Vector3(a.mX - b.mX, a.mY - b.mY, a.mZ - b.mZ);
}

Vector3 operator+(const Vector3 & a, const Vector3 & b)
{
	return Vector3(a.mX + b.mX, a.mY + b.mY, a.mZ + b.mZ);
}

Vector3 Vector3::operator*(float scalar)
{
	return Vector3(mX*scalar, mY*scalar, mZ*scalar);	
}

Vector3 operator*(float scalar, const Vector3 & a)
{
	return Vector3(a.mX*scalar, a.mY*scalar, a.mZ*scalar);
}

Vector3 operator*(const Vector3 & a, float scalar)
{
	return Vector3(a.mX*scalar, a.mY*scalar, a.mZ*scalar);
}