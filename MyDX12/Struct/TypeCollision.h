#pragma once
#include "Math.h"

namespace Math
{
	// ����
	struct Segment
	{
		Vector4 pos;// �n�_
		Vector4 vec;// �x�N�g��
	};

	// ��
	struct Sphere
	{
		Vector4 pos; // ���S���W
		float r; // ���a
	};

	// ����
	struct Plane
	{
		float a, b, c, d; // ���ʂ̗v�f
		float distance;
	};

	// �O�p�`
	struct Triangle
	{
		Vector4 pos[3]; // �e���_
		Plane plane; // �O�p�`���܂ޕ���
	};

	// ����(2����)
	struct Segment2
	{
		Vector2 pos; // �n�_
		Vector2 vec; // �x�N�g��
	};

	Vector4 SetVector4(float x, float y, float z);

	Vector4 SetPoint4(float x, float y, float z);

	Segment Segment_Set(float px, float py, float pz, float vx, float vy, float vz);

	Sphere Sphere_Set(float x, float y, float z, float r);

	Plane Plane_Set(const Vector4& normal, const Vector4& p);

	Plane Plane_Set(const Vector4& p0, const Vector4& p1, const Vector4& p2);

	Triangle Triangle_Set(const Vector4& p0, const Vector4& p1, const Vector4& p2);

	float Plane_Length(const Plane& pln, const Vector4& pos);

	int Plane_Side(const Plane& plane, const Vector4& pos);

	// ����(2����)�̏Փ˔���
	int HitCheck_Segment2_Segment2(Vector2* hit_pos, const Segment2& s0, const Segment2& s1);

	// �����~���̏Փ˔���
	int HitCheck_Sphere_Segment( const Sphere& sph, const Segment& seg,float* distance = nullptr ,Vector4* hit_pos = nullptr );

	// �����~���ʂ̏Փ˔���
	int HitCheck_Plane_Segment(const Plane& pln, const Segment& seg, float* distance = nullptr, Vector4* hit_pos = nullptr);

	// �O�p�`�̔C�ӂ̓_���O�p�`���ɑ��݂��邩�H
	int Triangle_CheckInner(const Triangle& tri, const Vector4& p);

	// �O�p�`�Ɛ����̏Փ˔���
	int HitCheck_Triangle_Segment(const Triangle& tri, const Segment& seg, float* distance = nullptr, Vector4* hit_pos = nullptr);

	// �����m�̏Փ˔���
	int HitCheck_Sphere_Sphere(const Sphere& sph0, const Sphere& sph1, Vector4* hit_vec = nullptr,Vector4* reject = nullptr);

	// ���ʂƋ��̏Փ˔���
	int HitCheck_Plane_Sphere(Vector4* hit_vec, const Plane& pln, const Sphere& sph);

	void ClosestPtPoint_Triangle(const Vector4& point, const Triangle& triangle, Vector4* closest);

	// �O�p�`�Ƌ��̏Փ˔���
	int HitCheck_Triangle_Sphere(const Triangle& tri, const Sphere& sph, Vector4* hit_vec = nullptr,Vector4* reject = nullptr);

	// ���ʓ��m�̏Փ˔���
	int HitCheck_Plane_Plane(Segment* cross_seg, const Plane& pln0, const Plane& pln1);

	// ���ʂƎO�p�`�̏Փ˔���
	int HitCheck_Plane_Triangle(const Plane& pln, const Triangle& tri);

	// �O�p�`���m�̏Փ˔���
	int HitCheck_Triangle_Triangle(const Triangle& tri0, const Triangle& tri1);

	// �o�E���f�B���O�X�t�B�A
	struct BoundingSphere
	{
		Vector4 pos;	// ���S���W
		float r;		// ���a
	};

	// �����m�̃o�E���f�B���O����
	int BoundingCheck_BoundingSphere_BoundingSphere(const BoundingSphere& bs0, const BoundingSphere& bs1);

	// �o�E���f�B���O�{�b�N�X(AABB)
	struct AABB
	{
		Vector4 min;	// ���W�ŏ��l
		Vector4 max;	// ���W�ő�l
	};

	// AABB�Ɛ����̃o�E���f�B���O�{�b�N�X����
	int BoundingCheck_AABB_Segment(const AABB& aabb, const Segment& seg);

	// AABB���m
	int BoundingCheck_AABB_AABB(const AABB& aabb1, const AABB& aabb2);

	// �s��̍\����
	struct Matrix4x4
	{
		float m00, m01, m02, m03;
		float m10, m11, m12, m13;
		float m20, m21, m22, m23;
		float m30, m31, m32, m33;
		// �z��p
		float m[4][4];
	};

	// �s��̐ݒ�(�z��ɃR�s�[)
	Matrix4x4 Matrix4x4_Array(const Matrix4x4& mat);

	// �P�ʍs��
	Matrix4x4 _Identity();

	// �X�P�[�����O�s��l��
	Matrix4x4 _Scale(float sx, float sy, float sz);

	// X����]�s��l��
	Matrix4x4 _RotationX(float rx);

	// Y����]�s��l��
	Matrix4x4 _RotationY(float ry);

	// Z����]�s��l��
	Matrix4x4 _RotationZ(float rz);

	// ���s�ړ��s��
	Matrix4x4 _Translate(float tx, float ty, float tz);

	// �s��̏�Z(�s��~�x�N�g��)
	Vector4 Matrix4x4_x_Vector4(const Matrix4x4& m, const Vector4& v);

	// �s�񓯎m�̏�Z
	Matrix4x4 _MulMatrix4x4(const Matrix4x4& m0, const Matrix4x4& m1);

	// �t�s��
	Matrix4x4 _InverseMatrix(const Matrix4x4& m);

	Matrix4x4 _InverseMatrixFast(const Matrix4x4& m);

	// �]�u�s��
	Matrix4x4 _Transpose(const Matrix4x4& m);

	// 3x4�̍s��
	struct Matrix3x4
	{
		float m00, m01, m02, m03;
		float m10, m11, m12, m13;
		float m20, m21, m22, m23;
	};

	// �s�񓯎m�̏�Z
	Matrix3x4 _MulMatrix3x4(const Matrix3x4& m0, const Matrix3x4& m1);

	// �o�E���f�B���O�{�b�N�X(OBB)
	struct OBB
	{
		Matrix4x4 m[2];		// �{�b�N�X�p�s��
	};

	// OBB ��ݒ�
	OBB OBB_Set(const Vector4& pos, const Vector4& rot, const Vector4& scale);

	// OBB�Ɛ����̃o�E���f�B���O����
	int BoundingCheck_OBB_Segment(const OBB& obb, const Segment seg);

	// �J�v�Z��
	struct Capsule
	{
		Vector4 start_pos;
		Vector4 end_pos;
		float radius;
	};

	// �J�v�Z���̐ݒ�
	Capsule Capsule_Set(const Vector4& start, const Vector4& end, float r);
	// 11/15(��)���o�Z�ɂȂ�\����(������803)
	// 11/19(��)�͕s��

	int HitCheck_Capsule_Sphere(const Capsule& caps, const Sphere& sph);

	// �l��͈͓��Ɏ��߂� [low,high](�␳�l���Ԃ��Ă���)
	float clamp(float x, float low, float high);

	// �����Ɛ����̋�����2������߂�
	float SegmentDistanceSquare(const Vector4& p1, const Vector4& q1, const Vector4& p2, const Vector4& q2);

	// �J�v�Z�����m
	int HitCheck_Capsule_Capsule(const Capsule& cap0, const Capsule& cap1);

	// �{�b�N�X�Ƌ��̓����蔻��
	int HitCheck_AABB_Sphere(const AABB& aabb, const Sphere& sph);

	// �J�v�Z���ƃ{�b�N�X�̓����蔻��
	int HitCheck_AABB_Capsule(const AABB& aabb, const Capsule& caps);

	struct Quaternion
	{
		float x, y, z, w;
	};

	// �N�H�[�^�j�I���̐ݒ�
	Quaternion Quaternion_Set(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f);
	Quaternion Quaternion_Set(const Vector4& v, float angle);

	// ����
	float Dot_Quaternion(const Quaternion& q1, const Quaternion& q2);

	// ����(�m����)�����߂�
	float Length_Quaternion(const Quaternion& q);

	// ���K��
	Quaternion Normalize_Quaternion(const Quaternion& q);

	// �P�����Z�q�I�[�o�[���[�h
	Quaternion operator + (const Quaternion& q);
	Quaternion operator - (const Quaternion& q);

	// ������Z�q�I�[�o�[���[�h
	Quaternion& operator += (Quaternion& q1, const Quaternion& q2);
	Quaternion& operator -= (Quaternion& q1, const Quaternion& q2);
	Quaternion& operator *= (Quaternion& q, float s);
	Quaternion& operator /= (Quaternion& q, float s);
	Quaternion& operator *= (Quaternion& q1, const Quaternion& q2);

	// �񍀉��Z�q�I�[�o�[���[�h
	Quaternion operator + (const Quaternion& q1, const Quaternion& q2);
	Quaternion operator - (const Quaternion& q1, const Quaternion& q2);
	Quaternion operator * (const Quaternion& q1, const Quaternion& q2);
	Quaternion operator * (const Quaternion& q, float s);
	Quaternion operator * (float s, const Quaternion& q);
	Quaternion operator / (const Quaternion& q, float s);

	Quaternion slerp(const Quaternion& q1, const Quaternion& q2, float t);
	Quaternion lerp(const Quaternion& q1, const Quaternion& q2, float t);

	Matrix4x4 rotate(const Quaternion& q);

	Quaternion Quaternion_Set(const Matrix4x4& m);
}