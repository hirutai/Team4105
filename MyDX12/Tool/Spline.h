#pragma once
#include <iostream>
#include <vector>
#include "../Struct/Math.h"

class Spline
{
private: // エイリアス
	using Vector3 = Math::Vector3;
private:
	Vector3 p0;		// 制御点 1
	Vector3 p1;		// 制御点 2
	Vector3 p2;		// 制御点 3
	Vector3 p3;		// 制御点 4

	//					P1 - P2 - P3 - P4 を通るスプライン曲線を考える
	//						先頭(P0)と最後(P5)に制御点を追加している
	//								P0	P1	P2	P3	P4	P5
	std::vector<Vector3> points;
	int color;

	Vector3 position;
	Vector3 futurePos;

public:
	Spline();
	~Spline();

	void AddPoint(Vector3 point);

	// 制御点の集合(vectorコンテナ), 補間する区間の添字、時間経過率
	static Vector3 splinePosition(const std::vector<Vector3>& points,
		int startIndex,
		float timeRate);

	void SetPos(Vector3 pos);
	Vector3 GetPos();
	Vector3 GetfPos();
	int GetPointSize();
	void Clear();
	void Update(int startIndex, float timeRate);
	void Draw();
};