#pragma once

#include <DirectXMath.h>

/// <summary>
/// �ۉe
/// </summary>
class CircleShadow
{
public: // �T�u�N���X
	// �萔�o�b�t�@�p�f�[�^�\����
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

public: // �����o�֐�

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


private: // �����o�ϐ�
	// ����(�P�ʃx�N�g��)
	DirectX::XMVECTOR dir = { 1,0,0,0 };
	// �L���X�^�[�ƃ��C�g�̋���
	float distanceCasterLight = 100.0f;
	// �L���X�^�[���W(���[���h���W�n)
	DirectX::XMFLOAT3 casterPos = { 0,0,0 };
	// ���������W��
	DirectX::XMFLOAT3 atten = { 0.5f,0.6f,0.0f };
	// �����p�x
	DirectX::XMFLOAT2 factorAngleCos = { 0.2f,0.5f };
	// �L���t���O
	bool active = false;
};