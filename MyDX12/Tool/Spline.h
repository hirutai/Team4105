#pragma once
#include <iostream>
#include <vector>
#include "../Struct/Math.h"

class Spline
{
private: // �G�C���A�X
	using Vector3 = Math::Vector3;
private:
	Vector3 p0;		// ����_ 1
	Vector3 p1;		// ����_ 2
	Vector3 p2;		// ����_ 3
	Vector3 p3;		// ����_ 4

	//					P1 - P2 - P3 - P4 ��ʂ�X�v���C���Ȑ����l����
	//						�擪(P0)�ƍŌ�(P5)�ɐ���_��ǉ����Ă���
	//								P0	P1	P2	P3	P4	P5
	std::vector<Vector3> points;
	int color;

	Vector3 position;
	Vector3 futurePos;

public:
	Spline();
	~Spline();

	void AddPoint(Vector3 point);

	// ����_�̏W��(vector�R���e�i), ��Ԃ����Ԃ̓Y���A���Ԍo�ߗ�
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