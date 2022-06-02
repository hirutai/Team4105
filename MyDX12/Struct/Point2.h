#pragma once

namespace Math {

	struct Point2 {
		int a, b;

		Point2();
		Point2(int _a, int _b);

		float length() const; // �m����(����)�����߂�
		Point2& normalize(); // ���K������

		// �P�����Z�q�I�[�o�[���[�h
		Point2 operator+()const;
		Point2 operator-()const;

		// ������Z�q�I�[�o�[���[�h
		Point2& operator+=(const Point2& p);
		Point2& operator-=(const Point2& p);
		Point2& operator*=(int s);
		Point2& operator/=(int s);

	};

	const Math::Point2 operator+(const Math::Point2& p1, const Math::Point2& p2);
	const Math::Point2 operator-(const Math::Point2& p1, const Math::Point2& p2);
	const Math::Point2 operator*(const Math::Point2& p, float s);
	const Math::Point2 operator*(float s, const Math::Point2& p);
	const Math::Point2 operator/(const Math::Point2& p, float s);

	bool MatchPoint2(const Math::Point2& p1, const Math::Point2& p2);

}