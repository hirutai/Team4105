#pragma once
#include <vector>

namespace Math
{

	struct Vector2
	{
		float x;	// x����
		float y;	// y����

		// �R���X�g���N�^
		Vector2();						// ��x�N�g���Ƃ���
		Vector2(float x, float y);		// x����,y�������w�肵�Ă̐���

		// �����o�֐�
		float length() const; // �m����(����)�����߂�
		Vector2& normalize(); // ���K������
		float dot(const Vector2& v)const;		// ���ς����߂�
		float cross(const Vector2& v)const;
		double distance(const Vector2& a)const;// ����

		// �P�����Z�q�I�[�o�[���[�h
		Vector2 operator+() const;
		Vector2 operator-() const;

		// ������Z�q�I�[�o�[���[�h
		Vector2& operator+=(const Vector2& v);
		Vector2& operator-=(const Vector2& v);
		Vector2& operator*=(float s);
		Vector2& operator/=(float s);
	};

	struct Vector3
	{
		float x, y, z;

		// �R���X�g���N�^
		Vector3();						// ��x�N�g���Ƃ���
		Vector3(float x, float y, float z);		// x����,y�������w�肵�Ă̐���
		Vector3(Vector2 v, float z);

		// �����o�֐�
		Vector2 xy()const;
		float length() const; // �m����(����)�����߂�
		Vector3& normalize(); // ���K������
		float dot(const Vector3& v)const;		// ���ς����߂�
		Vector3 cross(const Vector3& v)const;	// �O�ς����߂�

		// �P�����Z�q�I�[�o�[���[�h
		Vector3 operator+() const;
		Vector3 operator-() const;

		// ������Z�q�I�[�o�[���[�h
		Vector3& operator+=(const Vector3& v);
		Vector3& operator-=(const Vector3& v);
		Vector3& operator*=(float s);
		Vector3& operator/=(float s);
	};

	struct Vector4
	{
		float x, y, z,w;
		// �R���X�g���N�^
		Vector4();						// ��x�N�g���Ƃ���
		Vector4(float x, float y, float z,float w = 0.0f);		// x����,y�������w�肵�Ă̐���
		Vector4(Vector2 v, float z,float w);
		Vector4(Vector2 v1, Vector2 v2);
		Vector4(Vector3 v, float w);
		// �����o�֐�
		Vector2 xy()const;
		Vector3 xyz()const;
	};

	struct VECTOR
	{
		float r[4];

		VECTOR();
		VECTOR(float r, float g, float b);
		VECTOR(float r, float g, float b,float a);
	};

	struct VMatrix3
	{
		float r[3];
		VMatrix3();
		void Set(float x, float y, float z);
		void Set(Vector3 v);
	};
	struct Matrix3
	{
		float r[3][3];
		Matrix3();	// ��s��
		Matrix3& Identity();	// �P�ʍs��
		// �t�s��
		Matrix3 Inverse();
		void Set(int array_num, float a1, float a2, float a3);
	};
	struct Matrix4
	{
		VECTOR r[4];

		Matrix4();	// ��s��

		Matrix4& Identity();	// �P�ʍs��
	};
	struct MatNxN
	{
		std::vector<std::vector<float>> r;
		MatNxN(int n = 2);
		MatNxN Identity();
		MatNxN Inverse();
		MatNxN Cofactor(int ni, int nj);
		float Deploy();
		MatNxN Inverte();
		MatNxN Adjugate();
	};

	struct IMG_DATA
	{
		Vector2 position;
		float rotation;
		Vector2 scale;
		Vector4 color;
	};

	// 255 : 1 = 130 : x => x = 130/255
	float Transform(float bnMax, float aMax,float bNow);

	float ConvertToRadians(float rad);

	Vector4 AddVec4(const Vector4& v0, const Vector4& v1);
	
	Vector4 SubVec4(const Vector4& v0, const Vector4& v1);

	Vector4 MulVec4(const Vector4& v, float s);

	float VecLength4Square(const Vector4& v);

	float VecLength4(const Vector4& v);

	Vector4 VecNormalize4(const Vector4& v);

	float DotVec4(const Vector4& v0, const Vector4& v1);

	Vector4 CrossVec4(const Vector4& v0, const Vector4& v1);

	Vector4 CrossVec4(const Vector4& p0, const Vector4& p1, const Vector4& p2);

	Vector4 VectorLerp(const Vector4& v0, const Vector4& v1, float t);

	bool CompareVector2(const Vector2& v1, const Vector2& v2);
}

// 2�����Z�q�I�[�o�[���[�h
	// �����낢��Ȉ����̃p�^�[���ɑΉ�(�����̏���)���邽�߂ɁA�ȉ��̂悤�ɏ���
const Math::Vector2 operator+(const Math::Vector2& v1, const Math::Vector2& v2);
const Math::Vector2 operator-(const Math::Vector2& v1, const Math::Vector2& v2);
const Math::Vector2 operator*(const Math::Vector2& v, float s);
const Math::Vector2 operator*(float s, const Math::Vector2& v);
const Math::Vector2 operator/(const Math::Vector2& v, float s);

const Math::Vector3 operator+(const Math::Vector3& v1, const Math::Vector3& v2);
const Math::Vector3 operator-(const Math::Vector3& v1, const Math::Vector3& v2);
const Math::Vector3 operator*(const Math::Vector3& v, float s);
const Math::Vector3 operator*(float s, const Math::Vector3& v);
const Math::Vector3 operator/(const Math::Vector3& v, float s);
const Math::Vector3 lerp(const Math::Vector3& start, const Math::Vector3& end, const float t);
const Math::Vector3 easeIn(const Math::Vector3& start, const Math::Vector3& end, const float t);
const Math::Vector3 easeOut(const Math::Vector3& start, const Math::Vector3& end, const float t);
const Math::Vector3 easeInOut(const Math::Vector3& start, const Math::Vector3& end, const float t);

const Math::Matrix4 operator* (const Math::Matrix4& m1, const Math::Matrix4& m2);
const Math::VECTOR operator* (const Math::VECTOR& v, const Math::Matrix4& m);

Math::Matrix4 OrthographicOffCenterLH(float vLeft, float vRight, float vBottom, float vTop, float NearZ, float FarZ);
Math::Matrix4 Scaling(float x, float y, float z);
Math::Matrix4 RotX(float rot);
Math::Matrix4 RotY(float rot);
Math::Matrix4 RotZ(float rot);
Math::Matrix4 Translation(float x, float y, float z);

const Math::Matrix3 operator*(const Math::Matrix3& mat1, const Math::Matrix3& mat2);
const Math::VMatrix3 operator*(const Math::Matrix3& mat, const Math::VMatrix3& vmat);

const Math::MatNxN operator*(const Math::MatNxN& m1, const Math::MatNxN& m2);
const Math::MatNxN operator*(const Math::MatNxN& mat, float n);
const Math::MatNxN operator*(float n, const Math::MatNxN& mat);