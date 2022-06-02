#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <forward_list>
#include "../Pipeline/Pipeline.h"
#include "../Struct/Math.h"
class DebugCamera;

class ParticleManager
{
private: // �G�C���A�X
	using Vector2 = Math::Vector2;
	using Vector3 = Math::Vector3;

public: // �T�u�N���X
// ���_�f�[�^�\����
	struct VertexPos
	{
		Vector3 pos; // xyz���W
		float scale; // �X�P�[��
		DirectX::XMFLOAT4 color;	// �F (RGBA)
	};

	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		DirectX::XMMATRIX mat;	// �r���[�v���W�F�N�V�����s��
		DirectX::XMMATRIX matBillboard;	// �r���{�[�h�s��
	};

	// �p�[�e�B�N��1��
	class Particle
	{
		// Microsoft::WRL::���ȗ�
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
		// DirectX::���ȗ�
		using XMFLOAT4 = DirectX::XMFLOAT4;
		using XMMATRIX = DirectX::XMMATRIX;
	public:
		// ���W
		Vector3 position = {};
		// ���x
		Vector3 velocity = {};
		// �����x
		Vector3 accel = {};
		// �F
		Vector3 color = {};
		// �X�P�[��
		float scale = 1.0f;
		// ��]
		float rotation = 0.0f;
		// �����l
		Vector3 s_color = {};
		float s_scale = 1.0f;
		float s_rotation = 0.0f;
		// �ŏI�l
		Vector3 e_color = {};
		float e_scale = 0.0f;
		float e_rotation = 0.0f;
		// ���݃t���[��
		int frame = 0;
		// �I���t���[��
		int num_frame = 0;
	};

private: // �萔
	static const int vertexCount = 65536;		// ���_��

public:// �ÓI�����o�֐�
	static ParticleManager* GetInstance();

public: // �����o�֐�	
/// <summary>
/// ������
/// </summary>
/// <returns></returns>
	void Initialize(std::string textureName);
	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	inline void SetDebugCamera(DebugCamera* d_camera) { this->d_camera = d_camera; }

	/// <summary>
	/// �p�[�e�B�N���̒ǉ�
	/// </summary>
	/// <param name="life">��������</param>
	/// <param name="position">�������W</param>
	/// <param name="velocity">���x</param>
	/// <param name="accel">�����x</param>
	/// <param name="start_scale">�J�n���X�P�[��</param>
	/// <param name="end_scale">�I�����X�P�[��</param>
	void Add(int life, Vector3 position, Vector3 velocity, Vector3 accel, float start_scale, float end_scale);

	/// <summary>
	/// �f�X�N���v�^�q�[�v�̏�����
	/// </summary>
	/// <returns></returns>
	void InitializeDescriptorHeap();

	/// <summary>
	/// �O���t�B�b�N�p�C�v���C������
	/// </summary>
	/// <returns>����</returns>
	void InitializeGraphicsPipeline();

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	/// <returns>����</returns>
	void LoadTexture(std::string textureName);

	/// <summary>
	/// ���f���쐬
	/// </summary>
	void CreateModel();

private: // �����o�ϐ�
// �f�o�C�X
	ID3D12Device* device = nullptr;
	// �f�X�N���v�^�T�C�Y
	UINT descriptorHandleIncrementSize = 0u;
	// ���[�g�V�O�l�`��
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootsignature;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate;
	// �f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;
	// ���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
	// �e�N�X�`���o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> texbuff;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView;
	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;
	// �p�[�e�B�N���z��
	std::forward_list<Particle> particles;
	// �J����
	DebugCamera* d_camera = nullptr;

	private:
		ParticleManager() = default;
		ParticleManager(const ParticleManager&) = delete;
		~ParticleManager() = default;
		ParticleManager& operator=(const ParticleManager&) = delete;
};

