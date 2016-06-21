#pragma once
#ifndef APEFLOAT4_H
#define APEFLOAT4_H

namespace AML {

	// APEFLOAT4: 4 - Elements.
	struct APEFLOAT4
	{
		float x, y, z, w;

		APEFLOAT4() = default;
		APEFLOAT4(const float& x, const float& y, const float& z, const float& w);

		APEFLOAT4& Add(const APEFLOAT4& other);
		APEFLOAT4& Subtract(const APEFLOAT4& other);
		APEFLOAT4& Multiply(const APEFLOAT4& other);
		APEFLOAT4& Divide(const APEFLOAT4& other);

		friend APEFLOAT4 operator+(APEFLOAT4 first, const APEFLOAT4& second);
		friend APEFLOAT4 operator-(APEFLOAT4 first, const APEFLOAT4& second);
		friend APEFLOAT4 operator*(APEFLOAT4 first, const APEFLOAT4& second);
		friend APEFLOAT4 operator/(APEFLOAT4 first, const APEFLOAT4& second);

		bool operator==(const APEFLOAT4& other);
		bool operator!=(const APEFLOAT4& other);

		APEFLOAT4& operator+=(const APEFLOAT4& other);
		APEFLOAT4& operator-=(const APEFLOAT4& other);
		APEFLOAT4& operator*=(const APEFLOAT4& other);
		APEFLOAT4& operator/=(const APEFLOAT4& other);
	};
}
#endif