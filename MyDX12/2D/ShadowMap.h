#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include "../Base/DirectX12.h"
#include "../Struct/Math.h"

class ShadowMap
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ShadowMap();

	void SetResourceBarrier(ID3D12GraphicsCommandList* cmdList, ComPtr<ID3D12PipelineState> pipelineState, ComPtr<ID3D12RootSignature> rootSignature);

	void RetResourceBarrier(ID3D12GraphicsCommandList* cmdList);

	void CreateDepthBuffer();

	void CreateDSV();

	void Initialize();

	// ShadowMapのバッファ情報を渡す用のGet関数
	ComPtr<ID3D12Resource> GetDepthBuffer() { return depthBuff; }

private:
	// 深度バッファ
	ComPtr<ID3D12Resource> depthBuff;
	// DSV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;

};