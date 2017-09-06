#pragma once

class Vector3
{
public:
	Vector3()
		:mX(0), mY(0), mZ(0)
	{};

	Vector3(float _x, float _y, float _z)
		:mX(_x), mY(_y), mZ(_z)
	{};

	Vector3(const Vector3& other)
		: mX(other.mX), mY(other.mY), mZ(other.mZ)
	{};
	
	~Vector3() {};


	Vector3& operator =(const Vector3& other)
	{
		if (this == &other)
		{
			return *this;
		}

		mX = other.mX;
		mY = other.mY;
		mZ = other.mZ;

		return *this;
	}

	bool operator ==(const Vector3& other)
	{
		if (mX == other.mX && mY == other.mY && mZ == other.mZ)
		{
			return true;
		}

		return false;
	}

	Vector3 operator-(const Vector3& other);
	friend Vector3 operator- (const Vector3& a, const Vector3& b);

	friend Vector3 operator+ (const Vector3& a, const Vector3& b);

	Vector3 operator*(float scalar);
	friend Vector3 operator*(float scalar, const Vector3& a);
	friend Vector3 operator*(const Vector3& a, float scalar);


	float Length() const;
	float LengthSquared() const;

	float Distance(const Vector3& other);
	float DistanceSquared(const Vector3& other);

	void Normalize();
	float Dot(const Vector3& other) const;
	Vector3 Cross(const Vector3& other) const;

	void Rotate(float radian, const Vector3& center);

private:
	float mX, mY, mZ;	
};