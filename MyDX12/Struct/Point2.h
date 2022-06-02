#pragma once

namespace Math {

	struct Point2 {
		int a, b;

		Point2();
		Point2(int _a, int _b);

		float length() const; // ノルム(長さ)を求める
		Point2& normalize(); // 正規化する

		// 単項演算子オーバーロード
		Point2 operator+()const;
		Point2 operator-()const;

		// 代入演算子オーバーロード
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