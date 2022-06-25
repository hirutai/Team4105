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

// コンストラクタ
DebugCamera::DebugCamera(){}
// デストラクタ
DebugCamera::~DebugCamera(){}
// 初期化
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

	// 視点座標
	eye.x = data.position.x;
	eye.y = data.position.y;
	eye.z = data.position.z;
	// 注視点座標
	target.x = data.lookat.x;
	target.y = data.lookat.y;
	target.z = data.lookat.z;

	// 上方向ベクトル
	up = { 0.0f, 1.0f, 0.0f };

	matView = XMMatrixLookAtLH(
		XMLoadFloat3(&Vector3fromXMFLOAT3(eye)),
		XMLoadFloat3(&Vector3fromXMFLOAT3(target)),
		XMLoadFloat3(&Vector3fromXMFLOAT3(up)));

	c_fov = 60.0f;
	c_near = 0.1f;
	c_far = 1000.0f;

	//透視投影
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(c_fov),										//画角60度(FOV)
		(float)(WindowsApp::window_width / WindowsApp::window_height),	//アスペクト比(画面横幅/画面縦幅)
		c_near,															//前端(near)
		c_far);															//奥端(far)
}

void DebugCamera::_Update()
{
	// 視点座標
	eye.x = data.position.x;
	eye.y = data.position.y;
	eye.z = data.position.z;
	// 注視点座標
	target.x = data.lookat.x;
	target.y = data.lookat.y;
	target.z = data.lookat.z;

	// 上方向ベクトル
	up = { 0.0f, 1.0f, 0.0f };

	XMFLOAT3 eyes =    Vector3fromXMFLOAT3(eye);
	XMFLOAT3 targets = Vector3fromXMFLOAT3(target);
	XMFLOAT3 ups =     Vector3fromXMFLOAT3(up);

	matView = XMMatrixLookAtLH(
		XMLoadFloat3(&eyes),
		XMLoadFloat3(&targets),
		XMLoadFloat3(&ups));

	//透視投影
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(c_fov),										//画角60度(FOV)
		(float)(WindowsApp::window_width / WindowsApp::window_height),	//アスペクト比(画面横幅/画面縦幅)
		c_near,															//前端(near)
		c_far);															//奥端(far)

	// 視点座標
	XMVECTOR eyePosition = XMLoadFloat3(&eyes);
	// 注視点座標
	XMVECTOR targetPosition = XMLoadFloat3(&targets);
	// (仮の) 上方向
	XMVECTOR upVector = XMLoadFloat3(&ups);

	// カメラZ軸 (視線方向)
	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);

	// 0ベクトルだと向きが定まらないので除外
	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));

	// ベクトルを正規化
	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	// カメラのX軸 (右方向)
	XMVECTOR cameraAxisX;
	// X軸は上方向→Z軸の外積で求まる
	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
	// ベクトルを正規化
	cameraAxisX = XMVector3Normalize(cameraAxisX);

	// カメラのY軸 (上方向)
	XMVECTOR cameraAxisY;
	// Y軸はZ軸→X軸の外積で求まる
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);
	// ベクトルを正規化
	cameraAxisY = XMVector3Normalize(cameraAxisY);

	// カメラ回転行列
	XMMATRIX matCameraRot;
	// カメラ座標系→ワールド座標系の変換行列
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);

	// 転置により逆行列 (逆回転) を計算
	matView = XMMatrixTranspose(matCameraRot);

	// 視点座標に-1を掛けた座標
	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	// カメラの位置からワールド原点へのベクトル(カメラ座標系)
	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition); // X成分
	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition); // Y成分
	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition); // Z成分
	// 一つのベクトルにまとめる
	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);

	// ビュー行列に平行移動行列成分を設定
	matView.r[3] = translation;

#pragma region 全方向ビルボード行列の計算
	// ビルボード行列
	matBillboard.r[0] = cameraAxisX;
	matBillboard.r[1] = cameraAxisY;
	matBillboard.r[2] = cameraAxisZ;
	matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);
#pragma region

#pragma region Y軸回りビルボード行列の計算
	// カメラX軸,Y軸,Z軸
	XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;

	// X軸は共通
	ybillCameraAxisX = cameraAxisX;
	// Y軸はワールド座標系のY軸
	ybillCameraAxisY = XMVector3Normalize(upVector);
	// Z軸はX軸→Y軸の外積で求まる
	ybillCameraAxisZ = XMVector3Cross(ybillCameraAxisX, ybillCameraAxisY);

	// Y軸回りビルボード行列
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
