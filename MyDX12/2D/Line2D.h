#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include "../Base/DirectX12.h"
#include "../Struct/Math.h"

class Line2D
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using Vector3 = Math::Vector3;
	using Matrix4 = Math::Matrix4;
private:
	// ���_��
	static const int vertNum = 2;

public: // �T�u�N���X
	struct VertexPosUv
	{
		Math::Vector3 pos; // xyz���W
	};

	struct CbData
	{
		DirectX::XMMATRIX mat;		// 3D�ϊ��s��
		DirectX::XMFLOAT2 xm_start;
		DirectX::XMFLOAT2 xm_end;
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
	// �f�X�N���v�^�q�[�v
	static ComPtr<ID3D12DescriptorHeap> descHeap;

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
	/// �`��O����
	/// </summary>
	static void PreDraw();

	/// <summary>
	/// �`��㏈��
	/// </summary>
	static void PostDraw();

	static Line2D* Create(Math::Vector2 start, Math::Vector2 end);

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Line2D(Math::Vector2 start, Math::Vector2 end);

	void CreateVertexBuffer();

	void CreateConstBuffer();

	/// <summary>
	/// ������
	/// </summary>
	bool Initialize();

	/// <summary>
	/// �`��R�}���h�̔��s
	/// </summary>
	void Draw();

	void TransferVertices();

	void SetStP(const Math::Vector2 st) { start = st; TransferVertices(); }
	void SetStP(const float x, const float y) { start = { x,y }; TransferVertices(); }

	void SetEdP(const Math::Vector2 ed) { end = ed; TransferVertices(); }
	void SetEdP(const float x, const float y) { end = { x,y }; TransferVertices(); }

private:
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

	Math::Vector2 start = Math::Vector2(100, 400);
	Math::Vector2 end = Math::Vector2(1000, 400);
	// ���[���h�s��
	DirectX::XMMATRIX matWorld{};
};