#pragma once
#include <DirectXMath.h>
#include "../Struct/Math.h"

class DebugCamera
{
public:
	struct CameraData
	{
		Math::Vector3 position;
		Math::Vector3 lookat;
		float rad;
		float phi = 0.0f;
		float theta = 0.0f;
	};

private:
	Math::Vector3 eye;
	Math::Vector3 target;
	Math::Vector3 up;

	float lookatrange;
	float rotaterad;
	float moverange;

	CameraData data;

	float range;

	float c_fov;//��p(�x���@)(FOV)

	float c_near;//�O�[(near)

	float c_far;//���[(far)

	Math::Vector3 at = Math::Vector3();

	DirectX::XMMATRIX matView; // �r���[�ϊ��s��

	DirectX::XMMATRIX matProjection;			//�ˉe�ϊ��s��

	// �r���{�[�h�s��
	DirectX::XMMATRIX matBillboard;
	// Y�����r���{�[�h�s��
	DirectX::XMMATRIX matBillboardY;

public:
	DebugCamera();
	~DebugCamera();
	// �J����������
	void _Initialize(float lookatrange, float rotaterad, float moverange);
	// �J�����ʒu�X�V
	void _Update();

	// phi�l�̐ݒ�
	void SetPhi(float s_phi);
	void SetAdderPhi(float adder_phi);
	// theta�l�̐ݒ�
	void SetTheta(float s_theta);
	void SetAdderTheta(float adder_theta);
	// lookatrange�̐ݒ�
	void SetLookAtRange(float s_lookat_x, float s_lookat_y, float s_lookat_z);
	void SetAdderLookAt(float adder_lookat_x, float adder_lookat_y, float adder_lookat_z);
	// position�̐ݒ�
	void SetPosition(float s_pos_x, float s_pos_y, float s_pos_z);
	// MoveRange�̐ݒ�
	void SetMoveRange(float move_range);
	// at�̐ݒ�
	void SetAt(float s_at_x,float s_at_y,float s_at_z);
	// ������̃x�N�g��
	void SetUpVector(float x, float y, float z);

	// eye��Get()...x,y,z�͌X�ɍ��
	float GetEye_X()const { return eye.x; }
	float GetEye_Y()const { return eye.y; }
	float GetEye_Z()const { return eye.z; }
	// at��Get()...x,y,z�͌X�ɍ��
	float GetAt_X()const { return at.x; }
	float GetAt_Y()const { return at.y; }
	float GetAt_Z()const { return at.z; }
	// rotated��Get()
	float GetRotaterad();
	// phi��Get()
	float GetPhi()const { return data.phi; }
	// theta��Get()
	float GetTheta()const { return data.theta; }
	// lookAt��Get()...x,y,z�͌X�ɍ��
	float GetLookAtRange_X()const { return data.lookat.x; }
	float GetLookAtRange_Y()const { return data.lookat.y; }
	float GetLookAtRange_Z()const { return data.lookat.z; }
	// position��Get()...x,y,z�͌X�ɍ��
	float GetPos_X()const { return data.position.x; }
	float GetPos_Y()const { return data.position.y; }
	float GetPos_Z()const { return data.position.z; }
	// rightVec��Get()...x,y,z�͌X�ɍ��
	float GetRightVector_X()const;
	float GetRightVector_Y()const;
	float GetRightVector_Z()const;
	// upVec��Get()...x,y,z�͌X�ɍ��
	float GetUpVector_X()const;
	float GetUpVector_Y()const;
	float GetUpVector_Z()const;
	// moverange��Get()
	float GetMoveRange()const { return moverange; }

	// �v���W�F�N�V�����s���Get()
	DirectX::XMMATRIX GetMatProjection()const { return matProjection; }
	// �r���[�s���Get()
	DirectX::XMMATRIX GetMatView()const { return matView; }
	// �r���[�v���W�F�N�V�����s���Get()
	DirectX::XMMATRIX GetMatViewProjection()const { return matView * matProjection; }
	// �r���{�[�h��p�s���Get()
	DirectX::XMMATRIX GetMatBillboard()const { return matBillboard; }
	DirectX::XMMATRIX GetMatYBillboard()const { return matBillboardY; }
};