#include "TypeCollision.h"

using namespace Math;

Vector4 Math::SetVector4(float x, float y, float z)
{
	return Vector4(x, y, z, 0.0f);
}

Vector4 Math::SetPoint4(float x, float y, float z)
{
	return Vector4(x, y, z, 1.0f);
}

Segment Math::Segment_Set(float px, float py, float pz, float vx, float vy, float vz)
{
	// 始点ベクトルを設定
	Segment r;
	r.pos = SetPoint4(px, py, pz);
	r.vec = Vector4(vx, vy, vz);
	return r;
}

Sphere Math::Sphere_Set(float x, float y, float z, float r)
{
	Sphere s;
	s.pos = SetPoint4(x, y, z);
	s.r = r;
	return s;
}

Plane Math::Plane_Set(const Vector4& normal, const Vector4& p)
{
	// 法線を正規化しておく
	Vector4 n = VecNormalize4(normal);

	// 平面のa,b,cを設定
	Plane r;
	r.a = n.x;
	r.b = n.y;
	r.c = n.z;

	// 平面のdを設定
	r.d = -((n.x * p.x) + (n.y * p.y) + (n.z * p.z));
	return r;
}

Plane Math::Plane_Set(const Vector4& p0, const Vector4& p1, const Vector4& p2)
{
	// 法線を求める
	Vector4 n = CrossVec4(p0, p1, p2);

	// 別関数で処理
	return (Plane_Set(n, p0));
}

Triangle Math::Triangle_Set(const Vector4& p0, const Vector4& p1, const Vector4& p2)
{
	// 3点を利用した平面を求める
	Plane plane = Plane_Set(p0, p1, p2);

	// 三角形の各設定
	Triangle r;
	r.pos[0] = p0;
	r.pos[1] = p1;
	r.pos[2] = p2;
	r.plane = plane;
	return r;
}

float Math::Plane_Length(const Plane& pln, const Vector4& pos)
{
	return pln.a * pos.x + pln.b * pos.y + pln.c * pos.z + pln.d;
}

int Math::Plane_Side(const Plane& plane, const Vector4& pos)
{
	// 距離の値で判断
	if (Plane_Length(plane, pos) < 0.0f)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int Math::HitCheck_Segment2_Segment2(Vector2* hit_pos, const Segment2& s0, const Segment2& s1)
{
	// X軸方向のベクトルが0か？
	if ((s0.vec.x == 0.0f) || (s0.vec.y == 0.0f))
	{
		// X軸方向のベクトルが0のときは傾きが計算できないので特殊
		if ((s0.vec.x == 0.0f) && (s0.vec.y == 0.0f))return 0;

		Vector2 r;
		float t0, t1;
		if (s0.vec.x == 0.0f)
		{
			r.x = s0.pos.x;
			r.y = (s1.vec.y / s1.vec.x) * (r.x - s1.pos.x) + s1.pos.y;

			t0 = (r.y - s0.pos.y) / s0.vec.y; // t = 0~1のときは線分s0内
			t1 = (r.x - s1.pos.x) / s1.vec.x; // t = 0~1のときは線分s1内
		}
		else
		{
			r.x = s1.pos.x;
			r.y = (s0.vec.y / s0.vec.x) * (r.x - s0.pos.x) + s0.pos.y;

			t0 = (r.x - s0.pos.x) / s0.vec.x; // t = 0~1のときは線分s0内
			t1 = (r.y - s1.pos.y) / s1.vec.y; // t = 0~1のときは線分s1内
		}
		if ((t0 < 0.0f) || (t0 > 1.0f) || (t1 < 0.0f) || (t1 > 1.0f))return 0;
		if (hit_pos) *hit_pos = r;
		return 1;
	}
	else
	{
		// 線分の傾きを求める
		float a0 = s0.vec.y / s0.vec.x;
		float a1 = s1.vec.y / s1.vec.x;

		// 傾きが同一の場合は平行なので衝突しない
		if ((a0 == a1) || (a0 == -a1))return 0;

		// 交点のx,y座標を求める
		Vector2 r;
		r.x = (a0 * s0.pos.x - a1 * s1.pos.x + s1.pos.y - s0.pos.y) / (a0 - a1);
		r.y = a0 * (r.x - s0.pos.x) + s0.pos.y;

		// 交点が線分内にあるか調べる
		float t0 = (r.x - s0.pos.x) / s0.vec.x;
		float t1 = (r.x - s1.pos.x) / s1.vec.x;
		if ((t0 < 0.0f) || (t0 > 1.0f) || (t1 < 0.0f) || (t1 > 1.0f))return 0;

		// 衝突していることを返す
		if (hit_pos) *hit_pos = r;
		return 1;
	}
}

int Math::HitCheck_Sphere_Segment( const Sphere& sph, const Segment& seg, float* distance, Vector4* hit_pos)
{
	Vector4 m = SubVec4(seg.pos, sph.pos);
	float b = DotVec4(m, seg.vec);
	float c = DotVec4(m, m) - sph.r * sph.r;

	if (c > 0.0f && b > 0.0f)return 0;

	float discr = b * b - c;
	// 負の判別式はレイが球を外れていることに一致
	if (discr < 0.0f)return 0;

	// レイは球と交差している。
	// 交差する最小値tを計算
	float t = -b - sqrtf(discr);

	// tが負である場合、レイは球の内側から開始しているのでtをゼロにクランプ
	if (t < 0)t = 0.0f;
	if (distance) { *distance = t; }

	if (hit_pos) *hit_pos = AddVec4(seg.pos, MulVec4(seg.vec, t));

	return 1;
}

int Math::HitCheck_Plane_Segment(const Plane& pln, const Segment& seg, float* distance, Vector4* hit_pos)
{
	const float epsilon = 1.0e-5f;		// 誤差吸収用微小な値
	// 面法線とレイの方向ベクトルの内積
	Vector4 plane_normal = { pln.a,pln.b,pln.c,pln.d };
	float d1 = DotVec4(plane_normal, seg.vec);
	// 裏面には当たらない
	if (d1 > -epsilon) { return 0; }
	// 視点と原点の距離 (平面の法線方向)
	// 面法線とレイの始点座標(位置ベクトル)の内積
	float d2 = DotVec4(plane_normal, seg.pos);
	// 始点と平面の距離（平面の法線方向）
	float dist = d2 - pln.distance;
	// 始点と平面の距離（レイ方向）
	float t = dist / -d1;
	// 交点が始点より後ろにあるので、当たらない
	if (t < 0)return 0;
	// 距離を書き込む
	if (distance) { *distance = t; }
	// 交点を計算
	if (hit_pos) { *hit_pos = AddVec4(seg.pos, MulVec4(seg.vec, t)); }
	return 1;
}

int Math::Triangle_CheckInner(const Triangle& tri, const Vector4& p)
{
	// 任意点から各頂点へのベクトル作成
	Vector4 vt0 = SubVec4(tri.pos[0], p);
	Vector4 vt1 = SubVec4(tri.pos[1], p);
	Vector4 vt2 = SubVec4(tri.pos[2], p);

	// 三角形の辺をなぞるベクトル作成
	Vector4 v0 = SubVec4(tri.pos[1], tri.pos[0]);
	Vector4 v1 = SubVec4(tri.pos[2], tri.pos[1]);
	Vector4 v2 = SubVec4(tri.pos[0], tri.pos[2]);

	// 各外積を求める
	Vector4 c0 = CrossVec4(vt0, v0);
	Vector4 c1 = CrossVec4(vt1, v1);
	Vector4 c2 = CrossVec4(vt2, v2);

	// 外積の向きが揃っているかチェック(内積利用)
	float dot0 = DotVec4(c0, c1);
	float dot1 = DotVec4(c1, c2);
	if ((dot0 * dot1) <= 0.0f)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int Math::HitCheck_Triangle_Segment(const Triangle& tri, const Segment& seg, float* distance, Vector4* hit_pos)
{

	// 三角形が載っている平面を算出
	Plane plane;
	Vector4 interPlane;

	plane = tri.plane;
	plane.distance = DotVec4({ plane.a,plane.b ,plane.c ,plane.d }, tri.pos[0]);
	// レイと平面が当たっていなければ、当たっていない
	if (!HitCheck_Plane_Segment(plane, seg, distance, &interPlane)) return 0;
	// レイと平面が当たっていたので、距離と交点が書き込まれた
	// レイと平面の交点が三角形の内側にあるか判定
	const float epsilon = 1.0e-5f;		// 誤差吸収用微小な値
	Vector4 m;
	// 辺p0_p1について
	Vector4 pt_p0 = SubVec4(tri.pos[0] ,interPlane);
	Vector4 p0_p1 = SubVec4(tri.pos[1],tri.pos[0]);
	m = CrossVec4(pt_p0, p0_p1);
	// 辺の外側であれば当たってないので判定を打ち切る
	if (DotVec4(m, { tri.plane.a,tri.plane.b ,tri.plane.c ,tri.plane.d }) < -epsilon)return 0;
	// 辺p1_p2について
	Vector4 pt_p1 = SubVec4(tri.pos[1], interPlane);
	Vector4 p1_p2 = SubVec4(tri.pos[2], tri.pos[1]);
	m = CrossVec4(pt_p1, p1_p2);
	// 辺の外側であれば当たってないので判定を打ち切る
	if (DotVec4(m, { tri.plane.a,tri.plane.b ,tri.plane.c ,tri.plane.d }) < -epsilon)return 0;
	// 辺p2_p0について
	Vector4 pt_p2 = SubVec4(tri.pos[2], interPlane);
	Vector4 p2_p0 = SubVec4(tri.pos[0], tri.pos[2]);
	m = CrossVec4(pt_p2, p2_p0);
	// 辺の外側であれば当たってないので判定を打ち切る
	if (DotVec4(m, { tri.plane.a,tri.plane.b ,tri.plane.c ,tri.plane.d }) < -epsilon)return 0;
	// 内側なので、当たっている
	if (hit_pos)
	{
		*hit_pos = interPlane;
	}

	return 1;
}

int Math::HitCheck_Sphere_Sphere( const Sphere& sph0, const Sphere& sph1, Vector4* hit_vec, Vector4* reject)
{
	// 中心間の距離の2乗を獲得
	Vector4 vec = SubVec4(sph0.pos, sph1.pos);
	float l2 = vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
	float radius2 = sph0.r + sph1.r;
	radius2 *= radius2;

	// 距離の2乗の比較で届くかどうか調べる
	if (l2 > radius2)return 0;

	// 衝突しているのでめり込みに関する情報を取得
	if (hit_vec)
	{
		// Aの半径が0の時座標はBの中心　Bの半径が0の時座標はAの中心　となるよう補完
		float t = sph1.r / (sph0.r + sph1.r);
		*hit_vec = VectorLerp(sph0.pos, sph1.pos, t);
	}
	if (reject)
	{
		float l = (sph0.r + sph1.r) - sqrtf(l2);	// めり込み量
		*reject = VecNormalize4(vec);
		*reject = MulVec4(*reject, l);
	}

	// 衝突していることを返す
	return 1;
}

int Math::HitCheck_Plane_Sphere(Vector4* hit_vec, const Plane& pln, const Sphere& sph)
{
	// 平面と球の中心の距離で判断
	float l = Plane_Length(pln, sph.pos);
	if (l > sph.r)return 0;

	// どれだけめり込んでいるかを調べる
	if (hit_vec)
	{
		l = sph.r - l;	// めり込み量
		*hit_vec = SetVector4(pln.a * l, pln.b * l, pln.c * l);
		// 平面の法線を利用する
	}

	// 衝突していることを返す
	return 1;
}

void Math::ClosestPtPoint_Triangle(const Vector4& point, const Triangle& triangle, Vector4* closest)
{
	Vector4 p0_p1 = SubVec4(triangle.pos[1], triangle.pos[0]);
	Vector4 p0_p2 = SubVec4(triangle.pos[2], triangle.pos[0]);
	Vector4 p0_pt = SubVec4(point, triangle.pos[0]);

	float d1 = DotVec4(p0_p1, p0_pt);
	float d2 = DotVec4(p0_p2, p0_pt);

	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		// p0が最近傍
		*closest = triangle.pos[0];
		return;
	}

	// pointがp1の外側の頂点領域の中にあるかどうかチェック
	Vector4 p1_pt = SubVec4(point, triangle.pos[1]);

	float d3 = DotVec4(p0_p1, p1_pt);
	float d4 = DotVec4(p0_p2, p1_pt);

	if (d3 >= 0.0f && d4 <= d3)
	{
		// p1が最近傍
		*closest = triangle.pos[1];
		return;
	}

	// pointがp0_p1の辺領域の中にあるかどうかチェックし、あればpointのp0_p1上に対する射影を返す
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		*closest = AddVec4(triangle.pos[0],MulVec4(p0_p1,v));
		return;
	}

	// pointがp2の外側の頂点領域の中にあるかどうかチェック
	Vector4 p2_pt = SubVec4(point, triangle.pos[2]);

	float d5 = DotVec4(p0_p1, p2_pt);
	float d6 = DotVec4(p0_p2, p2_pt);
	if (d6 >= 0.0f && d5 <= d6)
	{
		*closest = triangle.pos[2];
		return;
	}

	// pointがp0_p2の辺領域の中にあるかどうかチェックし、あればpointのp0_p2上に対する射影を返す
	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		*closest = AddVec4(triangle.pos[0], MulVec4(p0_p2, w));
		return;
	}

	// pointがp1_p2の辺領域の中にあるかどうかチェックし、あればpointのp1_p2上に対する射影を返す
	float va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		*closest = AddVec4(triangle.pos[1], MulVec4(SubVec4(triangle.pos[2], triangle.pos[1]), w));
		// AddVec4(triangle.pos[1],MulVec4(SubVec4(triangle.pos[2],triangle.pos[1]),w))
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*closest = AddVec4(AddVec4(triangle.pos[0], MulVec4(p0_p1, v)),MulVec4(p0_p2 , w));
}

int Math::HitCheck_Triangle_Sphere(const Triangle& tri, const Sphere& sph, Vector4* hit_vec, Vector4* reject)
{
	// 三角形を含む平面と球とでチェック
	if (!HitCheck_Plane_Sphere(NULL, tri.plane, sph))return 0;

	Vector4 p;
	ClosestPtPoint_Triangle(sph.pos, tri, &p);
	// 点pと球の中心の差分ベクトル
	Vector4 v = SubVec4(p, sph.pos);
	// 距離の二乗を求める
	//（同じベクトル同士の内積は三平方の定理のルート内部の式と一致する）
	float distanceSquare = DotVec4(v, v);
	// 球と三角形の距離が半径以下なら当たっていない
	if (distanceSquare > sph.r * sph.r)	return 0;
	// 擬似交点を計算
	if (hit_vec) {
		// 三角形上の最近接点pを疑似交点とする
		*hit_vec = p;
	}
	// 押し出すベクトルを計算
	if (reject) {
		Vector4 normal = { tri.plane.a,tri.plane.b,tri.plane.c,tri.plane.d };
		float ds = DotVec4(sph.pos, normal);
		float dt = DotVec4(tri.pos[0], normal);
		float rejectLen = dt - ds + sph.r;
		*reject = MulVec4(normal, rejectLen);
	}
	return 1;
}

int Math::HitCheck_Plane_Plane(Segment* cross_seg, const Plane& pln0, const Plane& pln1)
{
	// 平面の法線を利用し、平面が平行になっているかチェック
	float dot = (pln0.a * pln1.a) + (pln0.b * pln1.b) + (pln0.c * pln1.c);
	if ((dot >= 1.0f) || (dot <= -1.0f))return 0;

	// 交差線分を必要に応じて作成
	if (cross_seg)
	{
		// 外積を利用して交差する線分ベクトルを作成
		Vector4 vec = CrossVec4(SetVector4(pln0.a, pln0.b, pln0.c),
			SetVector4(pln1.a, pln1.b, pln1.c));

		// 交差線分の始点を算出
		Vector4 pos;
		if (vec.z != 0.0f)
		{
			pos = SetPoint4(
				(pln0.b * pln1.d - pln1.b * pln0.d) / vec.z,
				(pln1.a * pln0.d - pln0.a * pln1.d) / vec.z,
				0.0f
			);
		}
		else if (vec.y != 0.0f)
		{
			pos = SetPoint4(
				(pln1.c * pln0.d - pln0.c * pln1.d) / vec.y,
				0.0f,
				(pln0.a * pln1.d - pln1.a * pln0.d) / vec.y
			);
		}
		else
		{
			pos = SetPoint4(
				0.0f,
				(pln0.c * pln1.d - pln1.c * pln0.d) / vec.x,
				(pln1.b * pln0.d - pln0.b * pln1.d) / vec.x
			);
		}

		// 線分の設定
		*cross_seg = Segment_Set(pos.x, pos.y, pos.z, vec.x, vec.y, vec.z);
	}

	// 衝突していることを返す
	return 1;
}

int Math::HitCheck_Plane_Triangle(const Plane& pln, const Triangle& tri)
{
	// 三角形の1点目が平面に対して表か裏のどちら側に存在しているかをチェック
	int side0 = Plane_Side(pln, tri.pos[0]);

	// 三角形の2点目が平面に対して表か裏のどちら側に存在しているかをチェックし、
	// 衝突を判定する
	int side1 = Plane_Side(pln, tri.pos[1]);
	if ((side0 && !side1) || (!side0 && side1)) return 1;
	// 表裏に存在するので衝突

	// 三角形の3点目が平面に対して表か裏のどちら側に存在しているかをチェックし、
	// 衝突を判定する
	int side2 = Plane_Side(pln, tri.pos[2]);
	if ((side1 && !side2) || (!side1 && side2)) return 1;
	// 表裏に存在するので衝突

	// 衝突していないことを返す
	return 0;
}

int Math::HitCheck_Triangle_Triangle(const Triangle& tri0, const Triangle& tri1)
{
	// 片方の三角形の2辺がもう片方の三角形を貫いているか？
	Vector4 vec0_0 = SubVec4(tri0.pos[1], tri0.pos[0]);
	Vector4 vec0_1 = SubVec4(tri0.pos[2], tri0.pos[1]);
	Vector4 vec0_2 = SubVec4(tri0.pos[0], tri0.pos[2]);
	int hit0_0 = HitCheck_Triangle_Segment(tri1,
		Segment_Set(tri0.pos[0].x, tri0.pos[0].y, tri0.pos[0].z, vec0_0.x, vec0_0.y, vec0_0.z));
	int hit0_1 = HitCheck_Triangle_Segment(tri1,
		Segment_Set(tri0.pos[1].x, tri0.pos[1].y, tri0.pos[1].z, vec0_1.x, vec0_1.y, vec0_1.z));
	int hit0_2 = HitCheck_Triangle_Segment(tri1,
		Segment_Set(tri0.pos[2].x, tri0.pos[2].y, tri0.pos[2].z, vec0_2.x, vec0_2.y, vec0_2.z));
	if ((hit0_0 && hit0_1) || (hit0_1 && hit0_2) || (hit0_2 && hit0_0))return 1;

	// 三角形を入れ替えて2辺のチェックを行う
	Vector4 vec1_0 = SubVec4(tri1.pos[1], tri1.pos[0]);
	Vector4 vec1_1 = SubVec4(tri1.pos[2], tri1.pos[1]);
	Vector4 vec1_2 = SubVec4(tri1.pos[0], tri1.pos[2]);
	int hit1_0 = HitCheck_Triangle_Segment(tri0,
		Segment_Set(tri1.pos[0].x, tri1.pos[0].y, tri1.pos[0].z, vec1_0.x, vec1_0.y, vec1_0.z));
	int hit1_1 = HitCheck_Triangle_Segment(tri0,
		Segment_Set(tri1.pos[1].x, tri1.pos[1].y, tri1.pos[1].z, vec1_1.x, vec1_1.y, vec1_1.z));
	int hit1_2 = HitCheck_Triangle_Segment(tri0,
		Segment_Set(tri1.pos[2].x, tri1.pos[2].y, tri1.pos[2].z, vec1_2.x, vec1_2.y, vec1_2.z));
	if ((hit1_0 && hit1_1) || (hit1_1 && hit1_2) || (hit1_2 && hit1_0))return 1;

	// お互いに1辺ずつ貫通しているかどうか調べる
	if ((hit0_0 || hit0_1 || hit0_2) && (hit1_0 || hit1_1 || hit1_2))return 1;

	// 衝突していないことを返す
	return 0;
}

int Math::BoundingCheck_BoundingSphere_BoundingSphere(const BoundingSphere& bs0, const BoundingSphere& bs1)
{
	// 中心間の距離の2乗を獲得
	Vector4 vec = SubVec4(bs1.pos, bs0.pos);
	float l2 = vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;

	// 距離の2乗の比較で届くかどうか調べる
	if (l2 > ((bs0.r + bs1.r) * (bs0.r + bs1.r)))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int Math::BoundingCheck_AABB_Segment(const AABB& aabb, const Segment& seg)
{
	// 結果格納用
	float t_min, t_max;

	// X軸領域での判定
	float tx_min, tx_max;
	if ((seg.vec.x == 0.0f))
	{
		// 線分ベクトルが0のときは特殊
		if ((seg.pos.x < aabb.min.x) || (seg.pos.x > aabb.max.x))
			return 0;
		tx_min = 0.0f;
		tx_max = 1.0f;
	}
	else
	{
		// X軸での領域を決定
		float t0 = (aabb.min.x - seg.pos.x) / seg.vec.x;
		float t1 = (aabb.max.x - seg.pos.x) / seg.vec.x;
		if (t0 < t1)
		{
			tx_min = t0;
			tx_max = t1;
		}
		else
		{
			tx_min = t1;
			tx_max = t0;
		}
		if ((tx_max < 0.0f) || (tx_min > 1.0f)) return 0;
		// X軸領域内ですでに範囲外
	}
	t_min = tx_min;
	t_max = tx_max;

	// Y軸領域での判定
	float ty_min, ty_max;
	if ((seg.vec.y == 0.0f))
	{
		// 線分ベクトルが0のときは特殊
		if ((seg.pos.y < aabb.min.y) || (seg.pos.y > aabb.max.y))
			return 0;
		ty_min = 0.0f;
		ty_max = 1.0f;
	}
	else
	{
		// Y軸での領域を決定
		float t0 = (aabb.min.y - seg.pos.y) / seg.vec.y;
		float t1 = (aabb.max.y - seg.pos.y) / seg.vec.y;
		if (t0 < t1)
		{
			ty_min = t0;
			ty_max = t1;
		}
		else
		{
			ty_min = t1;
			ty_max = t0;
		}
		if ((ty_max < 0.0f) || (ty_min > 1.0f)) return 0;
		// Y軸領域内ですでに範囲外
	}
	if ((t_max < ty_min) || (t_min > ty_max)) return 0;
	// X軸とY軸の領域がかぶってない!
	if (t_min < ty_min)t_min = ty_min;
	if (t_max > ty_max)t_max = ty_max;

	// Z軸領域での判定
	float tz_min, tz_max;
	if ((seg.vec.z == 0.0f))
	{
		// 線分ベクトルが0のときは特殊
		if ((seg.pos.z < aabb.min.z) || (seg.pos.z > aabb.max.z))
			return 0;
		tz_min = 0.0f;
		tz_max = 1.0f;
	}
	else
	{
		// Z軸での領域を決定
		float t0 = (aabb.min.z - seg.pos.z) / seg.vec.z;
		float t1 = (aabb.max.z - seg.pos.z) / seg.vec.z;
		if (t0 < t1)
		{
			tz_min = t0;
			tz_max = t1;
		}
		else
		{
			tz_min = t1;
			tz_max = t0;
		}
		if ((tz_max < 0.0f) || (tz_min > 1.0f)) return 0;
		// Z軸領域内ですでに範囲外
	}
	if ((t_max < tz_min) || (t_min > tz_max)) return 0;
	// X,Y軸とZ軸の領域がかぶってない!
	if (t_min < tz_min)t_min = tz_min;
	if (t_max > tz_max)t_max = tz_max;

	// 共通領域のチェック
	if ((t_min > 1.0f) || (t_max < 0.0f))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int Math::BoundingCheck_AABB_AABB(const AABB& aabb1, const AABB& aabb2)
{
	return (aabb1.max.x > aabb2.min.x && aabb1.min.x < aabb2.max.x&&
		aabb1.max.y > aabb2.min.y && aabb1.min.y < aabb2.max.y&&
		aabb1.max.z > aabb2.min.z && aabb1.min.z < aabb2.max.z);
}

Matrix4x4 Math::Matrix4x4_Array(const Matrix4x4& mat)
{
	Matrix4x4 result = mat;
	result.m[0][0] = mat.m00; result.m[0][1] = mat.m01; result.m[0][2] = mat.m02; result.m[0][3] = mat.m03;
	result.m[1][0] = mat.m10; result.m[1][1] = mat.m11; result.m[1][2] = mat.m12; result.m[1][3] = mat.m13;
	result.m[2][0] = mat.m20; result.m[2][1] = mat.m21; result.m[2][2] = mat.m22; result.m[2][3] = mat.m23;
	result.m[3][0] = mat.m30; result.m[3][1] = mat.m31; result.m[3][2] = mat.m32; result.m[3][3] = mat.m33;

	return result;
}

Matrix4x4 Math::_Identity()
{
	Matrix4x4 m;
	m.m00 = 1.0f; m.m01 = 0.0f; m.m02 = 0.0f; m.m03 = 0.0f;
	m.m10 = 0.0f; m.m11 = 1.0f; m.m12 = 0.0f; m.m13 = 0.0f;
	m.m20 = 0.0f; m.m21 = 0.0f; m.m22 = 1.0f; m.m23 = 0.0f;
	m.m30 = 0.0f; m.m31 = 0.0f; m.m32 = 0.0f; m.m33 = 1.0f;
	return m;
}

Matrix4x4 Math::_Scale(float sx, float sy, float sz)
{
	Matrix4x4 m;
	m.m00 = sx;   m.m01 = 0.0f; m.m02 = 0.0f; m.m03 = 0.0f;
	m.m10 = 0.0f; m.m11 = sy;   m.m12 = 0.0f; m.m13 = 0.0f;
	m.m20 = 0.0f; m.m21 = 0.0f; m.m22 = sz;   m.m23 = 0.0f;
	m.m30 = 0.0f; m.m31 = 0.0f; m.m32 = 0.0f; m.m33 = 1.0f;
	m = Matrix4x4_Array(m);
	return m;
}

Matrix4x4 Math::_RotationX(float rx)
{
	Matrix4x4 m;
	m.m00 = 1.0f; m.m01 = 0.0f;     m.m02 = 0.0f;      m.m03 = 0.0f;
	m.m10 = 0.0f; m.m11 = cosf(rx); m.m12 = -sinf(rx); m.m13 = 0.0f;
	m.m20 = 0.0f; m.m21 = sinf(rx); m.m22 = cosf(rx); m.m23 = 0.0f;
	m.m30 = 0.0f; m.m31 = 0.0f;     m.m32 = 0.0f;      m.m33 = 1.0f;
	m = Matrix4x4_Array(m);
	return m;
}

Matrix4x4 Math::_RotationY(float ry)
{
	Matrix4x4 m;
	m.m00 = cosf(ry); m.m01 = 0.0f; m.m02 = sinf(ry); m.m03 = 0.0f;
	m.m10 = 0.0f;     m.m11 = 1.0f; m.m12 = 0.0f;    m.m13 = 0.0f;
	m.m20 = -sinf(ry); m.m21 = 0.0f; m.m22 = cosf(ry); m.m23 = 0.0f;
	m.m30 = 0.0f;     m.m31 = 0.0f; m.m32 = 0.0f;    m.m33 = 1.0f;
	m = Matrix4x4_Array(m);
	return m;
}

Matrix4x4 Math::_RotationZ(float rz)
{
	Matrix4x4 m;
	m.m00 = cosf(rz); m.m01 = -sinf(rz); m.m02 = 0.0f; m.m03 = 0.0f;
	m.m10 = sinf(rz); m.m11 = cosf(rz); m.m12 = 0.0f; m.m13 = 0.0f;
	m.m20 = 0.0f;     m.m21 = 0.0f;      m.m22 = 1.0f; m.m23 = 0.0f;
	m.m30 = 0.0f;     m.m31 = 0.0f;      m.m32 = 0.0f; m.m33 = 1.0f;
	m = Matrix4x4_Array(m);
	return m;
}

Matrix4x4 Math::_Translate(float tx, float ty, float tz)
{
	Matrix4x4 m;
	m.m00 = 1.0f; m.m01 = 0.0f; m.m02 = 0.0f; m.m03 = tx;
	m.m10 = 0.0f; m.m11 = 1.0f; m.m12 = 0.0f; m.m13 = ty;
	m.m20 = 0.0f; m.m21 = 0.0f; m.m22 = 1.0f; m.m23 = tz;
	m.m30 = 0.0f; m.m31 = 0.0f; m.m32 = 0.0f; m.m33 = 1.0f;
	m = Matrix4x4_Array(m);
	return m;
}

Vector4 Math::Matrix4x4_x_Vector4(const Matrix4x4& m, const Vector4& v)
{
	Vector4 r;
	r.x = m.m00 * v.x + m.m01 * v.y + m.m02 * v.z + m.m03 * v.w;
	r.y = m.m10 * v.x + m.m11 * v.y + m.m12 * v.z + m.m13 * v.w;
	r.z = m.m20 * v.x + m.m21 * v.y + m.m22 * v.z + m.m23 * v.w;
	r.w = m.m30 * v.x + m.m31 * v.y + m.m32 * v.z + m.m33 * v.w;
	return r;
}

Matrix4x4 Math::_MulMatrix4x4(const Matrix4x4& m0, const Matrix4x4& m1)
{
	Matrix4x4 r;
	r.m00 = m0.m00 * m1.m00 + m0.m01 * m1.m10 + m0.m02 * m1.m20 + m0.m03 * m1.m30;
	r.m01 = m0.m00 * m1.m01 + m0.m01 * m1.m11 + m0.m02 * m1.m21 + m0.m03 * m1.m31;
	r.m02 = m0.m00 * m1.m02 + m0.m01 * m1.m12 + m0.m02 * m1.m22 + m0.m03 * m1.m32;
	r.m03 = m0.m00 * m1.m03 + m0.m01 * m1.m13 + m0.m02 * m1.m23 + m0.m03 * m1.m33;

	r.m10 = m0.m10 * m1.m00 + m0.m11 * m1.m10 + m0.m12 * m1.m20 + m0.m13 * m1.m30;
	r.m11 = m0.m10 * m1.m01 + m0.m11 * m1.m11 + m0.m12 * m1.m21 + m0.m13 * m1.m31;
	r.m12 = m0.m10 * m1.m02 + m0.m11 * m1.m12 + m0.m12 * m1.m22 + m0.m13 * m1.m32;
	r.m13 = m0.m10 * m1.m03 + m0.m11 * m1.m13 + m0.m12 * m1.m23 + m0.m13 * m1.m33;

	r.m20 = m0.m20 * m1.m00 + m0.m21 * m1.m10 + m0.m22 * m1.m20 + m0.m23 * m1.m30;
	r.m21 = m0.m20 * m1.m01 + m0.m21 * m1.m11 + m0.m22 * m1.m21 + m0.m23 * m1.m31;
	r.m22 = m0.m20 * m1.m02 + m0.m21 * m1.m12 + m0.m22 * m1.m22 + m0.m23 * m1.m32;
	r.m23 = m0.m20 * m1.m03 + m0.m21 * m1.m13 + m0.m22 * m1.m23 + m0.m23 * m1.m33;

	r.m30 = m0.m30 * m1.m00 + m0.m31 * m1.m10 + m0.m32 * m1.m20 + m0.m33 * m1.m30;
	r.m31 = m0.m30 * m1.m01 + m0.m31 * m1.m11 + m0.m32 * m1.m21 + m0.m33 * m1.m31;
	r.m32 = m0.m30 * m1.m02 + m0.m31 * m1.m12 + m0.m32 * m1.m22 + m0.m33 * m1.m32;
	r.m33 = m0.m30 * m1.m03 + m0.m31 * m1.m13 + m0.m32 * m1.m23 + m0.m33 * m1.m33;
	r = Matrix4x4_Array(r);
	return r;
}

Matrix4x4 Math::_InverseMatrix(const Matrix4x4& m)
{
	float det;
	det =
		m.m00 * m.m11 * m.m22 * m.m33 + m.m00 * m.m12 * m.m23 * m.m31 + m.m00 * m.m13 * m.m21 * m.m32 +
		m.m01 * m.m10 * m.m23 * m.m32 + m.m01 * m.m12 * m.m20 * m.m33 + m.m01 * m.m13 * m.m22 * m.m30 +
		m.m02 * m.m10 * m.m21 * m.m33 + m.m02 * m.m11 * m.m23 * m.m30 + m.m02 * m.m13 * m.m20 * m.m31 +
		m.m03 * m.m10 * m.m22 * m.m31 + m.m03 * m.m11 * m.m20 * m.m32 + m.m03 * m.m12 * m.m21 * m.m30 -
		m.m00 * m.m11 * m.m23 * m.m32 - m.m00 * m.m12 * m.m21 * m.m33 - m.m00 * m.m13 * m.m22 * m.m31 -
		m.m01 * m.m10 * m.m22 * m.m33 - m.m01 * m.m12 * m.m23 * m.m30 - m.m01 * m.m13 * m.m20 * m.m32 -
		m.m02 * m.m10 * m.m23 * m.m31 - m.m02 * m.m11 * m.m20 * m.m33 - m.m02 * m.m13 * m.m21 * m.m30 -
		m.m03 * m.m10 * m.m21 * m.m32 - m.m03 * m.m11 * m.m22 * m.m30 - m.m03 * m.m12 * m.m20 * m.m31;
	// Δ(デルタ) = 0のときは処理できないのでそのまま返す
	if (det == 0.0f)return m;
	det = 1.0f / det;		// 予め逆数にしておく

	Matrix4x4 r;
	r.m00 = det * (m.m11 * m.m22 * m.m33 + m.m12 * m.m23 * m.m31 + m.m13 * m.m21 * m.m32 -
		m.m11 * m.m23 * m.m32 - m.m12 * m.m21 * m.m33 - m.m13 * m.m22 * m.m31);
	r.m01 = det * (m.m01 * m.m23 * m.m32 + m.m02 * m.m21 * m.m33 + m.m03 * m.m22 * m.m31 -
		m.m01 * m.m22 * m.m33 - m.m02 * m.m23 * m.m31 - m.m03 * m.m21 * m.m32);
	r.m02 = det * (m.m01 * m.m12 * m.m33 + m.m02 * m.m13 * m.m31 + m.m03 * m.m11 * m.m32 -
		m.m01 * m.m13 * m.m32 - m.m02 * m.m11 * m.m33 - m.m03 * m.m12 * m.m31);
	r.m03 = det * (m.m01 * m.m13 * m.m22 + m.m02 * m.m11 * m.m23 + m.m03 * m.m12 * m.m21 -
		m.m01 * m.m12 * m.m23 - m.m02 * m.m13 * m.m21 - m.m03 * m.m11 * m.m22);

	r.m10 = det * (m.m10 * m.m23 * m.m32 + m.m12 * m.m20 * m.m33 + m.m13 * m.m22 * m.m30 -
		m.m10 * m.m22 * m.m33 - m.m12 * m.m23 * m.m30 - m.m13 * m.m20 * m.m32);
	r.m11 = det * (m.m00 * m.m22 * m.m33 + m.m02 * m.m23 * m.m30 + m.m03 * m.m20 * m.m32 -
		m.m00 * m.m23 * m.m32 - m.m02 * m.m20 * m.m33 - m.m03 * m.m22 * m.m30);
	r.m12 = det * (m.m00 * m.m13 * m.m32 + m.m02 * m.m10 * m.m33 + m.m03 * m.m12 * m.m30 -
		m.m00 * m.m12 * m.m33 - m.m02 * m.m13 * m.m30 - m.m03 * m.m10 * m.m32);
	r.m13 = det * (m.m00 * m.m12 * m.m23 + m.m02 * m.m13 * m.m20 + m.m03 * m.m10 * m.m22 -
		m.m00 * m.m13 * m.m22 - m.m02 * m.m10 * m.m23 - m.m03 * m.m12 * m.m20);

	r.m20 = det * (m.m10 * m.m21 * m.m33 + m.m11 * m.m23 * m.m30 + m.m13 * m.m20 * m.m31 -
		m.m10 * m.m23 * m.m31 - m.m11 * m.m20 * m.m33 - m.m13 * m.m21 * m.m30);
	r.m21 = det * (m.m00 * m.m23 * m.m31 + m.m01 * m.m20 * m.m33 + m.m03 * m.m21 * m.m30 -
		m.m00 * m.m21 * m.m33 - m.m01 * m.m23 * m.m30 - m.m03 * m.m20 * m.m31);
	r.m22 = det * (m.m00 * m.m11 * m.m33 + m.m01 * m.m13 * m.m30 + m.m03 * m.m10 * m.m31 -
		m.m00 * m.m13 * m.m31 - m.m01 * m.m10 * m.m33 - m.m03 * m.m11 * m.m30);
	r.m23 = det * (m.m00 * m.m13 * m.m21 + m.m01 * m.m10 * m.m23 + m.m03 * m.m11 * m.m20 -
		m.m00 * m.m11 * m.m23 - m.m01 * m.m13 * m.m20 - m.m03 * m.m10 * m.m21);

	r.m30 = det * (m.m10 * m.m22 * m.m31 + m.m11 * m.m20 * m.m32 + m.m12 * m.m21 * m.m30 -
		m.m10 * m.m21 * m.m32 - m.m11 * m.m22 * m.m30 - m.m12 * m.m20 * m.m31);
	r.m31 = det * (m.m00 * m.m21 * m.m32 + m.m01 * m.m22 * m.m30 + m.m02 * m.m20 * m.m31 -
		m.m00 * m.m22 * m.m31 - m.m01 * m.m20 * m.m32 - m.m02 * m.m21 * m.m30);
	r.m32 = det * (m.m00 * m.m12 * m.m31 + m.m01 * m.m10 * m.m32 + m.m02 * m.m11 * m.m30 -
		m.m00 * m.m11 * m.m32 - m.m01 * m.m12 * m.m30 - m.m02 * m.m10 * m.m31);
	r.m33 = det * (m.m00 * m.m11 * m.m22 + m.m01 * m.m12 * m.m20 + m.m02 * m.m10 * m.m21 -
		m.m00 * m.m12 * m.m21 - m.m01 * m.m10 * m.m22 - m.m02 * m.m11 * m.m20);
	r = Matrix4x4_Array(r);
	return r;
}

Matrix4x4 Math::_InverseMatrixFast(const Matrix4x4& m)
{
	float det;
	det = m.m00 * m.m11 * m.m22
		+ m.m01 * m.m12 * m.m20
		+ m.m02 * m.m10 * m.m21
		- m.m00 * m.m12 * m.m21
		- m.m01 * m.m10 * m.m22
		- m.m02 * m.m11 * m.m20;
	// Δ(デルタ) = 0のときは処理できないのでそのまま返す
	if (det == 0.0f)return m;
	det = 1.0f / det;		// 予め逆数にしておく

	Matrix4x4 r;
	r.m00 = det * (m.m11 * m.m22 - m.m12 * m.m21);
	r.m01 = det * (m.m02 * m.m21 - m.m01 * m.m22);
	r.m02 = det * (m.m01 * m.m12 - m.m02 * m.m11);
	r.m03 = det * (m.m01 * m.m13 * m.m22 + m.m02 * m.m11 * m.m23 + m.m03 * m.m12 * m.m21 -
		m.m01 * m.m12 * m.m23 - m.m02 * m.m13 * m.m21 - m.m03 * m.m11 * m.m22);

	r.m10 = det * (m.m12 * m.m20 - m.m10 * m.m22);
	r.m11 = det * (m.m00 * m.m22 - m.m02 * m.m20);
	r.m12 = det * (m.m02 * m.m10 - m.m00 * m.m12);
	r.m13 = det * (m.m00 * m.m12 * m.m23 + m.m02 * m.m13 * m.m20 + m.m03 * m.m10 * m.m22 -
		m.m00 * m.m13 * m.m22 - m.m02 * m.m10 * m.m23 - m.m03 * m.m12 * m.m20);

	r.m20 = det * (m.m10 * m.m21 - m.m11 * m.m20);
	r.m21 = det * (m.m01 * m.m20 - m.m00 * m.m21);
	r.m22 = det * (m.m00 * m.m11 - m.m01 * m.m10);
	r.m23 = det * (m.m00 * m.m13 * m.m21 + m.m01 * m.m10 * m.m23 + m.m03 * m.m11 * m.m20 -
		m.m00 * m.m11 * m.m23 - m.m01 * m.m13 * m.m20 - m.m03 * m.m10 * m.m21);

	r.m30 = 0.0f;
	r.m31 = 0.0f;
	r.m32 = 0.0f;
	r.m33 = 1.0f;
	r = Matrix4x4_Array(r);
	return r;
}

Matrix4x4 Math::_Transpose(const Matrix4x4& m)
{
	Matrix4x4 r;
	r.m00 = m.m00; r.m01 = m.m10; r.m02 = m.m20; r.m03 = m.m30;
	r.m10 = m.m01; r.m11 = m.m11; r.m12 = m.m21; r.m13 = m.m31;
	r.m20 = m.m02; r.m21 = m.m12; r.m22 = m.m22; r.m23 = m.m32;
	r.m30 = m.m03; r.m31 = m.m13; r.m32 = m.m23; r.m33 = m.m33;
	r = Matrix4x4_Array(r);
	return r;
}

Matrix3x4 Math::_MulMatrix3x4(const Matrix3x4& m0, const Matrix3x4& m1)
{
	Matrix3x4 r;
	r.m00 = m0.m00 * m1.m00 + m0.m01 * m1.m10 + m0.m02 * m1.m20;
	r.m01 = m0.m00 * m1.m01 + m0.m01 * m1.m11 + m0.m02 * m1.m21;
	r.m02 = m0.m00 * m1.m02 + m0.m01 * m1.m12 + m0.m02 * m1.m22;
	r.m03 = m0.m00 * m1.m03 + m0.m01 * m1.m13 + m0.m02 * m1.m23 + m0.m03;

	r.m10 = m0.m10 * m1.m00 + m0.m11 * m1.m10 + m0.m12 * m1.m20;
	r.m11 = m0.m10 * m1.m01 + m0.m11 * m1.m11 + m0.m12 * m1.m21;
	r.m12 = m0.m10 * m1.m02 + m0.m11 * m1.m12 + m0.m12 * m1.m22;
	r.m13 = m0.m10 * m1.m03 + m0.m11 * m1.m13 + m0.m12 * m1.m23 + m0.m13;

	r.m20 = m0.m20 * m1.m00 + m0.m21 * m1.m10 + m0.m22 * m1.m20;
	r.m21 = m0.m20 * m1.m01 + m0.m21 * m1.m11 + m0.m22 * m1.m21;
	r.m22 = m0.m20 * m1.m02 + m0.m21 * m1.m12 + m0.m22 * m1.m22;
	r.m23 = m0.m20 * m1.m03 + m0.m21 * m1.m13 + m0.m22 * m1.m23 + m0.m23;

	return r;
}

OBB Math::OBB_Set(const Vector4& pos, const Vector4& rot, const Vector4& scale)
{
	// 行列の作成
	Matrix4x4 r;
	r = _Scale(scale.x, scale.y, scale.z);
	r = _MulMatrix4x4(_RotationZ(rot.z), r);			// Z軸回転
	r = _MulMatrix4x4(_RotationX(rot.x), r);			// X軸回転
	r = _MulMatrix4x4(_RotationY(rot.y), r);			// Y軸回転
	r = _MulMatrix4x4(_Translate(pos.x, pos.y, pos.z), r);// 平行移動

	// OBBの行列として設定
	OBB obb;
	obb.m[0] = r;
	obb.m[1] = _InverseMatrix(r); // 逆行列格納
	return obb;
}

int Math::BoundingCheck_OBB_Segment(const OBB& obb, const Segment seg)
{
	// 線分に対しOBBの逆行列を適用させる
	Segment segment;
	segment.pos = Matrix4x4_x_Vector4(obb.m[1], seg.pos);
	segment.vec = Matrix4x4_x_Vector4(obb.m[1], seg.vec);

	// 原点にあるサイズ1相当のAABBと衝突判定
	AABB aabb;
	aabb.min = SetPoint4(-0.5f, -0.5f, -0.5f);
	aabb.max = SetPoint4(0.5f, 0.5f, 0.5f);
	return (BoundingCheck_AABB_Segment(aabb, segment));
}

Capsule Math::Capsule_Set(const Vector4& start, const Vector4& end, float r)
{
	Capsule caps = { start,end,r };
	return caps;
}

int Math::HitCheck_Capsule_Sphere(const Capsule& caps, const Sphere& sph)
{
	// カプセル内の始点から終点までのベクトルを獲得
	Vector4 se_v = SubVec4(caps.end_pos, caps.start_pos);
	// 取得したベクトルを単位化
	Vector4 n = VecNormalize4(se_v);

	// 球からカプセルまでのベクトル
	Vector4 sph_caps = SubVec4(sph.pos, caps.start_pos);
	// 二点の内積を取る
	float t = DotVec4(sph_caps, n);

	Vector4 vPsPn = MulVec4(n, t);
	Vector4 posPn = AddVec4(caps.start_pos, vPsPn);

	// 5. 比率　t / (Ps -> Peの長さ)を求める
	float lengthRate = t / VecLength4(se_v);

	// 6. lengthRate < 0 , 0 <= lengthRate <= 1, 1 < lengthRate で場合分け
	float distance;
	if (lengthRate < 0.0f)
	{
		Vector4 v = SubVec4(caps.start_pos, sph.pos);
		distance = VecLength4(v) - caps.radius;
	}
	else if (lengthRate <= 1)
	{
		Vector4 v = SubVec4(posPn, sph.pos);
		distance = VecLength4(v) - caps.radius;
	}
	else
	{
		Vector4 v = SubVec4(caps.end_pos, sph.pos);
		distance = VecLength4(v) - caps.radius;
	}

	return distance < sph.r;
}

float Math::clamp(float x, float low, float high)
{
	x = (x < low) ? low : x;
	x = (x > high) ? high : x;
	return x;
}

float Math::SegmentDistanceSquare(const Vector4& p1, const Vector4& q1, const Vector4& p2, const Vector4& q2)
{
	Vector4 d1 = SubVec4(q1, p1);	// p1 -> q1 のベクトル
	Vector4 d2 = SubVec4(q2, p2);	// p2 -> q2 のベクトル

	Vector4 r = SubVec4(p1, p2);//(P1 - P2) -> r

	float a = DotVec4(d1, d1);			//  a = d1・d1
	float b = DotVec4(d1, d2);			//  b = d1・d2
	float e = DotVec4(d2, d2);			//  e = d2・d2

	float c = DotVec4(d1, r);			//  c = d1・r
	float f = DotVec4(d2, r);			//  f = d2・r

	float s = 0.0f;
	float t = 0.0f;

	float denominator = a * e - (b * b);			// 分母

	// s の値を求める
	if (denominator != 0.0f)
	{
		s = (b * f - c * e) / denominator;
		s = clamp(s, 0.0f, 1.0f);
	}
	else
	{
		s = 0.0f;
	}

	// t の値を求める
	// t = ((p1 + d1 * s) - p2)・d2 / (d2 ・ d2)
	t = (c + a * s) / b;

	// t が[0.0,1.0]の範囲外であれば、s を再計算する
	// s = ((p2 + d2 * t) - p1)・d1 / (d1・d1) = (t * b - c) / a
	if (t < 0.0f)
	{
		t = 0.0f;
		s = clamp(-c / a, 0.0f, 1.0f);
	}
	else if (t > 1.0f)
	{
		s = clamp((b - c) / a, 0.0f, 1.0f);
		t = 1.0f;
	}
	// s と t の値が決定したので、各線分内の座標 c1 と c2 を求める
	Vector4 c1 = AddVec4(p1, MulVec4(d1, s));
	Vector4 c2 = AddVec4(p2, MulVec4(d2, t));

	// 2点間(c1 と c2)の距離の2乗を求めて、結果を返す
	Vector4 v = SubVec4(c1, c2);
	return DotVec4(v,v);
}

int Math::HitCheck_Capsule_Capsule(const Capsule& cap0, const Capsule& cap1)
{
	// 線分と線分の距離を調べる ※結果は距離の2乗の値である
	float segment_distance =
		SegmentDistanceSquare(cap0.start_pos, cap0.end_pos, cap1.start_pos, cap1.end_pos);

	// 2つのカプセルの半径の和を求める
	float r = cap0.radius + cap1.radius;

	// 衝突判定の結果を返す
	return segment_distance < r * r;
}

int Math::HitCheck_AABB_Sphere(const AABB& aabb, const Sphere& sph)
{
	// 距離の二乗を取り出す
	float sphLength = 0.0f;
	for (int i = 0; i < 3; i++)
	{
		float sph_center;
		float box_min;
		float box_max;
		if (i == 0)
		{
			sph_center = sph.pos.x;
			box_min = aabb.min.x;
			box_max = aabb.max.x;
		}
		else if (i == 1)
		{
			sph_center = sph.pos.y;
			box_min = aabb.min.y;
			box_max = aabb.max.y;
		}
		else
		{
			sph_center = sph.pos.z;
			box_min = aabb.min.z;
			box_max = aabb.max.z;
		}

		if (sph_center < box_min)
		{
			sphLength += (sph_center - box_min) * (sph_center - box_min);
		}
		if (sph_center > box_max)
		{
			sphLength += (sph_center - box_max) * (sph_center - box_max);
		}
	}

	// 距離の二乗が0だったらCubeの内部にSphereの中心があるということ
	if (sphLength == 0.0f) {
		return 1;
	}

	return sphLength <= sph.r * sph.r;
}

int Math::HitCheck_AABB_Capsule(const AABB& aabb, const Capsule& caps)
{// この実装は球体とボックスの当たり判定を用いたごり押し実装である
	// そのためカプセルの長さによっては少し隙間ができるものもある
	// カプセルの始点位置を獲得
	Vector4 start = caps.start_pos;
	Vector4 normal = VecNormalize4(SubVec4(caps.end_pos, caps.start_pos));
	float dist = DotVec4(SubVec4(caps.end_pos, caps.start_pos), SubVec4(caps.end_pos, caps.start_pos));
	// 距離の値を使って計算を最低限に済ませるべく半径何個分になるかを取得
	// 単位ベクトルの半径倍を動かすことになるができる限り少ない数で球体判定をするべく
	// ゴリ押し実装した
	int count = (int)(dist / caps.radius);
	normal = MulVec4(normal, (float)count);

	for (int i = 0; i < count; i++)
	{
		Sphere sph = Sphere_Set(start.x, start.y, start.z, caps.radius);
		if (HitCheck_AABB_Sphere(aabb, sph))
		{
			return 1;
		}
		start = AddVec4(start, normal);
	}

	Sphere end_sph = Sphere_Set(caps.end_pos.x, caps.end_pos.y, caps.end_pos.z, caps.radius);
	if (HitCheck_AABB_Sphere(aabb, end_sph))
	{
		return 1;
	}
	// 当たっていない
	return 0;
}

Quaternion Math::Quaternion_Set(float x, float y, float z, float w)
{
	Quaternion result{ x,y,z,w };
	return result;
}

Quaternion Math::Quaternion_Set(const Vector4& v, float angle)
{
	float _sin = sin(angle / 2.0f);
	return Quaternion_Set(_sin * v.x, _sin * v.y, _sin * v.z, cos(angle / 2.0f));
}

float Math::Dot_Quaternion(const Quaternion& q1, const Quaternion& q2)
{
	return (q1.w * q2.w) + (q1.x * q2.x) + (q1.y * q2.y) + (q1.z * q2.z);
}

float Math::Length_Quaternion(const Quaternion& q)
{
	return sqrtf(pow(q.w, 2) + pow(q.x, 2) + pow(q.y, 2) + pow(q.z, 2));
}

Quaternion Math::Normalize_Quaternion(const Quaternion& q)
{
	Quaternion result = q;
	float len = Length_Quaternion(result);
	if (len != 0)
	{
		result /= len;
	}
	return result;
}

// 単項演算子 + オーバーロード
// 単項演算子 + は特に意味を持たない
// ( + クォータニオン)
Quaternion Math::operator+(const Quaternion& q) { return q; }

// 単項演算子 + オーバーロード
// ( + クォータニオン)
Quaternion Math::operator - (const Quaternion& q)
{
	Quaternion result = q * -1.0f;
	return result;
}

// 代入演算子 += のオーバーロード
// (クォータニオン += クォータニオン)
Quaternion& Math::operator += (Quaternion& q1, const Quaternion& q2)
{
	q1.w += q2.w;
	q1.x += q2.x;
	q1.y += q2.y;
	q1.z += q2.z;
	return q1;
}
// 代入演算子 -= のオーバーロード
// (クォータニオン -= クォータニオン)
Quaternion& Math::operator -= (Quaternion& q1, const Quaternion& q2)
{
	q1.w -= q2.w;
	q1.x -= q2.x;
	q1.y -= q2.y;
	q1.z -= q2.z;
	return q1;
}
// 代入演算子 *= のオーバーロード
// (クォータニオン *= スカラ―)
Quaternion& Math::operator *= (Quaternion& q, float s)
{
	q.w *= s;
	q.x *= s;
	q.y *= s;
	q.z *= s;

	return q;
}
// 代入演算子 /= のオーバーロード
// (クォータニオン /= スカラ―)
Quaternion& Math::operator /= (Quaternion& q, float s)
{
	return q *= 1.0f / s;
}
// 代入演算子 *= のオーバーロード
// (クォータニオン *= クォータニオン)
Quaternion& Math::operator *= (Quaternion& q1, const Quaternion& q2)
{
	Quaternion result
	{
		q1.w *= q2.w,
		q1.x *= q2.x,
		q1.y *= q2.y,
		q1.z *= q2.z
	};
	q1 = result;
	return q1;
}

// 二項演算子 + オーバーロード
// (クォータニオン + クォータニオン)
Quaternion Math::operator + (const Quaternion& q1, const Quaternion& q2)
{
	Quaternion result = q1;
	return result += q2;
}
// 二項演算子 - オーバーロード
// (クォータニオン - クォータニオン)
Quaternion Math::operator - (const Quaternion& q1, const Quaternion& q2)
{
	Quaternion result = q1;
	return result -= q2;
}
// 二項演算子 * オーバーロード
// (クォータニオン * クォータニオン)
Quaternion Math::operator * (const Quaternion& q1, const Quaternion& q2)
{
	Quaternion result = q1;
	return result *= q2;
}
// 二項演算子 * オーバーロード
// (クォータニオン * スカラ―)
Quaternion Math::operator * (const Quaternion& q, float s)
{
	Quaternion result = q;
	return result *= s;
}
// 二項演算子 * オーバーロード
// (スカラ― * クォータニオン)
Quaternion Math::operator * (float s, const Quaternion& q)
{
	Quaternion result = q;
	return result *= s;
}
// 二項演算子 / オーバーロード
// (クォータニオン / スカラ―)
Quaternion Math::operator / (const Quaternion& q, float s)
{
	Quaternion result = q;
	return result /= s;
}

// ■クォータニオンを球面線形補間する関数
//		q1 → q2 で補間する(t : 0.0 ～ 1.0)
Quaternion Math::slerp(const Quaternion& q1, const Quaternion& q2, float t)
{
	float cos = Dot_Quaternion(q1, q2);
	Quaternion t2 = q2;
	if (cos < 0.0f)
	{
		cos = -cos;
		t2 = -q2;
	}
	float k0 = 1.0f - t;
	float k1 = t;
	if ((1.0f - cos) > 0.001f)
	{
		float theta = (float)acos(cos);
		k0 = (float)(sin(theta * k0) / sin(theta));
		k1 = (float)(sin(theta * k1) / sin(theta));
	}
	return q1 * k0 + t2 * k1;
}
// ■クォータニオンを線形補間する関数
Quaternion Math::lerp(const Quaternion& q1, const Quaternion& q2, float t)
{
	float cos = Dot_Quaternion(q1, q2);
	Quaternion t2 = q2;
	if (cos < 0.0f)
	{
		cos = -cos;
		t2 = -q2;
	}
	float k0 = 1.0f - t;
	float k1 = t;
	return q1 * k0 + t2 * k1;
}

Matrix4x4 Math::rotate(const Quaternion& q)
{
	float xx = q.x * q.x * 2.0f;
	float yy = q.y * q.y * 2.0f;
	float zz = q.z * q.z * 2.0f;
	float xy = q.x * q.y * 2.0f;
	float xz = q.x * q.z * 2.0f;
	float yz = q.y * q.z * 2.0f;
	float wx = q.w * q.x * 2.0f;
	float wy = q.w * q.y * 2.0f;
	float wz = q.w * q.z * 2.0f;

	Matrix4x4 m;
	m.m00 = 1.0f - yy - zz; m.m01 = xy + wz; m.m02 = xz - wy; m.m03 = 0.0f;
	m.m10 = xy - wz; m.m11 = 1.0f - xx - zz; m.m12 = yz + wx; m.m13 = 0.0f;
	m.m20 = xz + wy; m.m21 = yz - wx; m.m22 = 1.0f - xx - yy; m.m23 = 0.0f;
	m.m30 = 0.0f; m.m31 = 0.0f; m.m32 = 0.0f; m.m33 = 1.0f;
	m = Matrix4x4_Array(m);

	return m;
}

Quaternion Math::Quaternion_Set(const Matrix4x4& m)
{
	Quaternion result;
	float tr = m.m00 + m.m11 + m.m22 + m.m33;
	if (tr >= 1.0f)
	{
		float fourD = 2.0f * sqrt(tr);
		result.x = (m.m12 - m.m21) / fourD;
		result.y = (m.m20 - m.m02) / fourD;
		result.z = (m.m01 - m.m10) / fourD;
		result.w = fourD / 4.0f;
		return result;
	}
	int i = 0;
	if (m.m00 <= m.m11) { i = 1; }
	// 以下、未完成部分!!
	if (m.m22 > m.m[i][i]) { i = 2; }

	int j = (i + 1) % 3;
	int k = (j + 1) % 3;
	tr = m.m[i][i] - m.m[j][j] - m.m[k][k] + 1.0f;
	float fourD = 2.0f * sqrt(tr);
	float qa[4];
	qa[i] = fourD / 4.0f;
	qa[j] = (m.m[j][i] + m.m[i][j]) / fourD;
	qa[k] = (m.m[k][i] + m.m[i][k]) / fourD;
	qa[3] = (m.m[j][k] - m.m[k][j]) / fourD;
	result.x = qa[0];
	result.y = qa[1];
	result.z = qa[2];
	result.w = qa[3];
	return Quaternion();
}