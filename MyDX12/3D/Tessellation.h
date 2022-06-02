#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>
#include "../Base/DirectX12.h"
#include "../Struct/Math.h"
class DebugCamera;

class Tessellation
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using Vector3 = Math::Vector3;
	using Vector2 = Math::Vector2;

private:
	const float edge = 200.0f;
	const int divide = 10;

public: // �T�u�N���X
	struct VertexPosUv
	{
		Vector3 pos; // xyz���W
		Vector2 uv; // uv���W
	};

	struct CbData
	{
		XMFLOAT4 cameraPos;
		XMMATRIX mat;		// 3D�ϊ��s��
		XMMATRIX viewproj;
		XMFLOAT4 tessRange;
	};

private:
	// �f�o�C�X
	static ID3D12Device* device;
	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootSignature;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelineState;
	// �ˉe�s��
	static DirectX::XMMATRIX matProjection;
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
		Tessellation::d_camera = d_camera;
	}

	/// <summary>
	/// �`��O����
	/// </summary>
	static void PreDraw();

	/// <summary>
	/// �`��㏈��
	/// </summary>
	static void PostDraw();

	static Tessellation* Create();

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Tessellation();

	void CreateVertexBuffer();
	void CreateIndexBuffer();

	void CreateConstBuffer();

	ComPtr<ID3D12Resource> LoadTextureFromFile(std::string& texPath);

	XMMATRIX GetProjectionMatrix(float fov, float aspect, float znear, float zfar)
	{
		auto mtxProj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fov), aspect, znear, zfar);
		return mtxProj;
	}

	void CreateTexture();
	void CreateSRV();

	/// <summary>
	/// ������
	/// </summary>
	bool Initialize();

	void Update();

	/// <summary>
	/// �`��R�}���h�̔��s
	/// </summary>
	void Draw();

	void VerticesStream();

	void TransferVertices();

	void IndexStream();

private:
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	// �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView = {};

	// �e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texBuff[2];

	// SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

	std::vector<VertexPosUv> vertices;
	std::vector<UINT> indices;

	// ���W
	Math::Vector3 position{};

	// ���[���h�s��
	DirectX::XMMATRIX matWorld{};

};