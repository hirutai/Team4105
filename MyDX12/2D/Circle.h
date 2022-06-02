#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include "../Base/DirectX12.h"
#include "../Struct/Math.h"

class Circle
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using Vector2 = Math::Vector2;
	using Vector3 = Math::Vector3;

private:
	// ���_��
	static const int vertNum = 32;

public: // �T�u�N���X
	struct VertexPosUv
	{
		Math::Vector3 pos; // xyz���W
	};

	struct CbData
	{
		DirectX::XMMATRIX mat;		// 3D�ϊ��s��
		DirectX::XMFLOAT4 color;
		unsigned int active;
	};

	static const int dataIndexCount = 500;

	struct CBVIDParameter
	{
		CbData data[dataIndexCount];
	};

	struct CircleData
	{
		Vector2 position{};
		DirectX::XMFLOAT4 color{};
		float radian = 0.0f;
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

	static Circle* Create();

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Circle();

	void CreateVertexBuffer();

	void CreateConstBuffer();

	/// <summary>
	/// ������
	/// </summary>
	bool Initialize();

	/// <summary>
	/// �`��R�}���h�̔��s
	/// </summary>
	void DrawCircle(float x, float y, float rad, float r, float g, float b, float a);

	void Draw();

	void TransferVertices();
	void TransferCBVInstance();
	void ClearCircle() { if(datas.size() != 0)datas.clear(); }

private:
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	// �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;
	// �萔�o�b�t�@(Instance�p)
	ComPtr<ID3D12Resource> constBuff;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView = {};

	// ���W
	std::vector<CircleData> datas;
	// ���[���h�s��
	DirectX::XMMATRIX matWorld{};
};