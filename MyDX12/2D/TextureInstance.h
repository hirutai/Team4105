#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include "../Base/DirectX12.h"
#include "../Struct/Math.h"
#include "../System/Texture.h"

class TextureInstance
{
private: // Alias
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using Vector2 = Math::Vector2;
	using Vector3 = Math::Vector3;

public: // Sub
	struct VertexPosUv
	{
		Vector3 pos;
		Vector2 uv;
	};

	struct CbData // �ʂ̏��
	{
		DirectX::XMMATRIX mat;
		DirectX::XMFLOAT4 color;
		unsigned int active;
	};

	static const int dataIndexCount = 500;

	struct CBVIDParameter
	{
		CbData data[dataIndexCount];
	};

	struct SpData
	{
		Vector2 position{};
		DirectX::XMFLOAT4 color{};
		float rotation = 0.0f;
		Vector2 scale{};
	};

private:
	// ���_��
	static const int vertNum = 4;

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
	static ComPtr<ID3D12Resource>whiteTexBuff;

public: // �ÓI�����o�֐�
/// <summary>
/// �ÓI������
/// </summary>
	static bool StaticInitialize();

	/// <summary>
	/// �p�C�v���C������
	/// </summary>
	static void CreateGraphicsPipelineState();

	static bool CheckShaderCompileResultTIS(HRESULT result, ID3DBlob* error);

	/// <summary>
	/// �`��O����
	/// </summary>
	static void PreDraw();

	/// <summary>
	/// �`��㏈��
	/// </summary>
	static void PostDraw();

	static TextureInstance* Create();

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	TextureInstance();

	void CreateVertexBuffer();

	void CreateConstBuffer();

	/// <summary>
	/// ������
	/// </summary>
	bool Initialize();

	/// <summary>
	/// �`��R�}���h�̔��s
	/// </summary>
	void DrawTexture(float x, float y, float rot = 0.0f,float sx = 1.0f,float sy = 1.0f, float r = 255.0f, float g = 255.0f, float b = 255.0f, float a = 255.0f);
	void DrawTexture(Vector2 pos, float rot = 0.0f, Vector2 scale = { 1,1 }, Math::Vector4 color = {255,255,255,255 });
	void DrawTexture(Math::IMG_DATA data);

	void Draw();

	void SetTexture(Texture& tex);
	void SetSRV();
	void TransferVertices();
	void StreamTVBuff();
	void TransferCBVInstance();
	void ClearTextureInstance() { if (datas.size() != 0)datas.clear(); }

	void SetSize(Vector2 size)
	{
		this->size = size;

		TransferVertices();
	}

	void SetAnchorPoint(Vector2 anchorpoint)
	{
		this->anchorpoint = anchorpoint;

		TransferVertices();
	}

	void SetIsFlipX(bool isFlipX)
	{
		this->isFlipX = isFlipX;

		TransferVertices();
	}

	void SetIsFlipY(bool isFlipY)
	{
		this->isFlipY = isFlipY;

		TransferVertices();
	}

	void SetTextureRect(Vector2 texBase, Vector2 texSize)
	{
		this->texBase = texBase;
		this->texSize = texSize;

		TransferVertices();
	}

	Vector2 GetRectSize()
	{
		return texSize;
	}

private:
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	// �萔�o�b�t�@(Instance�p)
	ComPtr<ID3D12Resource> constBuff;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	ComPtr<ID3D12Resource> texBuff;

	// ���W
	std::vector<Math::IMG_DATA> datas;
	
	// �X�v���C�g���A����
	Vector2 size = { 100.0f, 100.0f };
	// �A���J�[�|�C���g
	Vector2 anchorpoint = { 0, 0 };
	// ���E���]
	bool isFlipX = false;
	// �㉺���]
	bool isFlipY = false;
	// �e�N�X�`���n�_
	Vector2 texBase = { 0, 0 };
	// �e�N�X�`�����A����
	Vector2 texSize = { 100.0f, 100.0f };
};