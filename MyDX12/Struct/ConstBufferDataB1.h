#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>

struct ConstBufferDataB1
{
	DirectX::XMFLOAT3 ambient; // �A���r�G���g�W��
	float pad1; // �p�f�B���O
	DirectX::XMFLOAT3 diffuse; // �f�B�t���[�Y�W��
	float pad2; // �p�f�B���O
	DirectX::XMFLOAT3 specular; // �X�y�L�����[�W��
	float alpha;	// �A���t�@
};