#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include "../Base/DirectX12.h"
#include "../Struct/Math.h"
class PostEffect
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
private:
	// ��ʃN���A�J���[
	static const float clearColor[4];

	// ���_��
	static const int vertNum = 4;

public: // �T�u�N���X
	struct VertexPosUv
	{
		Math::Vector3 pos; // xyz���W
		Math::Vector2 uv;  // uv���W
	};

	struct CbData
	{
		DirectX::XMMATRIX mat;		// 3D�ϊ��s��
		float _Time;
		float _density;
		int _type;
	};
	
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	PostEffect();

	/// <summary>
	/// �p�C�v���C������
	/// </summary>
	void CreateGraphicsPipelineState();

	/// <summary>
	/// �V�[���`��O����
	/// </summary>
	/// <param name = "cmdList">�R�}���h���X�g</param>
	void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �V�[���`��㏈��
	/// </summary>
	/// <param name = "cmdList">�R�}���h���X�g</param>
	void PostDrawScene(ID3D12GraphicsCommandList* cmdList);
	void PostLDrawScene(ID3D12GraphicsCommandList* cmdList);

	void SetResourceBarrier(ID3D12GraphicsCommandList* cmdList, ComPtr<ID3D12PipelineState> pipelineState, ComPtr<ID3D12RootSignature> rootSignature);

	void RetResourceBarrier(ID3D12GraphicsCommandList* cmdList);

	void CreateVertexBuffer();

	void CreateConstBuffer();

	void CreateTexBuffer();

	void CreateSRV();

	void CreateRTV();

	void CreateDepthBuffer();

	void CreateDSV();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �`��R�}���h�̔��s
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	void TransferVertices();

	// ���T�C�Y
	void SetSize(const Math::Vector2& wh) { size = wh; TransferVertices();}
	void SetSize(const float wid, const float hig) { size = {wid,hig}; TransferVertices();}

	// ���W��Seter
	void SetPosition(const Math::Vector2& pos) { position = pos;}
	void SetPosition(const float x, const float y) { position = { x,y };}

	void SetAncPoint(const Math::Vector2& ap) { anchorpoint = ap; TransferVertices();}
	void SetAncPoint(const float x, const float y) { anchorpoint = { x,y }; TransferVertices();}

	void SetDensity(float dens) { density = dens; }
	void SetTypeEffect(int typ) { postType = typ; }

	void SetZBuffer(ComPtr<ID3D12Resource> pZ) { zTex = pZ; }

private:
	ID3D12Device* device = nullptr;
	// ����
	float _time = 0.0f;
	float density = 0.0f;
	int postType = 0;
	// �e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texBuff;
	// SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

	// �[�x�o�b�t�@
	ComPtr<ID3D12Resource> depthBuff;
	// �[�x�e�N�X�`��
	ComPtr<ID3D12Resource> sMapTex;
	// Z�e�N�X�`��
	ComPtr<ID3D12Resource> zTex;
	// RTV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	// DSV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;

	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	// Z�����̉�]�p
	float rotation = 0.0f;
	// ���W
	Math::Vector2 position{};
	// �X�v���C�g���A����
	Math::Vector2 size = { 100.0f, 100.0f };
	// �A���J�[�|�C���g
	Math::Vector2 anchorpoint = { 0, 0 };
	// ���[���h�s��
	DirectX::XMMATRIX matWorld{};
	// �ˉe�s��
	DirectX::XMMATRIX matProjection;
	// ���E���]
	bool isFlipX = false;
	// �㉺���]
	bool isFlipY = false;

	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	ComPtr<ID3D12PipelineState> pipelineState;
	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootSignature;
};