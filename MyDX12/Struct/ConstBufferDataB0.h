#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>

struct ConstBufferDataB0
{
	//XMMATRIX mat;	// �R�c�ϊ��s��
	DirectX::XMFLOAT4 color;
	DirectX::XMMATRIX viewproj;	// �r���\�v���W�F�N�V�����s��
	DirectX::XMMATRIX world;		// ���[���h�s��
	DirectX::XMFLOAT3 cameraPos;	// �J�������W(���[���h���W)

	DirectX::XMFLOAT4 lightpos;
	DirectX::XMMATRIX wlp;
	DirectX::XMMATRIX wlpt;

};

struct ConstBufferDataB01
{
	DirectX::XMFLOAT4 color;
	DirectX::XMMATRIX viewproj;	// �r���\�v���W�F�N�V�����s��
	DirectX::XMFLOAT3 cameraPos;	// �J�������W(���[���h���W)
};