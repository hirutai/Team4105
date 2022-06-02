#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include "../Base/DirectX12.h"
#include "../Struct/Math.h"

class DebugCamera;

class CollisionCircle
{
public: // �T�u�N���X
	struct VertexPosUv
	{
		Math::Vector3 pos; // xyz���W
	};

	struct CbData
	{
		DirectX::XMMATRIX mat;		// 3D�ϊ��s��
		DirectX::XMMATRIX viewproj;
		DirectX::XMFLOAT4 color;
	};

private:
	// �f�o�C�X
	static ID3D12Device* device;
	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	// ���[�g�V�O�l�`��
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	// �ˉe�s��
	static DirectX::XMMATRIX matProjection;
	// �f�X�N���v�^�q�[�v
	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;
	// �J����
	static DebugCamera* d_camera;

public: // �ÓI�����o�֐�
	/// <summary>
	/// �ÓI������
	/// </summary>
	static bool StaticInitialize();

	/// <summary>
	/// �p�C�v���C������
	/// </summary>
	static void CreateGraphicsPipelineState();

	/// <summary>
	/// �J�����̃Z�b�g
	/// </summary>
	static void SetDebugCamera(DebugCamera* d_camera)
	{
		CollisionCircle::d_camera = d_camera;
	}

	/// <summary>
	/// �`��O����
	/// </summary>
	static void PreDraw();

	/// <summary>
	/// �`��㏈��
	/// </summary>
	static void PostDraw();

	static CollisionCircle* Create(float radius = 1.0f,int v_count = 8);

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CollisionCircle(float radius, int v_count);

	void CreateVertexBuffer();

	void CreateConstBuffer();

	/// <summary>
	/// ������
	/// </summary>
	bool Initialize();

	void Update();

	/// <summary>
	/// �`��R�}���h�̔��s
	/// </summary>
	void Draw();

	void TransferVertices();

	void CreateIndexBuffer();

	void SetMesh();

	void SetVertexCount(int count_v);

	void SetPosition(float x, float y, float z) { position = { x,y,z }; }

	void SetScale(float sx, float sy, float sz) { scale = { sx,sy,sz }; }

	void SetColor(float r, float g, float b, float a) { color = { r,g,b,a }; }

	float GetRadius()const { return radius; }

	Math::Vector3 GetPosition()const { return position; }

private:
	// ���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
	// �C���f�b�N�X�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff;
	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView = {};

	unsigned int vert_count = 8;

	// ���W
	Math::Vector3 position{};

	DirectX::XMFLOAT4 color;
	float radius = 0.0f;
	Math::Vector3 scale{};
	// ���[���h�s��
	DirectX::XMMATRIX matWorld{};
};