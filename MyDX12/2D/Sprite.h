#pragma once
#include "../Struct/ConstBufferData.h"
#include "../Pipeline/Pipeline.h"
#include "../Struct/Math.h"
#include "../System/Texture.h"

class Sprite
{
private: // �G�C���A�X
	using Vector2 = Math::Vector2;
	using Vector3 = Math::Vector3;

public: // �T�u�N���X
	struct VertexPosUv
	{
		Vector3 pos; // xyz���W
		Vector2 uv;  // uv���W
	};

public: // �ÓI�����o�֐�
	/// <summary>
	/// �ÓI������
	/// </summary>
	static bool StaticInitialize();

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	static bool LoadTexture(UINT texnumber, const wchar_t*filename);
	static bool LoadTexture(UINT texnumber, Texture texture);

	/// <summary>
	/// �`��O����
	/// </summary>
	static void PreDraw();

	/// <summary>
	/// �`��㏈��
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// �X�v���C�g����
	/// </summary>
	static Sprite* Create(UINT texNumber, Vector2 position, DirectX::XMFLOAT4 color = { 1, 1, 1, 1 }, Vector2 anchorpoint = { 0.0f, 0.0f }, bool isFlipX = false, bool isFlipY = false);

private: // �ÓI�����o�ϐ�
	// �e�N�X�`���̍ő喇��
	static const int srvCount = 512;
	// ���_��
	static const int vertNum = 4;
	// �f�o�C�X
	static ID3D12Device* device;
	// �f�X�N���v�^�T�C�Y
	static UINT descriptorHandleIncrementSize;
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
	// �e�N�X�`���o�b�t�@
	static Microsoft::WRL::ComPtr<ID3D12Resource> texBuff[srvCount];

public: // �����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Sprite(UINT texNumber, Vector2 position, Vector2 size, DirectX::XMFLOAT4 color, Vector2 anchorpoint, bool isFlipX, bool isFlipY);
	/// <summary>
	/// ������
	/// </summary>
	bool Initialize();
	/// <summary>
	/// �p�x�̐ݒ�
	/// </summary>
	void SetRotation(float rotation);
	/// <summary>
	/// ���W�̎擾
	/// </summary>
	const Vector2& GetPosition() { return position; }
	/// <summary>
	/// ���W�̐ݒ�
	/// </summary>
	void SetPosition(Vector2 position);
	/// <summary>
	/// �T�C�Y�̐ݒ�
	/// </summary>
	void SetSize(Vector2 size);
	/// <summary>
	/// �A���J�[�|�C���g�̐ݒ�
	/// </summary>
	void SetAnchorPoint(Vector2 anchorpoint);

	// RGB�l�̐ݒ�
	void SetColorRGB(float r, float g, float b);
	// ���l�̐ݒ�
	void SetAlpha(float alpha);
	// �F�̎l�v�f�̐ݒ�
	void SetColor(float r,float g,float b,float a);

	/// <summary>
	/// ���E���]�̐ݒ�
	/// </summary>
	void SetIsFlipX(bool isFlipX);

	/// <summary>
	/// �㉺���]�̐ݒ�
	/// </summary>
	void SetIsFlipY(bool isFlipY);

	/// <summary>
	/// �e�N�X�`���͈͐ݒ�
	/// </summary>
	void SetTextureRect(Vector2 texBase, Vector2 texSize);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	Vector2 GetDefault()const { return defaultSize; }

private: // �����o�ϐ�
	// ���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW spriteVBView{};

	// �e�N�X�`���ԍ�
	UINT texNumber = 0;
	// Z�����̉�]�p
	float rotation = 0.0f;
	// ���W
	Vector2 position{};
	// �X�v���C�g���A����
	Vector2 size = { 100.0f, 100.0f };
	Vector2 defaultSize = { 100.0f, 100.0f };
	// �A���J�[�|�C���g
	Vector2 anchorpoint = { 0, 0 };
	// ���[���h�s��
	DirectX::XMMATRIX matWorld{};
	// �F
	DirectX::XMFLOAT4 color = { 1, 1, 1, 1 };
	// ���E���]
	bool isFlipX = false;
	// �㉺���]
	bool isFlipY = false;
	// �e�N�X�`���n�_
	Vector2 texBase = { 0, 0 };
	// �e�N�X�`�����A����
	Vector2 texSize = { 100.0f, 100.0f };

private:
	/// <summary>
	/// ���_�f�[�^�]��
	/// </summary>
	void TransferVertices();
};

