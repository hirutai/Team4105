#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>

struct ConstBufferDataB1
{
	DirectX::XMFLOAT3 ambient; // アンビエント係数
	float pad1; // パディング
	DirectX::XMFLOAT3 diffuse; // ディフューズ係数
	float pad2; // パディング
	DirectX::XMFLOAT3 specular; // スペキュラー係数
	float alpha;	// アルファ
};