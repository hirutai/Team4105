#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>

struct ConstBufferDataB0
{
	//XMMATRIX mat;	// ３Ｄ変換行列
	DirectX::XMFLOAT4 color;
	DirectX::XMMATRIX viewproj;	// ビュ―プロジェクション行列
	DirectX::XMMATRIX world;		// ワールド行列
	DirectX::XMFLOAT3 cameraPos;	// カメラ座標(ワールド座標)

	DirectX::XMFLOAT4 lightpos;
	DirectX::XMMATRIX wlp;
	DirectX::XMMATRIX wlpt;
	DirectX::XMFLOAT3 pd;

	float _time;					// シェーダー内部で使うタイマー
	float isElec;					// シェーダータイプ
};

struct ConstBufferDataB01
{
	DirectX::XMFLOAT4 color;
	DirectX::XMMATRIX viewproj;	// ビュ―プロジェクション行列
	DirectX::XMFLOAT3 cameraPos;	// カメラ座標(ワールド座標)
};