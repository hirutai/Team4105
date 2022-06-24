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

	float c_fov;//画角(度数法)(FOV)

	float c_near;//前端(near)

	float c_far;//奥端(far)

	Math::Vector3 at = Math::Vector3();

	DirectX::XMMATRIX matView; // ビュー変換行列

	DirectX::XMMATRIX matProjection;			//射影変換行列

	// ビルボード行列
	DirectX::XMMATRIX matBillboard;
	// Y軸回りビルボード行列
	DirectX::XMMATRIX matBillboardY;

public:
	DebugCamera();
	~DebugCamera();
	// カメラ初期化
	void _Initialize(float lookatrange, float rotaterad, float moverange);
	// カメラ位置更新
	void _Update();

	// phi値の設定
	void SetPhi(float s_phi);
	void SetAdderPhi(float adder_phi);
	// theta値の設定
	void SetTheta(float s_theta);
	void SetAdderTheta(float adder_theta);
	// lookatrangeの設定
	void SetLookAtRange(float s_lookat_x, float s_lookat_y, float s_lookat_z);
	void SetAdderLookAt(float adder_lookat_x, float adder_lookat_y, float adder_lookat_z);
	// positionの設定
	void SetPosition(float s_pos_x, float s_pos_y, float s_pos_z);
	// MoveRangeの設定
	void SetMoveRange(float move_range);
	// atの設定
	void SetAt(float s_at_x,float s_at_y,float s_at_z);
	// 上方向のベクトル
	void SetUpVector(float x, float y, float z);

	// eyeのGet()...x,y,zは個々に作る
	float GetEye_X()const { return eye.x; }
	float GetEye_Y()const { return eye.y; }
	float GetEye_Z()const { return eye.z; }
	// atのGet()...x,y,zは個々に作る
	float GetAt_X()const { return at.x; }
	float GetAt_Y()const { return at.y; }
	float GetAt_Z()const { return at.z; }
	// rotatedのGet()
	float GetRotaterad();
	// phiのGet()
	float GetPhi()const { return data.phi; }
	// thetaのGet()
	float GetTheta()const { return data.theta; }
	// lookAtのGet()...x,y,zは個々に作る
	float GetLookAtRange_X()const { return data.lookat.x; }
	float GetLookAtRange_Y()const { return data.lookat.y; }
	float GetLookAtRange_Z()const { return data.lookat.z; }
	// positionのGet()...x,y,zは個々に作る
	float GetPos_X()const { return data.position.x; }
	float GetPos_Y()const { return data.position.y; }
	float GetPos_Z()const { return data.position.z; }
	// rightVecのGet()...x,y,zは個々に作る
	float GetRightVector_X()const;
	float GetRightVector_Y()const;
	float GetRightVector_Z()const;
	// upVecのGet()...x,y,zは個々に作る
	float GetUpVector_X()const;
	float GetUpVector_Y()const;
	float GetUpVector_Z()const;
	// moverangeのGet()
	float GetMoveRange()const { return moverange; }

	// プロジェクション行列のGet()
	DirectX::XMMATRIX GetMatProjection()const { return matProjection; }
	// ビュー行列のGet()
	DirectX::XMMATRIX GetMatView()const { return matView; }
	// ビュープロジェクション行列のGet()
	DirectX::XMMATRIX GetMatViewProjection()const { return matView * matProjection; }
	// ビルボード専用行列のGet()
	DirectX::XMMATRIX GetMatBillboard()const { return matBillboard; }
	DirectX::XMMATRIX GetMatYBillboard()const { return matBillboardY; }
};