#include "Math.h"
#include <cmath>
#include <xmmintrin.h>
#define PI 3.14159265358979323846

using namespace Math;

Vector2::Vector2()
	:x(0.0f), y(0.0f)
{}

Vector2::Vector2(float x, float y)
	: x(x), y(y)
{}

Vector3::Vector3() : Vector3(0, 0, 0)
{}

Vector3::Vector3(float x, float y, float z) : x(x),y(y), z(z)
{}

Vector3::Vector3(Vector2 v, float z) : x(v.x), y(v.y), z(z)
{}

float Vector2::length() const
{
	return sqrtf(powf(x, 2) + pow(y, 2));
}

Vector2& Vector2::normalize()
{
	float len = length();
	if (len != 0)
	{
		return *this /= len;
	}
	return *this;
}

float Vector2::dot(const Vector2& v)const
{
	return (x * v.x) + (y * v.y);
}

float Math::Vector2::cross(const Vector2& v) const
{
	return (x * v.y) - (y * v.x);
}

double Math::Vector2::distance(const Vector2& a) const
{
	return sqrt((a.x - x) * (a.x - x) + (a.y - y) * (a.y - y));
}

Vector2 Vector2::operator+() const
{
	return *this;
}

Vector2 Vector2::operator-() const
{
	return Vector2(-x, -y);
}

Vector2& Vector2::operator+=(const Vector2& v)
{
	x += v.x;
	y += v.y;

	return *this;
}

Vector2& Vector2::operator-=(const Vector2& v)
{
	x -= v.x;
	y -= v.y;

	return *this;
}

Vector2& Vector2::operator*=(float s)
{
	x *= s;
	y *= s;
	return *this;
}

Vector2& Vector2::operator/=(float s)
{
	x /= s;
	y /= s;
	return *this;
}

Vector2 Math::Vector3::xy()const
{
	return Vector2(x, y);
}

float Vector3::length() const
{
	return sqrtf(powf(x, 2) + powf(y, 2) + powf(z, 2));
}

Vector3& Vector3::normalize()
{
	float len = length();
	if (len != 0)
	{
		return *this /= len;
	}
	return *this;
}

float Vector3::dot(const Vector3& v)const
{
	return (x * v.x) + (y * v.y) + (z * v.z);
}

Vector3 Vector3::cross(const Vector3& v)const
{
	Vector3 temp(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	return temp;
}


Vector3 Vector3::operator+() const
{
	return *this;
}

Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

Vector3& Vector3::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;

	return *this;
}

Vector3& Vector3::operator-=(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;

	return *this;
}

Vector3& Vector3::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Vector3& Vector3::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

const Vector2 operator+(const Vector2& v1, const Vector2& v2)
{
	Vector2 temp(v1);
	return temp += v2;
}

const Vector2 operator-(const Vector2& v1, const Vector2& v2)
{
	Vector2 temp(v1);
	return temp -= v2;
}

const Vector2 operator*(const Vector2& v, float s)
{
	Vector2 temp(v);
	return temp *= s;
}

const Vector2 operator*(float s, const Vector2& v)
{
	return v * s;
}

const Vector2 operator/(const Vector2& v, float s)
{
	Vector2 temp(v);
	return temp /= s;
}

const Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp += v2;
}

const Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp -= v2;
}

const Vector3 operator*(const Vector3& v, float s)
{
	Vector3 temp(v);
	return temp *= s;
}

const Vector3 operator*(float s, const Vector3& v)
{
	return v * s;
}

const Vector3 operator/(const Vector3& v, float s)
{
	Vector3 temp(v);
	return temp /= s;
}

const Vector3 lerp(const Vector3& start, const Vector3& end, const float t)
{
	return start * (1.0f - t) + end * t;
}

const Vector3 easeIn(const Vector3& start, const Vector3& end, const float t)
{
	float w = 2.0f;
	return start * (1.0f - powf(t, w)) + end * powf(t, w);
}

const Vector3 easeOut(const Vector3& start, const Vector3& end, const float t)
{
	float n = t * (2.0f - t);
	return start * (1.0f - n) + end * n;
}

const Vector3 easeInOut(const Vector3& start, const Vector3& end, const float t)
{
	float w = 2.0f;
	float n = powf(t, w) * ((w + 1.0f) - (w * t));
	return start * (1.0f - n) + end * n;
}

Math::Vector4::Vector4() : Vector4(0,0,0,0)
{}

Math::Vector4::Vector4(float x, float y, float z, float w)
	: x(x), y(y), z(z),w(w)
{}

Math::Vector4::Vector4(Vector2 v, float z, float w)
	: Vector4(v.x,v.y,z,w)
{}

Math::Vector4::Vector4(Vector2 v1, Vector2 v2)
	: Vector4(v1.x, v1.y, v2.x, v2.y)
{}

Math::Vector4::Vector4(Vector3 v, float w)
	: x(v.x), y(v.y), z(v.z),w(w)
{}

Vector2 Math::Vector4::xy() const
{
	return Vector2(x,y);
}

Vector3 Math::Vector4::xyz() const
{
	return Vector3(x,y,z);
}

Matrix4::Matrix4()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i == j)
			{
				r[i].r[j] = 1;
			}
			else
			{
				r[i].r[j] = 0;
			}
		}
	}
}

Matrix4& Matrix4::Identity()
{
	*this = Matrix4();
	return *this;
}

VECTOR::VECTOR() :r{ 0,0,0,1 }
{}

VECTOR::VECTOR(float x, float y, float z) : r{ x,y,z,1 }
{}

Math::VECTOR::VECTOR(float x, float y, float z, float a)
	: r{x,y,z,a}
{
}

const Matrix4 operator*(const Matrix4& m1, const Matrix4& m2)
{
	Matrix4 result;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			float temp = 0.0f;

			for (int k = 0; k < 4; k++)
			{
				temp += m1.r[i].r[k] * m2.r[k].r[j];
			}

			result.r[i].r[j] = temp;
		}
	}

	return result;
}

const VECTOR operator*(const VECTOR& v, const Matrix4& m)
{
	VECTOR result;
	for (int i = 0; i < 4; i++)
	{
		float temp = 0.0f;
		for (int j = 0; j < 4; j++)
		{
			temp += v.r[j] * m.r[j].r[i];
		}

		result.r[i] = temp;
	}

	return result;
}

float Math::ConvertToRadians(float rad)
{
	return rad * (PI / 180.0f);
}

float Math::ConvertToAngle(float rad)
{
	return rad * (180.0f / PI);
}

float Math::Vector2ConversionToAngle(float x, float y)
{
	return ConvertToAngle(std::atan2(x, y));
}

float Math::Vector2ConversionToAngle(const Vector2& v)
{
	return Vector2ConversionToAngle(v.x,v.y);
}

Vector4 Math::AddVec4(const Vector4& v0, const Vector4& v1)
{
	Vector4 r;
	r.x = v0.x + v1.x;
	r.y = v0.y + v1.y;
	r.z = v0.z + v1.z;
	r.w = v0.w + v1.w;
	return r;
}

Vector4 Math::SubVec4(const Vector4& v0, const Vector4& v1)
{
	Vector4 r;
	r.x = v0.x - v1.x;
	r.y = v0.y - v1.y;
	r.z = v0.z - v1.z;
	r.w = v0.w - v1.w;
	return r;
}

Vector4 Math::MulVec4(const Vector4& v, float s)
{
	Vector4 r;
	r.x = v.x * s;
	r.y = v.y * s;
	r.z = v.z * s;
	r.w = v.w;
	return r;
}

float Math::VecLength4Square(const Vector4& v)
{
	float ls = v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
	return ls;
}

float Math::VecLength4(const Vector4& v)
{
	float l = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
	return l;
}

Vector4 Math::VecNormalize4(const Vector4& v)
{
	float l = VecLength4(v);
	Vector4 n;
	n.x = v.x / l;
	n.y = v.y / l;
	n.z = v.z / l;
	n.w = v.w / l;
	return n;
}

float Math::DotVec4(const Vector4& v0, const Vector4& v1)
{
	float r;
	r = (v0.x * v1.x) + (v0.y * v1.y) + (v0.z * v1.z) + (v0.w * v1.w);
	return r;
}

Vector4 Math::CrossVec4(const Vector4& v0, const Vector4& v1)
{
	Vector4 r;
	r.x = v0.y * v1.z - v0.z * v1.y;
	r.y = v0.z * v1.x - v0.x * v1.z;
	r.z = v0.x * v1.y - v0.y * v1.x;
	r.w = 0.0f;
	return r;
}

Vector4 Math::CrossVec4(const Vector4& p0, const Vector4& p1, const Vector4& p2)
{
	// ベクトルの作成
	Vector4 v0 = SubVec4(p1, p0);
	Vector4 v1 = SubVec4(p2, p1);
	// ベクトルから外積を算出
	return (CrossVec4(v0, v1));
}

Vector4 Math::VectorLerp(const Vector4& v0, const Vector4& v1, float t)
{
	Vector4 result;
	result.x = v0.x + t * (v1.x - v0.x);
	result.y = v0.y + t * (v1.y - v0.y);
	result.z = v0.z + t * (v1.z - v0.z);
	result.w = v0.w + t * (v1.w - v0.w);
	return result;
}

bool Math::CompareVector2(const Vector2& v1, const Vector2& v2)
{
	if (v1.x == v2.x && v1.y == v2.y)return 1;
	return 0;
}

Math::Matrix4 OrthographicOffCenterLH(float vLeft, float vRight, float vBottom, float vTop, float NearZ, float FarZ)
{
	Matrix4 M;
	float fReciprocalWidth = 1.0f / (vRight - vLeft);
	float fReciprocalHeight = 1.0f / (vTop - vBottom);
	float fRange = 1.0f / (FarZ - NearZ);
	VECTOR rMem =
	{ fReciprocalWidth,
		fReciprocalHeight,
		fRange,
		1.0f };

	VECTOR rMem2 =
	{
		-(vLeft + vRight),
		-(vTop + vBottom),
		-NearZ,
		1.0f
	};

	VECTOR vValues = rMem;
	// _mm_setzero_ps() : ゼロクリア
	VECTOR vTemp = {0,0,0,0};
	// _mm_move_ss(a,b) : vector = {b0,a1,a2,a3}
	vTemp = { vValues.r[0],vTemp.r[1],vTemp.r[2],vTemp.r[3] };
	// _mm_add_ss(a,b) : vector = {a0 + b0,a1,a2,a3}
	vTemp = { vTemp.r[0] + vTemp.r[0],vTemp.r[1],vTemp.r[2],vTemp.r[3] };
	M.r[0] = vTemp;
	vTemp = vValues;
	// _mm_add_ps(a,b) : vector = {a0 + b0,a1 + b1,a2 + b2,a3 + b3}
	vTemp =
	{
		vTemp.r[0] + 0,
		vTemp.r[1] + 0xFFFFFFFF,
		vTemp.r[2] + 0,
		vTemp.r[3] + 0
	};
	vTemp =
	{
		vTemp.r[0] + vTemp.r[0],
		vTemp.r[1] + vTemp.r[1],
		vTemp.r[2] + vTemp.r[2],
		vTemp.r[3] + vTemp.r[3]
	};
	M.r[1] = vTemp;
	vTemp = vValues;
	vTemp = 
	{
		vTemp.r[0] + 0,
		vTemp.r[1] + 0,
		vTemp.r[2] + 0xFFFFFFFF,
		vTemp.r[3] + 0
	};
	M.r[2] = vTemp;
	vValues = 
	{
		vValues.r[0] * rMem2.r[0],
		vValues.r[1] * rMem2.r[1],
		vValues.r[2] * rMem2.r[2],
		vValues.r[3] * rMem2.r[3]
	};
	M.r[3] = vValues;
	return M;
}

Matrix4 Scaling(float x, float y, float z)
{
	Matrix4 m;

	m.r[0].r[0] = x;
	m.r[1].r[1] = y;
	m.r[2].r[2] = z;
	return m;
}

Matrix4 RotX(float rot)
{
	Matrix4 m;
	m.r[1].r[1] = std::cosf(ConvertToRadians(rot));
	m.r[1].r[2] = std::sinf(ConvertToRadians(rot));
	m.r[2].r[1] = -std::sinf(ConvertToRadians(rot));
	m.r[2].r[2] = std::cosf(ConvertToRadians(rot));
	return m;
}

Matrix4 RotY(float rot)
{
	Matrix4 m;
	m.r[0].r[0] = std::cosf(ConvertToRadians(rot));
	m.r[0].r[2] = -std::sinf(ConvertToRadians(rot));
	m.r[2].r[0] = std::sinf(ConvertToRadians(rot));
	m.r[2].r[2] = std::cosf(ConvertToRadians(rot));
	return m;
}

Matrix4 RotZ(float rot)
{
	Matrix4 m;
	m.r[0].r[0] = std::cosf(ConvertToRadians(rot));
	m.r[0].r[1] = std::sinf(ConvertToRadians(rot));
	m.r[1].r[0] = -std::sinf(ConvertToRadians(rot));
	m.r[1].r[1] = std::cosf(ConvertToRadians(rot));
	return m;
}

Matrix4 Translation(float x, float y, float z)
{
	Matrix4 m;
	m.r[3].r[0] = x;
	m.r[3].r[1] = y;
	m.r[3].r[2] = z;
	return m;
}

const Math::Matrix3 operator*(const Math::Matrix3& mat1, const Math::Matrix3& mat2)
{
	Math::Matrix3 result;
	float term;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			term = 0.0f;
			for (int k = 0; k < 3; k++)
			{
				term = term + mat1.r[i][k] * mat2.r[k][j];
			}
			result.r[i][j] = term;
		}
	}

	return result;
}

const Math::VMatrix3 operator*(const Math::Matrix3& mat, const Math::VMatrix3& vmat)
{
	VMatrix3 result;
	float term;
	for (int i = 0; i < 3; i++)
	{
		term = 0.0f;
		for (int j = 0; j < 3; j++)
		{
			term = term + mat.r[i][j] * vmat.r[j];
		}
		result.r[i] = term;
	}
	return result;
}

float Math::Transform(float bnMax, float aMax, float bNow)
{
	float afbn = aMax * bNow;
	float ms = bnMax;
	return afbn /ms;
}

Math::Matrix3::Matrix3()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (i == j)
			{
				r[i][j] = 1;
			}
			else
			{
				r[i][j] = 0;
			}
		}
	}
}

Matrix3& Math::Matrix3::Identity()
{
	*this = Matrix3();
	return *this;
}

Matrix3 Math::Matrix3::Inverse()
{
	Matrix3 result;
	float invl = 1 /
		(
			r[0][0] * r[1][1] * r[2][2] +
			r[0][1] * r[1][2] * r[2][0] +
			r[0][2] * r[1][0] * r[2][1] -
			r[0][2] * r[1][1] * r[2][0] -
			r[0][1] * r[1][0] * r[2][2] -
			r[0][0] * r[1][2] * r[2][1]
		);
	float a00 =  r[1][1] * r[2][2] - r[1][2] * r[2][1];
	float a01 = -(r[0][1] * r[2][2] - r[0][2] * r[2][1]);
	float a02 =  (r[0][1] * r[1][2] - r[0][2] * r[1][1]);
	float a10 = -(r[1][0] * r[2][2] - r[1][2] * r[2][0]);
	float a11 =  (r[0][0] * r[2][2] - r[0][2] * r[2][0]);
	float a12 = -(r[0][0] * r[1][2] - r[0][2] * r[1][0]);
	float a20 =  (r[1][0] * r[2][1] - r[1][1] * r[2][0]);
	float a21 = -(r[0][0] * r[2][1] - r[0][1] * r[2][0]);
	float a22 =  (r[0][0] * r[1][1] - r[0][1] * r[1][0]);

	result.r[0][0] = a00 * invl;
	result.r[0][1] = a01 * invl;
	result.r[0][2] = a02 * invl;
	result.r[1][0] = a10 * invl;
	result.r[1][1] = a11 * invl;
	result.r[1][2] = a12 * invl;
	result.r[2][0] = a20 * invl;
	result.r[2][1] = a21 * invl;
	result.r[2][2] = a22 * invl;

	return result;
}

void Math::Matrix3::Set(int array_num, float a1, float a2, float a3)
{
	r[array_num][0] = a1;
	r[array_num][1] = a2;
	r[array_num][2] = a3;
}

Math::VMatrix3::VMatrix3()
{
	r[0] = 0;
	r[1] = 0;
	r[2] = 0;
}

void Math::VMatrix3::Set(float x, float y, float z)
{
	r[0] = x;
	r[1] = y;
	r[2] = z;
}

void Math::VMatrix3::Set(Vector3 v)
{
	Set(v.x, v.y, v.z);
}

Math::MatNxN::MatNxN(int n)
{
	r.resize(n, std::vector<float>(n));
	for (int i = 0; i < r.size(); i++)
	{
		for (int j = 0; j < r[0].size(); j++)
		{
			r[i][j] = 0;
		}
	}
}

MatNxN Math::MatNxN::Identity()
{
	for (int i = 0; i < r.size(); i++)
	{
		for (int j = 0; j < r[0].size(); j++)
		{
			if (i == j)
			{
				r[i][j] = 1;
			}
			else
			{
				r[i][j] = 0;
			}
		}
	}
	return *this;
}

MatNxN Math::MatNxN::Inverse()
{
	MatNxN result(r.size());
	MatNxN sub = *this;
	float denom = 1 / sub.Deploy();

	result = sub.Adjugate() * denom;

	return result;
}

MatNxN Math::MatNxN::Cofactor(int ni, int nj)
{
	MatNxN ubM(r.size());
	int ubI = 0;
	int ubJ = 0;
	for (int i = 0; i < r.size(); i++)
	{
		if (i == ni)continue;

		ubJ = 0;
		for (int j = 0; j < r[0].size(); j++)
		{
			if (j == nj)continue;

			ubM.r[ubI][ubJ] = r[i][j];
			ubJ++;
		}
		ubI++;
	}
	MatNxN result = ubM * pow(-1.0f, (ni + 1) + (nj + 1));

	return result;
}

float Math::MatNxN::Deploy()
{
	float dep = 0.0f;

	for (int i = 0; i < r.size(); i++)
	{
		for (int j = 0; j < r[0].size(); j++)
		{
			MatNxN cof = this->Cofactor(i, j);

			dep += r[i][j] * cof.r[i][j];
		}
	}

	return dep;
}

MatNxN Math::MatNxN::Inverte()
{
	MatNxN result(r.size());
	for (int i = 0; i < r.size(); i++)
	{
		for (int j = 0; j < r[0].size(); j++)
		{
			result.r[j][i] = r[i][j];
		}
	}

	return result;
}

MatNxN Math::MatNxN::Adjugate()
{
	MatNxN result(r.size());
	for (int i = 0; i < r.size(); i++)
	{
		for (int j = 0; j < r[0].size(); j++)
		{
			result.r[i][j] = this->Deploy();
		}
	}

	return result;
}

const Math::MatNxN operator*(const Math::MatNxN& m1, const Math::MatNxN& m2)
{
	MatNxN result(m1.r.size());
	float term;
	for (int i = 0; i < m1.r.size(); i++)
	{
		for (int j = 0; j < m1.r[0].size(); j++)
		{
			term = 0.0f;
			for (int k = 0; k < m1.r.size(); k++)
			{
				term = term + m1.r[i][k] * m2.r[k][j];
			}
			result.r[i][j] = term;
		}
	}

	return result;
}

const Math::MatNxN operator*(const Math::MatNxN& mat, float n)
{
	MatNxN result = mat;
	for (int i = 0; i < result.r.size(); i++)
	{
		for (int j = 0; j < result.r[0].size(); j++)
		{
			result.r[i][j] *= n;
		}
	}
	return result;
}

const Math::MatNxN operator*(float n, const Math::MatNxN& mat)
{
	MatNxN result = mat;
	return result * n;
}