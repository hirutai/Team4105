#include "Spline.h"

Spline::Spline()
{}

Spline::~Spline()
{
	/*for (auto x : spheres)
	{
		delete x;
	}
	spheres.clear();*/
}

void Spline::AddPoint(Math::Vector3 point)
{
	points.push_back(point);
}

Math::Vector3 Spline::splinePosition(const std::vector<Math::Vector3>& points,
	int startIndex,
	float timeRate)
{
	// •âŠÔ‚·‚×‚«“_‚Ì”
	int n = points.size() - 2;

	if (startIndex > n)return points[n];	// Pn‚Ì’l‚ð•Ô‚·
	if (startIndex < 1)return points[1];	// P1‚Ì’l‚ð•Ô‚·

	// p0`p3 ‚Ì§Œä“_‚ðŽæ“¾‚·‚é ¦ p1`p2 ‚ð•âŠÔ‚·‚é.
	Math::Vector3 P0 = points[startIndex - 1];
	Math::Vector3 P1 = points[startIndex];
	Math::Vector3 P2 = points[startIndex + 1];
	Math::Vector3 P3 = points[startIndex + 2];

	// Catmull-Rom ‚ÌŽ®‚É‚æ‚é•âŠÔ (p0,p1,p2,p3‚ÌŽ®)
	Math::Vector3 position = 0.5f * (P1 * 2 + (-P0 + P2) * timeRate
		+ ((P0 * 2) - (P1 * 5) + (P2 * 4) - P3) * (timeRate * timeRate)
		+ (-P0 + (P1 * 3) - (P2 * 3) + P3) * (timeRate * timeRate * timeRate));

	return position;
}

void Spline::SetPos(Math::Vector3 pos)
{
	position = pos;
}

Math::Vector3 Spline::GetPos()
{
	return position;
}

Math::Vector3 Spline::GetfPos()
{
	return futurePos;
}

int Spline::GetPointSize()
{
	return (int)points.size();
}

void Spline::Clear()
{
	/*for (auto x : spheres)
	{
		delete x;
	}
	spheres.clear();*/
	points.clear();
}

void Spline::Update(int startIndex, float timeRate)
{
	position = Spline::splinePosition(points, startIndex, timeRate);
	futurePos = Spline::splinePosition(points, startIndex, timeRate + 0.01f);

	int cPoint = 0;
	/*for (auto x : spheres)
	{
		x->position = points[cPoint];
		x->scale = Math::Vector3(1, 1, 1) * 0.5f;
		x->RGB = Math::Vector3(2, 2, 1);
		
		x->Update();
		cPoint++;
	}*/
}

void Spline::Draw()
{
	//DrawSphere3D(position, 5.0f, 32, color, GetColor(255, 255, 255), TRUE);
	/*if (spheres.size() != 0)
	{
		for (auto x : spheres)
		{
			x->Draw();
		}
	}*/
}