#pragma once
#include "Math.h"

namespace Math
{
	// 線分
	struct Segment
	{
		Vector4 pos;// 始点
		Vector4 vec;// ベクトル
	};

	// 球
	struct Sphere
	{
		Vector4 pos; // 中心座標
		float r; // 半径
	};

	// 平面
	struct Plane
	{
		float a, b, c, d; // 平面の要素
		float distance;
	};

	// 三角形
	struct Triangle
	{
		Vector4 pos[3]; // 各頂点
		Plane plane; // 三角形を含む平面
	};

	// 線分(2次元)
	struct Segment2
	{
		Vector2 pos; // 始点
		Vector2 vec; // ベクトル
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

	// 線分(2次元)の衝突判定
	int HitCheck_Segment2_Segment2(Vector2* hit_pos, const Segment2& s0, const Segment2& s1);

	// 線分×球の衝突判定
	int HitCheck_Sphere_Segment( const Sphere& sph, const Segment& seg,float* distance = nullptr ,Vector4* hit_pos = nullptr );

	// 線分×平面の衝突判定
	int HitCheck_Plane_Segment(const Plane& pln, const Segment& seg, float* distance = nullptr, Vector4* hit_pos = nullptr);

	// 三角形の任意の点が三角形内に存在するか？
	int Triangle_CheckInner(const Triangle& tri, const Vector4& p);

	// 三角形と線分の衝突判定
	int HitCheck_Triangle_Segment(const Triangle& tri, const Segment& seg, float* distance = nullptr, Vector4* hit_pos = nullptr);

	// 球同士の衝突判定
	int HitCheck_Sphere_Sphere(const Sphere& sph0, const Sphere& sph1, Vector4* hit_vec = nullptr,Vector4* reject = nullptr);

	// 平面と球の衝突判定
	int HitCheck_Plane_Sphere(Vector4* hit_vec, const Plane& pln, const Sphere& sph);

	void ClosestPtPoint_Triangle(const Vector4& point, const Triangle& triangle, Vector4* closest);

	// 三角形と球の衝突判定
	int HitCheck_Triangle_Sphere(const Triangle& tri, const Sphere& sph, Vector4* hit_vec = nullptr,Vector4* reject = nullptr);

	// 平面同士の衝突判定
	int HitCheck_Plane_Plane(Segment* cross_seg, const Plane& pln0, const Plane& pln1);

	// 平面と三角形の衝突判定
	int HitCheck_Plane_Triangle(const Plane& pln, const Triangle& tri);

	// 三角形同士の衝突判定
	int HitCheck_Triangle_Triangle(const Triangle& tri0, const Triangle& tri1);

	// バウンディングスフィア
	struct BoundingSphere
	{
		Vector4 pos;	// 中心座標
		float r;		// 半径
	};

	// 球同士のバウンディング判定
	int BoundingCheck_BoundingSphere_BoundingSphere(const BoundingSphere& bs0, const BoundingSphere& bs1);

	// バウンディングボックス(AABB)
	struct AABB
	{
		Vector4 min;	// 座標最小値
		Vector4 max;	// 座標最大値
	};

	// AABBと線分のバウンディングボックス判定
	int BoundingCheck_AABB_Segment(const AABB& aabb, const Segment& seg);

	// AABB同士
	int BoundingCheck_AABB_AABB(const AABB& aabb1, const AABB& aabb2);

	// 行列の構造体
	struct Matrix4x4
	{
		float m00, m01, m02, m03;
		float m10, m11, m12, m13;
		float m20, m21, m22, m23;
		float m30, m31, m32, m33;
		// 配列用
		float m[4][4];
	};

	// 行列の設定(配列にコピー)
	Matrix4x4 Matrix4x4_Array(const Matrix4x4& mat);

	// 単位行列
	Matrix4x4 _Identity();

	// スケーリング行列獲得
	Matrix4x4 _Scale(float sx, float sy, float sz);

	// X軸回転行列獲得
	Matrix4x4 _RotationX(float rx);

	// Y軸回転行列獲得
	Matrix4x4 _RotationY(float ry);

	// Z軸回転行列獲得
	Matrix4x4 _RotationZ(float rz);

	// 平行移動行列
	Matrix4x4 _Translate(float tx, float ty, float tz);

	// 行列の乗算(行列×ベクトル)
	Vector4 Matrix4x4_x_Vector4(const Matrix4x4& m, const Vector4& v);

	// 行列同士の乗算
	Matrix4x4 _MulMatrix4x4(const Matrix4x4& m0, const Matrix4x4& m1);

	// 逆行列化
	Matrix4x4 _InverseMatrix(const Matrix4x4& m);

	Matrix4x4 _InverseMatrixFast(const Matrix4x4& m);

	// 転置行列化
	Matrix4x4 _Transpose(const Matrix4x4& m);

	// 3x4の行列
	struct Matrix3x4
	{
		float m00, m01, m02, m03;
		float m10, m11, m12, m13;
		float m20, m21, m22, m23;
	};

	// 行列同士の乗算
	Matrix3x4 _MulMatrix3x4(const Matrix3x4& m0, const Matrix3x4& m1);

	// バウンディングボックス(OBB)
	struct OBB
	{
		Matrix4x4 m[2];		// ボックス用行列
	};

	// OBB を設定
	OBB OBB_Set(const Vector4& pos, const Vector4& rot, const Vector4& scale);

	// OBBと線分のバウンディング判定
	int BoundingCheck_OBB_Segment(const OBB& obb, const Segment seg);

	// カプセル
	struct Capsule
	{
		Vector4 start_pos;
		Vector4 end_pos;
		float radius;
	};

	// カプセルの設定
	Capsule Capsule_Set(const Vector4& start, const Vector4& end, float r);
	// 11/15(月)が登校になる可能性大(教室は803)
	// 11/19(金)は不明

	int HitCheck_Capsule_Sphere(const Capsule& caps, const Sphere& sph);

	// 値を範囲内に収める [low,high](補正値が返ってくる)
	float clamp(float x, float low, float high);

	// 線分と線分の距離の2乗を求める
	float SegmentDistanceSquare(const Vector4& p1, const Vector4& q1, const Vector4& p2, const Vector4& q2);

	// カプセル同士
	int HitCheck_Capsule_Capsule(const Capsule& cap0, const Capsule& cap1);

	// ボックスと球の当たり判定
	int HitCheck_AABB_Sphere(const AABB& aabb, const Sphere& sph);

	// カプセルとボックスの当たり判定
	int HitCheck_AABB_Capsule(const AABB& aabb, const Capsule& caps);

	struct Quaternion
	{
		float x, y, z, w;
	};

	// クォータニオンの設定
	Quaternion Quaternion_Set(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f);
	Quaternion Quaternion_Set(const Vector4& v, float angle);

	// 内積
	float Dot_Quaternion(const Quaternion& q1, const Quaternion& q2);

	// 長さ(ノルム)を求める
	float Length_Quaternion(const Quaternion& q);

	// 正規化
	Quaternion Normalize_Quaternion(const Quaternion& q);

	// 単項演算子オーバーロード
	Quaternion operator + (const Quaternion& q);
	Quaternion operator - (const Quaternion& q);

	// 代入演算子オーバーロード
	Quaternion& operator += (Quaternion& q1, const Quaternion& q2);
	Quaternion& operator -= (Quaternion& q1, const Quaternion& q2);
	Quaternion& operator *= (Quaternion& q, float s);
	Quaternion& operator /= (Quaternion& q, float s);
	Quaternion& operator *= (Quaternion& q1, const Quaternion& q2);

	// 二項演算子オーバーロード
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