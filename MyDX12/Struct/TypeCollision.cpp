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
	// �n�_�x�N�g����ݒ�
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
	// �@���𐳋K�����Ă���
	Vector4 n = VecNormalize4(normal);

	// ���ʂ�a,b,c��ݒ�
	Plane r;
	r.a = n.x;
	r.b = n.y;
	r.c = n.z;

	// ���ʂ�d��ݒ�
	r.d = -((n.x * p.x) + (n.y * p.y) + (n.z * p.z));
	return r;
}

Plane Math::Plane_Set(const Vector4& p0, const Vector4& p1, const Vector4& p2)
{
	// �@�������߂�
	Vector4 n = CrossVec4(p0, p1, p2);

	// �ʊ֐��ŏ���
	return (Plane_Set(n, p0));
}

Triangle Math::Triangle_Set(const Vector4& p0, const Vector4& p1, const Vector4& p2)
{
	// 3�_�𗘗p�������ʂ����߂�
	Plane plane = Plane_Set(p0, p1, p2);

	// �O�p�`�̊e�ݒ�
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
	// �����̒l�Ŕ��f
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
	// X�������̃x�N�g����0���H
	if ((s0.vec.x == 0.0f) || (s0.vec.y == 0.0f))
	{
		// X�������̃x�N�g����0�̂Ƃ��͌X�����v�Z�ł��Ȃ��̂œ���
		if ((s0.vec.x == 0.0f) && (s0.vec.y == 0.0f))return 0;

		Vector2 r;
		float t0, t1;
		if (s0.vec.x == 0.0f)
		{
			r.x = s0.pos.x;
			r.y = (s1.vec.y / s1.vec.x) * (r.x - s1.pos.x) + s1.pos.y;

			t0 = (r.y - s0.pos.y) / s0.vec.y; // t = 0~1�̂Ƃ��͐���s0��
			t1 = (r.x - s1.pos.x) / s1.vec.x; // t = 0~1�̂Ƃ��͐���s1��
		}
		else
		{
			r.x = s1.pos.x;
			r.y = (s0.vec.y / s0.vec.x) * (r.x - s0.pos.x) + s0.pos.y;

			t0 = (r.x - s0.pos.x) / s0.vec.x; // t = 0~1�̂Ƃ��͐���s0��
			t1 = (r.y - s1.pos.y) / s1.vec.y; // t = 0~1�̂Ƃ��͐���s1��
		}
		if ((t0 < 0.0f) || (t0 > 1.0f) || (t1 < 0.0f) || (t1 > 1.0f))return 0;
		if (hit_pos) *hit_pos = r;
		return 1;
	}
	else
	{
		// �����̌X�������߂�
		float a0 = s0.vec.y / s0.vec.x;
		float a1 = s1.vec.y / s1.vec.x;

		// �X��������̏ꍇ�͕��s�Ȃ̂ŏՓ˂��Ȃ�
		if ((a0 == a1) || (a0 == -a1))return 0;

		// ��_��x,y���W�����߂�
		Vector2 r;
		r.x = (a0 * s0.pos.x - a1 * s1.pos.x + s1.pos.y - s0.pos.y) / (a0 - a1);
		r.y = a0 * (r.x - s0.pos.x) + s0.pos.y;

		// ��_���������ɂ��邩���ׂ�
		float t0 = (r.x - s0.pos.x) / s0.vec.x;
		float t1 = (r.x - s1.pos.x) / s1.vec.x;
		if ((t0 < 0.0f) || (t0 > 1.0f) || (t1 < 0.0f) || (t1 > 1.0f))return 0;

		// �Փ˂��Ă��邱�Ƃ�Ԃ�
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
	// ���̔��ʎ��̓��C�������O��Ă��邱�ƂɈ�v
	if (discr < 0.0f)return 0;

	// ���C�͋��ƌ������Ă���B
	// ��������ŏ��lt���v�Z
	float t = -b - sqrtf(discr);

	// t�����ł���ꍇ�A���C�͋��̓�������J�n���Ă���̂�t���[���ɃN�����v
	if (t < 0)t = 0.0f;
	if (distance) { *distance = t; }

	if (hit_pos) *hit_pos = AddVec4(seg.pos, MulVec4(seg.vec, t));

	return 1;
}

int Math::HitCheck_Plane_Segment(const Plane& pln, const Segment& seg, float* distance, Vector4* hit_pos)
{
	const float epsilon = 1.0e-5f;		// �덷�z���p�����Ȓl
	// �ʖ@���ƃ��C�̕����x�N�g���̓���
	Vector4 plane_normal = { pln.a,pln.b,pln.c,pln.d };
	float d1 = DotVec4(plane_normal, seg.vec);
	// ���ʂɂ͓�����Ȃ�
	if (d1 > -epsilon) { return 0; }
	// ���_�ƌ��_�̋��� (���ʂ̖@������)
	// �ʖ@���ƃ��C�̎n�_���W(�ʒu�x�N�g��)�̓���
	float d2 = DotVec4(plane_normal, seg.pos);
	// �n�_�ƕ��ʂ̋����i���ʂ̖@�������j
	float dist = d2 - pln.distance;
	// �n�_�ƕ��ʂ̋����i���C�����j
	float t = dist / -d1;
	// ��_���n�_�����ɂ���̂ŁA������Ȃ�
	if (t < 0)return 0;
	// ��������������
	if (distance) { *distance = t; }
	// ��_���v�Z
	if (hit_pos) { *hit_pos = AddVec4(seg.pos, MulVec4(seg.vec, t)); }
	return 1;
}

int Math::Triangle_CheckInner(const Triangle& tri, const Vector4& p)
{
	// �C�ӓ_����e���_�ւ̃x�N�g���쐬
	Vector4 vt0 = SubVec4(tri.pos[0], p);
	Vector4 vt1 = SubVec4(tri.pos[1], p);
	Vector4 vt2 = SubVec4(tri.pos[2], p);

	// �O�p�`�̕ӂ��Ȃ���x�N�g���쐬
	Vector4 v0 = SubVec4(tri.pos[1], tri.pos[0]);
	Vector4 v1 = SubVec4(tri.pos[2], tri.pos[1]);
	Vector4 v2 = SubVec4(tri.pos[0], tri.pos[2]);

	// �e�O�ς����߂�
	Vector4 c0 = CrossVec4(vt0, v0);
	Vector4 c1 = CrossVec4(vt1, v1);
	Vector4 c2 = CrossVec4(vt2, v2);

	// �O�ς̌����������Ă��邩�`�F�b�N(���ϗ��p)
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

	// �O�p�`���ڂ��Ă��镽�ʂ��Z�o
	Plane plane;
	Vector4 interPlane;

	plane = tri.plane;
	plane.distance = DotVec4({ plane.a,plane.b ,plane.c ,plane.d }, tri.pos[0]);
	// ���C�ƕ��ʂ��������Ă��Ȃ���΁A�������Ă��Ȃ�
	if (!HitCheck_Plane_Segment(plane, seg, distance, &interPlane)) return 0;
	// ���C�ƕ��ʂ��������Ă����̂ŁA�����ƌ�_���������܂ꂽ
	// ���C�ƕ��ʂ̌�_���O�p�`�̓����ɂ��邩����
	const float epsilon = 1.0e-5f;		// �덷�z���p�����Ȓl
	Vector4 m;
	// ��p0_p1�ɂ���
	Vector4 pt_p0 = SubVec4(tri.pos[0] ,interPlane);
	Vector4 p0_p1 = SubVec4(tri.pos[1],tri.pos[0]);
	m = CrossVec4(pt_p0, p0_p1);
	// �ӂ̊O���ł���Γ������ĂȂ��̂Ŕ����ł��؂�
	if (DotVec4(m, { tri.plane.a,tri.plane.b ,tri.plane.c ,tri.plane.d }) < -epsilon)return 0;
	// ��p1_p2�ɂ���
	Vector4 pt_p1 = SubVec4(tri.pos[1], interPlane);
	Vector4 p1_p2 = SubVec4(tri.pos[2], tri.pos[1]);
	m = CrossVec4(pt_p1, p1_p2);
	// �ӂ̊O���ł���Γ������ĂȂ��̂Ŕ����ł��؂�
	if (DotVec4(m, { tri.plane.a,tri.plane.b ,tri.plane.c ,tri.plane.d }) < -epsilon)return 0;
	// ��p2_p0�ɂ���
	Vector4 pt_p2 = SubVec4(tri.pos[2], interPlane);
	Vector4 p2_p0 = SubVec4(tri.pos[0], tri.pos[2]);
	m = CrossVec4(pt_p2, p2_p0);
	// �ӂ̊O���ł���Γ������ĂȂ��̂Ŕ����ł��؂�
	if (DotVec4(m, { tri.plane.a,tri.plane.b ,tri.plane.c ,tri.plane.d }) < -epsilon)return 0;
	// �����Ȃ̂ŁA�������Ă���
	if (hit_pos)
	{
		*hit_pos = interPlane;
	}

	return 1;
}

int Math::HitCheck_Sphere_Sphere( const Sphere& sph0, const Sphere& sph1, Vector4* hit_vec, Vector4* reject)
{
	// ���S�Ԃ̋�����2����l��
	Vector4 vec = SubVec4(sph0.pos, sph1.pos);
	float l2 = vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
	float radius2 = sph0.r + sph1.r;
	radius2 *= radius2;

	// ������2��̔�r�œ͂����ǂ������ׂ�
	if (l2 > radius2)return 0;

	// �Փ˂��Ă���̂ł߂荞�݂Ɋւ�������擾
	if (hit_vec)
	{
		// A�̔��a��0�̎����W��B�̒��S�@B�̔��a��0�̎����W��A�̒��S�@�ƂȂ�悤�⊮
		float t = sph1.r / (sph0.r + sph1.r);
		*hit_vec = VectorLerp(sph0.pos, sph1.pos, t);
	}
	if (reject)
	{
		float l = (sph0.r + sph1.r) - sqrtf(l2);	// �߂荞�ݗ�
		*reject = VecNormalize4(vec);
		*reject = MulVec4(*reject, l);
	}

	// �Փ˂��Ă��邱�Ƃ�Ԃ�
	return 1;
}

int Math::HitCheck_Plane_Sphere(Vector4* hit_vec, const Plane& pln, const Sphere& sph)
{
	// ���ʂƋ��̒��S�̋����Ŕ��f
	float l = Plane_Length(pln, sph.pos);
	if (l > sph.r)return 0;

	// �ǂꂾ���߂荞��ł��邩�𒲂ׂ�
	if (hit_vec)
	{
		l = sph.r - l;	// �߂荞�ݗ�
		*hit_vec = SetVector4(pln.a * l, pln.b * l, pln.c * l);
		// ���ʂ̖@���𗘗p����
	}

	// �Փ˂��Ă��邱�Ƃ�Ԃ�
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
		// p0���ŋߖT
		*closest = triangle.pos[0];
		return;
	}

	// point��p1�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	Vector4 p1_pt = SubVec4(point, triangle.pos[1]);

	float d3 = DotVec4(p0_p1, p1_pt);
	float d4 = DotVec4(p0_p2, p1_pt);

	if (d3 >= 0.0f && d4 <= d3)
	{
		// p1���ŋߖT
		*closest = triangle.pos[1];
		return;
	}

	// point��p0_p1�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p1��ɑ΂���ˉe��Ԃ�
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		*closest = AddVec4(triangle.pos[0],MulVec4(p0_p1,v));
		return;
	}

	// point��p2�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	Vector4 p2_pt = SubVec4(point, triangle.pos[2]);

	float d5 = DotVec4(p0_p1, p2_pt);
	float d6 = DotVec4(p0_p2, p2_pt);
	if (d6 >= 0.0f && d5 <= d6)
	{
		*closest = triangle.pos[2];
		return;
	}

	// point��p0_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p2��ɑ΂���ˉe��Ԃ�
	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		*closest = AddVec4(triangle.pos[0], MulVec4(p0_p2, w));
		return;
	}

	// point��p1_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p1_p2��ɑ΂���ˉe��Ԃ�
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
	// �O�p�`���܂ޕ��ʂƋ��ƂŃ`�F�b�N
	if (!HitCheck_Plane_Sphere(NULL, tri.plane, sph))return 0;

	Vector4 p;
	ClosestPtPoint_Triangle(sph.pos, tri, &p);
	// �_p�Ƌ��̒��S�̍����x�N�g��
	Vector4 v = SubVec4(p, sph.pos);
	// �����̓������߂�
	//�i�����x�N�g�����m�̓��ς͎O�����̒藝�̃��[�g�����̎��ƈ�v����j
	float distanceSquare = DotVec4(v, v);
	// ���ƎO�p�`�̋��������a�ȉ��Ȃ瓖�����Ă��Ȃ�
	if (distanceSquare > sph.r * sph.r)	return 0;
	// �[����_���v�Z
	if (hit_vec) {
		// �O�p�`��̍ŋߐړ_p���^����_�Ƃ���
		*hit_vec = p;
	}
	// �����o���x�N�g�����v�Z
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
	// ���ʂ̖@���𗘗p���A���ʂ����s�ɂȂ��Ă��邩�`�F�b�N
	float dot = (pln0.a * pln1.a) + (pln0.b * pln1.b) + (pln0.c * pln1.c);
	if ((dot >= 1.0f) || (dot <= -1.0f))return 0;

	// ����������K�v�ɉ����č쐬
	if (cross_seg)
	{
		// �O�ς𗘗p���Č�����������x�N�g�����쐬
		Vector4 vec = CrossVec4(SetVector4(pln0.a, pln0.b, pln0.c),
			SetVector4(pln1.a, pln1.b, pln1.c));

		// ���������̎n�_���Z�o
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

		// �����̐ݒ�
		*cross_seg = Segment_Set(pos.x, pos.y, pos.z, vec.x, vec.y, vec.z);
	}

	// �Փ˂��Ă��邱�Ƃ�Ԃ�
	return 1;
}

int Math::HitCheck_Plane_Triangle(const Plane& pln, const Triangle& tri)
{
	// �O�p�`��1�_�ڂ����ʂɑ΂��ĕ\�����̂ǂ��瑤�ɑ��݂��Ă��邩���`�F�b�N
	int side0 = Plane_Side(pln, tri.pos[0]);

	// �O�p�`��2�_�ڂ����ʂɑ΂��ĕ\�����̂ǂ��瑤�ɑ��݂��Ă��邩���`�F�b�N���A
	// �Փ˂𔻒肷��
	int side1 = Plane_Side(pln, tri.pos[1]);
	if ((side0 && !side1) || (!side0 && side1)) return 1;
	// �\���ɑ��݂���̂ŏՓ�

	// �O�p�`��3�_�ڂ����ʂɑ΂��ĕ\�����̂ǂ��瑤�ɑ��݂��Ă��邩���`�F�b�N���A
	// �Փ˂𔻒肷��
	int side2 = Plane_Side(pln, tri.pos[2]);
	if ((side1 && !side2) || (!side1 && side2)) return 1;
	// �\���ɑ��݂���̂ŏՓ�

	// �Փ˂��Ă��Ȃ����Ƃ�Ԃ�
	return 0;
}

int Math::HitCheck_Triangle_Triangle(const Triangle& tri0, const Triangle& tri1)
{
	// �Е��̎O�p�`��2�ӂ������Е��̎O�p�`���т��Ă��邩�H
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

	// �O�p�`�����ւ���2�ӂ̃`�F�b�N���s��
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

	// ���݂���1�ӂ��ђʂ��Ă��邩�ǂ������ׂ�
	if ((hit0_0 || hit0_1 || hit0_2) && (hit1_0 || hit1_1 || hit1_2))return 1;

	// �Փ˂��Ă��Ȃ����Ƃ�Ԃ�
	return 0;
}

int Math::BoundingCheck_BoundingSphere_BoundingSphere(const BoundingSphere& bs0, const BoundingSphere& bs1)
{
	// ���S�Ԃ̋�����2����l��
	Vector4 vec = SubVec4(bs1.pos, bs0.pos);
	float l2 = vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;

	// ������2��̔�r�œ͂����ǂ������ׂ�
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
	// ���ʊi�[�p
	float t_min, t_max;

	// X���̈�ł̔���
	float tx_min, tx_max;
	if ((seg.vec.x == 0.0f))
	{
		// �����x�N�g����0�̂Ƃ��͓���
		if ((seg.pos.x < aabb.min.x) || (seg.pos.x > aabb.max.x))
			return 0;
		tx_min = 0.0f;
		tx_max = 1.0f;
	}
	else
	{
		// X���ł̗̈������
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
		// X���̈���ł��łɔ͈͊O
	}
	t_min = tx_min;
	t_max = tx_max;

	// Y���̈�ł̔���
	float ty_min, ty_max;
	if ((seg.vec.y == 0.0f))
	{
		// �����x�N�g����0�̂Ƃ��͓���
		if ((seg.pos.y < aabb.min.y) || (seg.pos.y > aabb.max.y))
			return 0;
		ty_min = 0.0f;
		ty_max = 1.0f;
	}
	else
	{
		// Y���ł̗̈������
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
		// Y���̈���ł��łɔ͈͊O
	}
	if ((t_max < ty_min) || (t_min > ty_max)) return 0;
	// X����Y���̗̈悪���Ԃ��ĂȂ�!
	if (t_min < ty_min)t_min = ty_min;
	if (t_max > ty_max)t_max = ty_max;

	// Z���̈�ł̔���
	float tz_min, tz_max;
	if ((seg.vec.z == 0.0f))
	{
		// �����x�N�g����0�̂Ƃ��͓���
		if ((seg.pos.z < aabb.min.z) || (seg.pos.z > aabb.max.z))
			return 0;
		tz_min = 0.0f;
		tz_max = 1.0f;
	}
	else
	{
		// Z���ł̗̈������
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
		// Z���̈���ł��łɔ͈͊O
	}
	if ((t_max < tz_min) || (t_min > tz_max)) return 0;
	// X,Y����Z���̗̈悪���Ԃ��ĂȂ�!
	if (t_min < tz_min)t_min = tz_min;
	if (t_max > tz_max)t_max = tz_max;

	// ���ʗ̈�̃`�F�b�N
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
	// ��(�f���^) = 0�̂Ƃ��͏����ł��Ȃ��̂ł��̂܂ܕԂ�
	if (det == 0.0f)return m;
	det = 1.0f / det;		// �\�ߋt���ɂ��Ă���

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
	// ��(�f���^) = 0�̂Ƃ��͏����ł��Ȃ��̂ł��̂܂ܕԂ�
	if (det == 0.0f)return m;
	det = 1.0f / det;		// �\�ߋt���ɂ��Ă���

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
	// �s��̍쐬
	Matrix4x4 r;
	r = _Scale(scale.x, scale.y, scale.z);
	r = _MulMatrix4x4(_RotationZ(rot.z), r);			// Z����]
	r = _MulMatrix4x4(_RotationX(rot.x), r);			// X����]
	r = _MulMatrix4x4(_RotationY(rot.y), r);			// Y����]
	r = _MulMatrix4x4(_Translate(pos.x, pos.y, pos.z), r);// ���s�ړ�

	// OBB�̍s��Ƃ��Đݒ�
	OBB obb;
	obb.m[0] = r;
	obb.m[1] = _InverseMatrix(r); // �t�s��i�[
	return obb;
}

int Math::BoundingCheck_OBB_Segment(const OBB& obb, const Segment seg)
{
	// �����ɑ΂�OBB�̋t�s���K�p������
	Segment segment;
	segment.pos = Matrix4x4_x_Vector4(obb.m[1], seg.pos);
	segment.vec = Matrix4x4_x_Vector4(obb.m[1], seg.vec);

	// ���_�ɂ���T�C�Y1������AABB�ƏՓ˔���
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
	// �J�v�Z�����̎n�_����I�_�܂ł̃x�N�g�����l��
	Vector4 se_v = SubVec4(caps.end_pos, caps.start_pos);
	// �擾�����x�N�g����P�ʉ�
	Vector4 n = VecNormalize4(se_v);

	// ������J�v�Z���܂ł̃x�N�g��
	Vector4 sph_caps = SubVec4(sph.pos, caps.start_pos);
	// ��_�̓��ς����
	float t = DotVec4(sph_caps, n);

	Vector4 vPsPn = MulVec4(n, t);
	Vector4 posPn = AddVec4(caps.start_pos, vPsPn);

	// 5. �䗦�@t / (Ps -> Pe�̒���)�����߂�
	float lengthRate = t / VecLength4(se_v);

	// 6. lengthRate < 0 , 0 <= lengthRate <= 1, 1 < lengthRate �ŏꍇ����
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
	Vector4 d1 = SubVec4(q1, p1);	// p1 -> q1 �̃x�N�g��
	Vector4 d2 = SubVec4(q2, p2);	// p2 -> q2 �̃x�N�g��

	Vector4 r = SubVec4(p1, p2);//(P1 - P2) -> r

	float a = DotVec4(d1, d1);			//  a = d1�Ed1
	float b = DotVec4(d1, d2);			//  b = d1�Ed2
	float e = DotVec4(d2, d2);			//  e = d2�Ed2

	float c = DotVec4(d1, r);			//  c = d1�Er
	float f = DotVec4(d2, r);			//  f = d2�Er

	float s = 0.0f;
	float t = 0.0f;

	float denominator = a * e - (b * b);			// ����

	// s �̒l�����߂�
	if (denominator != 0.0f)
	{
		s = (b * f - c * e) / denominator;
		s = clamp(s, 0.0f, 1.0f);
	}
	else
	{
		s = 0.0f;
	}

	// t �̒l�����߂�
	// t = ((p1 + d1 * s) - p2)�Ed2 / (d2 �E d2)
	t = (c + a * s) / b;

	// t ��[0.0,1.0]�͈̔͊O�ł���΁As ���Čv�Z����
	// s = ((p2 + d2 * t) - p1)�Ed1 / (d1�Ed1) = (t * b - c) / a
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
	// s �� t �̒l�����肵���̂ŁA�e�������̍��W c1 �� c2 �����߂�
	Vector4 c1 = AddVec4(p1, MulVec4(d1, s));
	Vector4 c2 = AddVec4(p2, MulVec4(d2, t));

	// 2�_��(c1 �� c2)�̋�����2������߂āA���ʂ�Ԃ�
	Vector4 v = SubVec4(c1, c2);
	return DotVec4(v,v);
}

int Math::HitCheck_Capsule_Capsule(const Capsule& cap0, const Capsule& cap1)
{
	// �����Ɛ����̋����𒲂ׂ� �����ʂ͋�����2��̒l�ł���
	float segment_distance =
		SegmentDistanceSquare(cap0.start_pos, cap0.end_pos, cap1.start_pos, cap1.end_pos);

	// 2�̃J�v�Z���̔��a�̘a�����߂�
	float r = cap0.radius + cap1.radius;

	// �Փ˔���̌��ʂ�Ԃ�
	return segment_distance < r * r;
}

int Math::HitCheck_AABB_Sphere(const AABB& aabb, const Sphere& sph)
{
	// �����̓������o��
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

	// �����̓�悪0��������Cube�̓�����Sphere�̒��S������Ƃ�������
	if (sphLength == 0.0f) {
		return 1;
	}

	return sphLength <= sph.r * sph.r;
}

int Math::HitCheck_AABB_Capsule(const AABB& aabb, const Capsule& caps)
{// ���̎����͋��̂ƃ{�b�N�X�̓����蔻���p�������艟�������ł���
	// ���̂��߃J�v�Z���̒����ɂ���Ă͏������Ԃ��ł�����̂�����
	// �J�v�Z���̎n�_�ʒu���l��
	Vector4 start = caps.start_pos;
	Vector4 normal = VecNormalize4(SubVec4(caps.end_pos, caps.start_pos));
	float dist = DotVec4(SubVec4(caps.end_pos, caps.start_pos), SubVec4(caps.end_pos, caps.start_pos));
	// �����̒l���g���Čv�Z���Œ���ɍς܂���ׂ����a�����ɂȂ邩���擾
	// �P�ʃx�N�g���̔��a�{�𓮂������ƂɂȂ邪�ł�����菭�Ȃ����ŋ��̔��������ׂ�
	// �S��������������
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
	// �������Ă��Ȃ�
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

// �P�����Z�q + �I�[�o�[���[�h
// �P�����Z�q + �͓��ɈӖ��������Ȃ�
// ( + �N�H�[�^�j�I��)
Quaternion Math::operator+(const Quaternion& q) { return q; }

// �P�����Z�q + �I�[�o�[���[�h
// ( + �N�H�[�^�j�I��)
Quaternion Math::operator - (const Quaternion& q)
{
	Quaternion result = q * -1.0f;
	return result;
}

// ������Z�q += �̃I�[�o�[���[�h
// (�N�H�[�^�j�I�� += �N�H�[�^�j�I��)
Quaternion& Math::operator += (Quaternion& q1, const Quaternion& q2)
{
	q1.w += q2.w;
	q1.x += q2.x;
	q1.y += q2.y;
	q1.z += q2.z;
	return q1;
}
// ������Z�q -= �̃I�[�o�[���[�h
// (�N�H�[�^�j�I�� -= �N�H�[�^�j�I��)
Quaternion& Math::operator -= (Quaternion& q1, const Quaternion& q2)
{
	q1.w -= q2.w;
	q1.x -= q2.x;
	q1.y -= q2.y;
	q1.z -= q2.z;
	return q1;
}
// ������Z�q *= �̃I�[�o�[���[�h
// (�N�H�[�^�j�I�� *= �X�J���\)
Quaternion& Math::operator *= (Quaternion& q, float s)
{
	q.w *= s;
	q.x *= s;
	q.y *= s;
	q.z *= s;

	return q;
}
// ������Z�q /= �̃I�[�o�[���[�h
// (�N�H�[�^�j�I�� /= �X�J���\)
Quaternion& Math::operator /= (Quaternion& q, float s)
{
	return q *= 1.0f / s;
}
// ������Z�q *= �̃I�[�o�[���[�h
// (�N�H�[�^�j�I�� *= �N�H�[�^�j�I��)
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

// �񍀉��Z�q + �I�[�o�[���[�h
// (�N�H�[�^�j�I�� + �N�H�[�^�j�I��)
Quaternion Math::operator + (const Quaternion& q1, const Quaternion& q2)
{
	Quaternion result = q1;
	return result += q2;
}
// �񍀉��Z�q - �I�[�o�[���[�h
// (�N�H�[�^�j�I�� - �N�H�[�^�j�I��)
Quaternion Math::operator - (const Quaternion& q1, const Quaternion& q2)
{
	Quaternion result = q1;
	return result -= q2;
}
// �񍀉��Z�q * �I�[�o�[���[�h
// (�N�H�[�^�j�I�� * �N�H�[�^�j�I��)
Quaternion Math::operator * (const Quaternion& q1, const Quaternion& q2)
{
	Quaternion result = q1;
	return result *= q2;
}
// �񍀉��Z�q * �I�[�o�[���[�h
// (�N�H�[�^�j�I�� * �X�J���\)
Quaternion Math::operator * (const Quaternion& q, float s)
{
	Quaternion result = q;
	return result *= s;
}
// �񍀉��Z�q * �I�[�o�[���[�h
// (�X�J���\ * �N�H�[�^�j�I��)
Quaternion Math::operator * (float s, const Quaternion& q)
{
	Quaternion result = q;
	return result *= s;
}
// �񍀉��Z�q / �I�[�o�[���[�h
// (�N�H�[�^�j�I�� / �X�J���\)
Quaternion Math::operator / (const Quaternion& q, float s)
{
	Quaternion result = q;
	return result /= s;
}

// ���N�H�[�^�j�I�������ʐ��`��Ԃ���֐�
//		q1 �� q2 �ŕ�Ԃ���(t : 0.0 �` 1.0)
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
// ���N�H�[�^�j�I������`��Ԃ���֐�
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
	// �ȉ��A����������!!
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