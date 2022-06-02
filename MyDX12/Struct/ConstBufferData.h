#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>

struct ConstBufferData
{
	DirectX::XMFLOAT4 color;	// F(RGBA)
	DirectX::XMMATRIX mat;		// 3D•ÏŠ·s—ñ
};