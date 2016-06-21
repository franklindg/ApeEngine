#include "ApeFloat4.h"

namespace AML {

	APEFLOAT4::APEFLOAT4(const float& x, const float& y, const float& z, const float& w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	APEFLOAT4& APEFLOAT4::Add(const APEFLOAT4& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.z;

		return *this;
	}

	APEFLOAT4& APEFLOAT4::Subtract(const APEFLOAT4& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;

		return *this;
	}

	APEFLOAT4& APEFLOAT4::Multiply(const APEFLOAT4& other)
	{
		x *= other.x;
		y *= other.y;
		w *= other.z;
		w *= other.w;

		return *this;
	}

	APEFLOAT4& APEFLOAT4::Divide(const APEFLOAT4& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		w /= other.w;

		return *this;
	}

	APEFLOAT4 operator+(APEFLOAT4 first, const APEFLOAT4& second)
	{
		return first.Add(second);
	}

	APEFLOAT4 operator-(APEFLOAT4 first, const APEFLOAT4& second)
	{
		return first.Subtract(second);
	}

	APEFLOAT4 operator*(APEFLOAT4 first, const APEFLOAT4& second)
	{
		return first.Multiply(second);
	}

	APEFLOAT4 operator/(APEFLOAT4 first, const APEFLOAT4& second)
	{
		return first.Divide(second);
	}

	APEFLOAT4& APEFLOAT4::operator+=(const APEFLOAT4& other)
	{
		return Add(other);
	}

	APEFLOAT4& APEFLOAT4::operator-=(const APEFLOAT4& other)
	{
		return Subtract(other);
	}

	APEFLOAT4& APEFLOAT4::operator*=(const APEFLOAT4& other)
	{
		return Multiply(other);
	}

	APEFLOAT4& APEFLOAT4::operator/=(const APEFLOAT4& other)
	{
		return Divide(other);
	}

	bool APEFLOAT4::operator==(const APEFLOAT4& other)
	{
		return x == other.x && y == other.y && z == other.z && w == other.w;
	}

	bool APEFLOAT4::operator!=(const APEFLOAT4& other)
	{
		return !(*this == other);
	}
}