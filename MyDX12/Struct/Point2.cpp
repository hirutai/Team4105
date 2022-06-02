#include "Point2.h"
#include <cmath>
using namespace Math;

Point2::Point2():a(0),b(0) {}

Point2::Point2(int _a, int _b):a(_a),b(_b) {}

float Math::Point2::length() const
{
	return sqrtf(powf(a, 2) + powf(b, 2));
}

Point2& Math::Point2::normalize()
{
	float len = length();
	if (len != 0)
	{
		return *this /= len;
	}
	return *this;
}

Point2 Math::Point2::operator+() const
{
	return *this;
}

Point2 Math::Point2::operator-() const
{
	return Point2(-a,-b);
}

Point2& Math::Point2::operator+=(const Point2& p)
{
	a += p.a;
	b += p.b;
	return *this;
}

Point2& Math::Point2::operator-=(const Point2& p)
{
	a -= p.a;
	b -= p.b;
	return *this;
}

Point2& Math::Point2::operator*=(int s)
{
	a *= s;
	b *= s;
	return *this;
}

Point2& Math::Point2::operator/=(int s)
{
	a /= s;
	b /= s;
	return *this;
}

const Math::Point2 Math::operator+(const Math::Point2& p1, const Math::Point2& p2)
{
	Point2 temp(p1);
	return temp += p2;
}

const Math::Point2 Math::operator-(const Math::Point2& p1, const Math::Point2& p2)
{
	Point2 temp(p1);
	return temp -= p2;
}

const Math::Point2 Math::operator*(const Math::Point2& p, float s)
{
	Point2 temp(p);
	return temp *= s;
}

const Math::Point2 Math::operator*(float s, const Math::Point2& p)
{
	return p * s;
}

const Math::Point2 Math::operator/(const Math::Point2& p, float s)
{
	Point2 temp(p);
	return temp /= s;
}

bool Math::MatchPoint2(const Math::Point2& p1, const Math::Point2& p2)
{
	if (p1.a != p2.a)return false;
	if (p1.b != p2.b)return false;

	return true;
}
