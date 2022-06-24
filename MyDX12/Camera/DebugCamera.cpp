#include "DebugCamera.h"
#include "../Base/WindowsApp.h"

using namespace DirectX;

namespace
{
	XMFLOAT3 Vector3fromXMFLOAT3(const Math::Vector3& v)
	{
		return XMFLOAT3(v.x, v.y, v.z);
	}

	Math::Vector3 XMMATRIXfromVector3(const XMMATRIX& mat, int ind)
	{
		return Math::Vector3(mat.r[ind].m128_f32[0], mat.r[ind].m128_f32[1], mat.r[ind].m128_f32[2]);
	}
}

// �R���X�g���N�^
DebugCamera::DebugCamera(){}
// �f�X�g���N�^
DebugCamera::~DebugCamera(){}
// ������
void DebugCamera::_Initialize(float lookatrange, float rotaterad, float moverange)
{
	data.phi = Math::ConvertToRadians(-90.0f);

	this->lookatrange = lookatrange;
	this->rotaterad = rotaterad;
	this->moverange = moverange;

	range = 0.0f;

	data.rad = 0.0f;
	data.position = { 0.0f, 0.0f, 0.0f };
	data.lookat = { 0,2.5f,0 };

	// ���_���W
	eye.x = data.position.x;
	eye.y = data.position.y;
	eye.z = data.position.z;
	// �����_���W
	target.x = data.lookat.x;
	target.y = data.lookat.y;
	target.z = data.lookat.z;

	// ������x�N�g��
	up = { 0.0f, 1.0f, 0.0f };

	matView = XMMatrixLookAtLH(
		XMLoadFloat3(&Vector3fromXMFLOAT3(eye)),
		XMLoadFloat3(&Vector3fromXMFLOAT3(target)),
		XMLoadFloat3(&Vector3fromXMFLOAT3(up)));

	c_fov = 60.0f;
	c_near = 0.1f;
	c_far = 1000.0f;

	//�������e
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(c_fov),										//��p60�x(FOV)
		(float)(WindowsApp::window_width / WindowsApp::window_height),	//�A�X�y�N�g��(��ʉ���/��ʏc��)
		c_near,															//�O�[(near)
		c_far);															//���[(far)
}

void DebugCamera::_Update()
{
	// ���_���W
	eye.x = data.position.x;
	eye.y = data.position.y;
	eye.z = data.position.z;
	// �����_���W
	target.x = data.lookat.x;
	target.y = data.lookat.y;
	target.z = data.lookat.z;

	// ������x�N�g��
	up = { 0.0f, 1.0f, 0.0f };

	XMFLOAT3 eyes =    Vector3fromXMFLOAT3(eye);
	XMFLOAT3 targets = Vector3fromXMFLOAT3(target);
	XMFLOAT3 ups =     Vector3fromXMFLOAT3(up);

	matView = XMMatrixLookAtLH(
		XMLoadFloat3(&eyes),
		XMLoadFloat3(&targets),
		XMLoadFloat3(&ups));

	//�������e
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(c_fov),										//��p60�x(FOV)
		(float)(WindowsApp::window_width / WindowsApp::window_height),	//�A�X�y�N�g��(��ʉ���/��ʏc��)
		c_near,															//�O�[(near)
		c_far);															//���[(far)

	// ���_���W
	XMVECTOR eyePosition = XMLoadFloat3(&eyes);
	// �����_���W
	XMVECTOR targetPosition = XMLoadFloat3(&targets);
	// (����) �����
	XMVECTOR upVector = XMLoadFloat3(&ups);

	// �J����Z�� (��������)
	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);

	// 0�x�N�g�����ƌ�������܂�Ȃ��̂ŏ��O
	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));

	// �x�N�g���𐳋K��
	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	// �J������X�� (�E����)
	XMVECTOR cameraAxisX;
	// X���͏������Z���̊O�ςŋ��܂�
	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
	// �x�N�g���𐳋K��
	cameraAxisX = XMVector3Normalize(cameraAxisX);

	// �J������Y�� (�����)
	XMVECTOR cameraAxisY;
	// Y����Z����X���̊O�ςŋ��܂�
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);
	// �x�N�g���𐳋K��
	cameraAxisY = XMVector3Normalize(cameraAxisY);

	// �J������]�s��
	XMMATRIX matCameraRot;
	// �J�������W�n�����[���h���W�n�̕ϊ��s��
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);

	// �]�u�ɂ��t�s�� (�t��]) ���v�Z
	matView = XMMatrixTranspose(matCameraRot);

	// ���_���W��-1���|�������W
	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	// �J�����̈ʒu���烏�[���h���_�ւ̃x�N�g��(�J�������W�n)
	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition); // X����
	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition); // Y����
	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition); // Z����
	// ��̃x�N�g���ɂ܂Ƃ߂�
	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);

	// �r���[�s��ɕ��s�ړ��s�񐬕���ݒ�
	matView.r[3] = translation;

#pragma region �S�����r���{�[�h�s��̌v�Z
	// �r���{�[�h�s��
	matBillboard.r[0] = cameraAxisX;
	matBillboard.r[1] = cameraAxisY;
	matBillboard.r[2] = cameraAxisZ;
	matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);
#pragma region

#pragma region Y�����r���{�[�h�s��̌v�Z
	// �J����X��,Y��,Z��
	XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;

	// X���͋���
	ybillCameraAxisX = cameraAxisX;
	// Y���̓��[���h���W�n��Y��
	ybillCameraAxisY = XMVector3Normalize(upVector);
	// Z����X����Y���̊O�ςŋ��܂�
	ybillCameraAxisZ = XMVector3Cross(ybillCameraAxisX, ybillCameraAxisY);

	// Y�����r���{�[�h�s��
	matBillboardY.r[0] = ybillCameraAxisX;
	matBillboardY.r[1] = ybillCameraAxisY;
	matBillboardY.r[2] = ybillCameraAxisZ;
	matBillboardY.r[3] = XMVectorSet(0, 0, 0, 1);
#pragma region
}

void DebugCamera::SetPhi(float s_phi)
{
	data.phi = s_phi;
}

void DebugCamera::SetAdderPhi(float adder_phi)
{
	data.phi += adder_phi;
}

void DebugCamera::SetTheta(float s_theta)
{
	data.theta = s_theta;
}

void DebugCamera::SetAdderTheta(float adder_theta)
{
	data.theta += adder_theta;
}

void DebugCamera::SetLookAtRange(float s_lookat_x,float s_lookat_y, float s_lookat_z)
{
	data.lookat = Math::Vector3(s_lookat_x,s_lookat_y,s_lookat_z);
}

void DebugCamera::SetAdderLookAt(float adder_lookat_x, float adder_lookat_y, float adder_lookat_z)
{
	data.lookat += Math::Vector3(adder_lookat_x, adder_lookat_y, adder_lookat_z);
}

void DebugCamera::SetPosition(float s_pos_x, float s_pos_y, float s_pos_z)
{
	data.position = Math::Vector3(s_pos_x, s_pos_y, s_pos_z);
}

void DebugCamera::SetMoveRange(float move_range)
{
	moverange = move_range;
}

void DebugCamera::SetAt(float s_at_x, float s_at_y, float s_at_z)
{
	at = Math::Vector3(s_at_x, s_at_y, s_at_z);
}

void DebugCamera::SetUpVector(float x, float y, float z)
{
	up = { x,y,z };
}

float DebugCamera::GetRotaterad()
{
	return rotaterad;
}

float DebugCamera::GetRightVector_X() const
{
	return Math::Vector3(XMMATRIXfromVector3(matView, 0)).x;
}

float DebugCamera::GetRightVector_Y() const
{
	return Math::Vector3(XMMATRIXfromVector3(matView, 0)).y;
}

float DebugCamera::GetRightVector_Z() const
{
	return Math::Vector3(XMMATRIXfromVector3(matView, 0)).z;
}

float DebugCamera::GetUpVector_X() const
{
	return Math::Vector3(XMMATRIXfromVector3(matView, 1)).x;
}

float DebugCamera::GetUpVector_Y() const
{
	return Math::Vector3(XMMATRIXfromVector3(matView, 1)).y;
}

float DebugCamera::GetUpVector_Z() const
{
	return Math::Vector3(XMMATRIXfromVector3(matView, 1)).z;
}
