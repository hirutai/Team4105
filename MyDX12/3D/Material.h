#pragma once
#include "../Struct/ConstBufferDataB1.h"
#include <d3dx12.h>
#include <string>
#include "../System/Texture.h"

/// <summary>
/// �}�e���A��
/// </summary>
class Material
{
public: // �ÓI�����o�֐�
	/// <summary>
	/// �ÓI������
	/// </summary>
	static void StaticInitialize(ID3D12Device* device);
	
	/// <summary>
	/// �}�e���A������
	/// </summary>
	static Material* Create();

private: // �ÓI�����o�ϐ�
	static ID3D12Device* device;
public:
	std::string name;	// �}�e���A����
	DirectX::XMFLOAT3 ambient;	// �A���r�G���g�e���x
	DirectX::XMFLOAT3 diffuse;	// �f�B�t���[�Y�e���x
	DirectX::XMFLOAT3 specular;	// �X�y�L�����[�e���x
	float alpha;		// �A���t�@
	std::string textureFilename;	// �e�N�X�`���t�@�C����

public:
	/// <summary>
	/// �萔�o�b�t�@�̎擾
	/// </summary>
	ID3D12Resource* GetConstantBuffer() { return constBuff.Get(); }

	/// �e�N�X�`���ǂݍ���
	void LoadTexture(const std::string& directoryPath, CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandle, CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle);
	void LoadTexture(Texture texture, CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandle, CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle);
	void Update();

	const CD3DX12_CPU_DESCRIPTOR_HANDLE& GetCpuHandle() { return cpuDescHandleSRV; }
	const CD3DX12_GPU_DESCRIPTOR_HANDLE& GetGpuHandle() { return gpuDescHandleSRV; }

private:
	// �e�N�X�`���o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> texbuff;
	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;

private:
	// �R���X�g���N�^
	Material() {
		ambient = { 0.3f, 0.3f, 0.3f };
		diffuse = { 0.0f, 0.0f, 0.0f };
		specular = { 0.0f, 0.0f, 0.0f };
		alpha = 1.0f;
	}

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �萔�o�b�t�@�̐���
	/// </summary>
	void CreateConstantBuffer();
};

