#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include "../Base/DirectX12.h"
#include "../Struct/Math.h"

class ShadowMap
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	ShadowMap();

	void SetResourceBarrier(ID3D12GraphicsCommandList* cmdList, ComPtr<ID3D12PipelineState> pipelineState, ComPtr<ID3D12RootSignature> rootSignature);

	void RetResourceBarrier(ID3D12GraphicsCommandList* cmdList);

	void CreateDepthBuffer();

	void CreateDSV();

	void Initialize();

	// ShadowMap�̃o�b�t�@����n���p��Get�֐�
	ComPtr<ID3D12Resource> GetDepthBuffer() { return depthBuff; }

private:
	// �[�x�o�b�t�@
	ComPtr<ID3D12Resource> depthBuff;
	// DSV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;

};