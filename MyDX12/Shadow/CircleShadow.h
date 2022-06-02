#pragma once

#include <DirectXMath.h>

/// <summary>
/// 丸影
/// </summary>
class CircleShadow
{
public: // サブクラス
	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		DirectX::XMVECTOR dir;
		DirectX::XMFLOAT3 casterPos;
		float distanceCasterLight;
		DirectX::XMFLOAT3 atten;
		float pad3;
		DirectX::XMFLOAT2 factorAngleCos;
		unsigned int active;
		float pad4;
	};

public: // メンバ関数

	inline void SetDir(const DirectX::XMVECTOR& dir) { this->dir = DirectX::XMVector3Normalize(dir); }
	inline const DirectX::XMVECTOR& GetDir() { return dir; }
	inline void SetCasterPos(const DirectX::XMFLOAT3& casterPos) { this->casterPos = casterPos;}
	inline const DirectX::XMFLOAT3& GetCasterPos() { return casterPos; }
	inline void SetDistanceCasterLight(float distanceCasterLight) { this->distanceCasterLight = distanceCasterLight; }
	inline float GetDistanceCasterLight() { return distanceCasterLight; }
	inline void SetAtten(const DirectX::XMFLOAT3& atten) { this->atten = atten; }
	inline const DirectX::XMFLOAT3& GetAtten() { return atten; }
	inline void SetFactorAngle(const DirectX::XMFLOAT2& factorAngle)
	{
		this->factorAngleCos.x = cosf(DirectX::XMConvertToRadians(factorAngle.x));
		this->factorAngleCos.y = cosf(DirectX::XMConvertToRadians(factorAngle.y));
	}
	inline const DirectX::XMFLOAT2& GetFactorAngleCos() { return factorAngleCos; }
	inline void SetActive(bool active) { this->active = active; }
	inline bool IsActive() { return active; }


private: // メンバ変数
	// 方向(単位ベクトル)
	DirectX::XMVECTOR dir = { 1,0,0,0 };
	// キャスターとライトの距離
	float distanceCasterLight = 100.0f;
	// キャスター座標(ワールド座標系)
	DirectX::XMFLOAT3 casterPos = { 0,0,0 };
	// 距離減衰係数
	DirectX::XMFLOAT3 atten = { 0.5f,0.6f,0.0f };
	// 減衰角度
	DirectX::XMFLOAT2 factorAngleCos = { 0.2f,0.5f };
	// 有効フラグ
	bool active = false;
};