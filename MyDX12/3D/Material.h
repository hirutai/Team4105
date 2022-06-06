#pragma once
#include "../Struct/ConstBufferDataB1.h"
#include <d3dx12.h>
#include <string>
#include "../System/Texture.h"

/// <summary>
/// マテリアル
/// </summary>
class Material
{
public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	static void StaticInitialize(ID3D12Device* device);
	
	/// <summary>
	/// マテリアル生成
	/// </summary>
	static Material* Create();

private: // 静的メンバ変数
	static ID3D12Device* device;
public:
	std::string name;	// マテリアル名
	DirectX::XMFLOAT3 ambient;	// アンビエント影響度
	DirectX::XMFLOAT3 diffuse;	// ディフューズ影響度
	DirectX::XMFLOAT3 specular;	// スペキュラー影響度
	float alpha;		// アルファ
	std::string textureFilename;	// テクスチャファイル名

public:
	/// <summary>
	/// 定数バッファの取得
	/// </summary>
	ID3D12Resource* GetConstantBuffer() { return constBuff.Get(); }

	/// テクスチャ読み込み
	void LoadTexture(const std::string& directoryPath, CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandle, CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle);
	void LoadTexture(Texture texture, CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandle, CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle);
	void Update();

	const CD3DX12_CPU_DESCRIPTOR_HANDLE& GetCpuHandle() { return cpuDescHandleSRV; }
	const CD3DX12_GPU_DESCRIPTOR_HANDLE& GetGpuHandle() { return gpuDescHandleSRV; }

private:
	// テクスチャバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> texbuff;
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;

private:
	// コンストラクタ
	Material() {
		ambient = { 0.3f, 0.3f, 0.3f };
		diffuse = { 0.0f, 0.0f, 0.0f };
		specular = { 0.0f, 0.0f, 0.0f };
		alpha = 1.0f;
	}

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 定数バッファの生成
	/// </summary>
	void CreateConstantBuffer();
};

